#include "BuilderImpl.h"
#include "Configuration.h"
#include <string>
#include <thread>
#include <cmath>


void BuilderImpl::BuildColumnSummary(const BuilderImpl::Column &column,
                                     BuilderImpl::ColumnSummary &columnSummary)
{
    auto &attributes = columnSummary._attributes;
    for (const auto &element : column)
    {
        auto itr = attributes.find(element);
        if (attributes.end() != itr)
        {
            attributes[element]++;
        }
        else
        {
            attributes[element] = 1;
        }
    }

    columnSummary._total = column.size();
}


void BuilderImpl::BuildConditionedColumnSummary(const BuilderImpl::Column &outputColumn,
                                                const BuilderImpl::Column &conditionColumn,
                                                const std::string &conditionValue,
                                                BuilderImpl::ColumnSummary &columnSummary)
{
    auto &attributes = columnSummary._attributes;
    auto &total = columnSummary._total;
    total = 0;
    for (std::size_t ii = 0; ii < outputColumn.size(); ii++)
    {
        if (conditionColumn[ii] == conditionValue)
        {
            auto itr = attributes.find(outputColumn[ii]);
            if (attributes.end() != itr)
            {
                attributes[outputColumn[ii]]++;
            }
            else
            {
                attributes[outputColumn[ii]] = 1;
            }

            total++;
        }
    }
}


BuilderImpl::DecisionNodeSP BuilderImpl::BuildNodeFromMode(
    const std::string &message,
    const BuilderImpl::Header &header,
    const BuilderImpl::ColumnSummary &outputColumnSummary)
{
    std::size_t maxCounter = 0;
    std::string maxAttribute;
    for (const auto &attributeAndCounter : outputColumnSummary._attributes)
    {
        if (attributeAndCounter.second > maxCounter)
        {
            maxCounter = attributeAndCounter.second;
            maxAttribute = attributeAndCounter.first;
        }
    }

    std::string rule = message;
    rule += " predict the majority output => ";
    rule += header;
    rule += " = ";
    rule += maxAttribute;
    return std::make_shared<BuilderImpl::DecisionNode>(rule);
}


BuilderImpl::DecisionNodeSP BuilderImpl::BuildNodeEqualOutput(const std::string &message,
                                                              const BuilderImpl::Header &header,
                                                              BuilderImpl::ColumnSummary &outputColumnSummary)
{
    std::string rule = message;
    rule += " predict this unique output => ";
    rule += header;
    rule += " = ";
    rule += outputColumnSummary._attributes.begin()->first;
    return std::make_shared<BuilderImpl::DecisionNode>(rule);
}


BuilderImpl::DecisionNodeSP BuilderImpl::BuildNodeJustOutput(const std::string &message,
                                                             const BuilderImpl::Header &header,
                                                             BuilderImpl::ColumnSummary &outputColumnSummary)
{
    if (outputColumnSummary._attributes.size() == 1)
    {
        return BuildNodeEqualOutput(message, header, outputColumnSummary);
    }
    else
    {
        return BuildNodeFromMode(message, header, outputColumnSummary);
    }
}


bool BuilderImpl::CheckInputRowsAreEqual(const std::vector<BuilderImpl::ColumnSummary> inputColumnSumaries)
{
    bool areAqual = true;
    size_t ii = 0;
    while (areAqual && ii < inputColumnSumaries.size())
    {
        if (inputColumnSumaries[ii]._attributes.size() != 1)
        {
            areAqual = false;
        }
        else
        {
            ii++;
        }
    }

    return areAqual;
}


double BuilderImpl::CalculateEntropy(const BuilderImpl::ColumnSummary &columnSummary)
{
    if (columnSummary._attributes.size() == 1)
    {
        return 0.0;
    }

    double total = 0;
    for (const auto &attribute : columnSummary._attributes)
    {
        double probability = static_cast<double>(attribute.second) / columnSummary._total;
        total -= (probability * std::log2(probability));
    }

    return total;
}


void BuilderImpl::CalculateConditionalEntropy(const BuilderImpl::Column &outputColumn,
                                              const BuilderImpl::Column &inputColumn,
                                              const BuilderImpl::ColumnSummary &inputColumnSummary,
                                              double &conditionalEntropy)
{
    auto &attributes = inputColumnSummary._attributes;
    const size_t numberOfAttributes = attributes.size();

    std::vector<BuilderImpl::ColumnSummary> conditionedColumnSummaries;
    conditionedColumnSummaries.resize(numberOfAttributes);

    std::vector<double> entropies;
    entropies.resize(numberOfAttributes);

    conditionalEntropy = 0.0;
    for (const auto &attribute : attributes)
    {
        BuilderImpl::ColumnSummary conditionedSummary;
        BuilderImpl::BuildConditionedColumnSummary(outputColumn,
                                                   inputColumn,
                                                   attribute.first,
                                                   conditionedSummary);
        double attributeEntropy = CalculateEntropy(conditionedSummary);
        double attributeProbability = static_cast<double>(attribute.second) / inputColumnSummary._total;
        conditionalEntropy += (attributeEntropy * attributeProbability);
    }
}


void BuilderImpl::BuildColumnFromAttribute(const BuilderImpl::Column &fromColumn,
                                           const BuilderImpl::Column &attributeColumn,
                                           const std::string &attribute,
                                           BuilderImpl::Column &toColumn)
{
    toColumn.clear();
    for (size_t ii = 0; ii < fromColumn.size(); ii++)
    {
        if (attributeColumn[ii] == attribute)
        {
            toColumn.push_back(fromColumn[ii]);
        }
    }
}


void BuilderImpl::BuildTableFromAttribute(const Table &table,
                                          size_t attributeColumnIndex,
                                          const std::string &attribute,
                                          Table &newTable)
{
    if (Configuration::Parallelize(table.getNumberOfInputHeaders()))
    {
        BuildTableFromAttributeParallelized(table, attributeColumnIndex, attribute, newTable);
    }
    else
    {
        BuildTableFromAttributeNotParallelized(table, attributeColumnIndex, attribute, newTable);
    }
}


void BuilderImpl::BuildTableFromAttributeParallelized(const Table &table,
                                                      size_t attributeColumnIndex,
                                                      const std::string &attribute,
                                                      Table &newTable)
{
    BuilderImpl::Column newOutputColumn;
    BuildColumnFromAttribute(table.getOutputColumn(),
                             table.getInputColumn(attributeColumnIndex),
                             attribute,
                             newOutputColumn);

    newTable.setOutputHeaderAndColumn(table.getOutputHeader(), std::move(newOutputColumn));

    Headers inputHeaders(table.getNumberOfInputHeaders() - 1);
    Columns inputColumns(table.getNumberOfInputHeaders() - 1);
    std::vector<std::thread> threads;

    for (size_t ii = 0; ii < attributeColumnIndex; ii++)
    {
        inputHeaders[ii] = table.getInputHeader(ii);
        threads.emplace_back(std::thread(BuildColumnFromAttribute,
                                         std::cref(table.getInputColumn(ii)),
                                         std::cref(table.getInputColumn(attributeColumnIndex)),
                                         std::cref(attribute),
                                         std::ref(inputColumns[ii])));
    }

    for (size_t ii = attributeColumnIndex + 1; ii < table.getNumberOfInputHeaders(); ii++)
    {
        inputHeaders[ii - 1] = table.getInputHeader(ii);
        threads.emplace_back(std::thread(BuildColumnFromAttribute,
                                         std::cref(table.getInputColumn(ii)),
                                         std::cref(table.getInputColumn(attributeColumnIndex)),
                                         std::cref(attribute),
                                         std::ref(inputColumns[ii - 1])));
    }

    for (std::size_t ii = 0; ii < inputHeaders.size(); ii++)
    {
        threads[ii].join();
    }

    for (std::size_t ii = 0; ii < inputHeaders.size(); ii++)
    {
        newTable.addInputHeaderAndColumn(inputHeaders[ii], std::move(inputColumns[ii]));
    }
}


void BuilderImpl::BuildTableFromAttributeNotParallelized(const Table &table,
                                                         size_t attributeColumnIndex,
                                                         const std::string &attribute,
                                                         Table &newTable)
{
    BuilderImpl::Column newOutputColumn;
    BuildColumnFromAttribute(table.getOutputColumn(),
                             table.getInputColumn(attributeColumnIndex),
                             attribute,
                             newOutputColumn);

    newTable.setOutputHeaderAndColumn(table.getOutputHeader(), std::move(newOutputColumn));

    for (size_t ii = 0; ii < table.getNumberOfInputHeaders(); ii++)
    {
        if (ii == attributeColumnIndex)
        {
            continue;
        }

        BuilderImpl::Column newInputColumn;
        BuildColumnFromAttribute(table.getInputColumn(ii),
                                 table.getInputColumn(attributeColumnIndex),
                                 attribute,
                                 newInputColumn);

        newTable.addInputHeaderAndColumn(table.getInputHeader(ii), std::move(newInputColumn));
    }
}


BuilderImpl::DecisionNodeSP BuilderImpl::BuildNode(const std::string &message, const Table &table)
{
    const BuilderImpl::Header &outputHeader = table.getOutputHeader();
    const BuilderImpl::Column &outputColumn = table.getOutputColumn();
    const BuilderImpl::Headers &inputHeaders = table.getInputHeaders();
    const BuilderImpl::Columns &inputColumns = table.getInputColumns();

    BuilderImpl::ColumnSummary outputColumnSummary;
    BuildColumnSummary(outputColumn, outputColumnSummary);

    if (outputColumnSummary._attributes.size() == 1)
    {
        return BuildNodeEqualOutput(message, outputHeader, outputColumnSummary);
    }

    if (inputColumns.empty())
    {
        return BuildNodeJustOutput(message, outputHeader, outputColumnSummary);
    }

    std::vector<ColumnSummary> inputColumnSumaries;
    inputColumnSumaries.resize(inputColumns.size());
    std::vector<std::thread> threads;
    if (Configuration::Parallelize(inputColumnSumaries.size()))
    {
        for (size_t ii = 0; ii < inputColumnSumaries.size(); ii++)
        {
            threads.emplace_back(std::thread(BuildColumnSummary,
                                             std::cref(inputColumns[ii]),
                                             std::ref(inputColumnSumaries[ii])));
        }

        for (size_t ii = 0; ii < inputColumnSumaries.size(); ii++)
        {
            threads[ii].join();
        }
    }
    else
    {
        for (size_t ii = 0; ii < inputColumnSumaries.size(); ii++)
        {
            BuildColumnSummary(inputColumns[ii], inputColumnSumaries[ii]);
        }
    }

    for (size_t ii = 0; ii < inputColumnSumaries.size(); ii++)
    {
        BuildColumnSummary(inputColumns[ii], inputColumnSumaries[ii]);
    }

    if (CheckInputRowsAreEqual(inputColumnSumaries))
    {
        return BuildNodeEqualOutput(message, outputHeader, outputColumnSummary);
    }


    std::vector<double> conditionalEntropies;
    conditionalEntropies.resize(inputHeaders.size());
    threads.clear();
    if (Configuration::Parallelize(conditionalEntropies.size()))
    {
        for (size_t ii = 0; ii < conditionalEntropies.size(); ii++)
        {
            threads.emplace_back(std::thread(CalculateConditionalEntropy,
                                             std::cref(outputColumn),
                                             std::cref(inputColumns[ii]),
                                             std::cref(inputColumnSumaries[ii]),
                                             std::ref(conditionalEntropies[ii])));
        }

        for (size_t ii = 0; ii < conditionalEntropies.size(); ii++)
        {
            threads[ii].join();
        }
    }
    else
    {
        for (size_t ii = 0; ii < conditionalEntropies.size(); ii++)
        {
            CalculateConditionalEntropy(outputColumn,
                                        inputColumns[ii],
                                        inputColumnSumaries[ii],
                                        conditionalEntropies[ii]);
        }
    }

    double minConditionalEntropy = *conditionalEntropies.begin();
    size_t minPosition = 0;

    for (std::size_t ii = 1; ii < conditionalEntropies.size(); ii++)
    {
        if (conditionalEntropies[ii] < minConditionalEntropy)
        {
            minConditionalEntropy = conditionalEntropies[ii];
            minPosition = ii;
        }
    }

    const auto &selectedAttributeColumn = inputColumns[minPosition];
    const auto &selectedAttributeColumnSummary = inputColumnSumaries[minPosition];
    const auto &selectedAttributeHeader = inputHeaders[minPosition];

    BuilderImpl::DecisionNodeSP node = std::make_shared<BuilderImpl::DecisionNode>(message);

    std::string childMessageHeader;
    if (!message.empty())
    {
        childMessageHeader = message;
        childMessageHeader += ", ";
    }
    childMessageHeader += selectedAttributeHeader;
    childMessageHeader += " = ";

    for (const auto &attribute : selectedAttributeColumnSummary._attributes)
    {
        std::string childMessage = childMessageHeader + attribute.first;
        Table childTable;
        BuilderImpl::BuildTableFromAttribute(table, minPosition, attribute.first, childTable);

        BuilderImpl::DecisionNodeSP child = BuildNode(childMessage, childTable);
        node->addChild(child);
    }

    return node;
}