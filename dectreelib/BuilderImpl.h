#pragma once

#include "Tree.h"
#include "Table.h"
#include "Builder.h"
#include <iostream>
#include <vector>
#include <map>


class BuilderImpl
{
public:
    using DecisionTree = Builder::DecisionTree;
    using Result = Builder::Result;
    using Header = Table::Header;
    using Headers = Table::Headers;
    using Column = Table::Column;
    using Columns = Table::Columns;

    using DecisionNode = Node<std::string>;
    using DecisionNodeSP = std::shared_ptr<DecisionNode>;

    struct ColumnSummary
    {
        ColumnSummary()
          : _attributes()
          , _total(0)
        {}
        std::map<std::string, std::size_t> _attributes;
        std::size_t _total;
    };


    static void BuildColumnSummary(const BuilderImpl::Column &column, ColumnSummary &columnSummary);


    static void BuildConditionedColumnSummary(const BuilderImpl::Column &outputColumn,
                                              const BuilderImpl::Column &conditionColumn,
                                              const std::string &conditionValue,
                                              ColumnSummary &columnSummary);

    static BuilderImpl::DecisionNodeSP BuildNodeFromMode(const std::string &message,
                                                         const BuilderImpl::Header &header,
                                                         const ColumnSummary &outputColumnSummary);

    static BuilderImpl::DecisionNodeSP BuildNodeEqualOutput(const std::string &message,
                                                            const BuilderImpl::Header &header,
                                                            ColumnSummary &outputColumnSummary);

    static BuilderImpl::DecisionNodeSP BuildNodeJustOutput(const std::string &message,
                                                           const BuilderImpl::Header &header,
                                                           ColumnSummary &outputColumnSummary);

    static bool CheckInputRowsAreEqual(const std::vector<ColumnSummary> inputColumnSumaries);

    static double CalculateEntropy(const BuilderImpl::ColumnSummary &columnSummary);

    static void CalculateConditionalEntropy(const BuilderImpl::Column &outputColumn,
                                            const BuilderImpl::Column &inputColumn,
                                            const ColumnSummary &inputColumnSumary,
                                            double &conditionalEntropy);

    static void BuildColumnFromAttribute(const BuilderImpl::Column &fromColumn,
                                         const BuilderImpl::Column &attributeColumn,
                                         const std::string &attribute,
                                         BuilderImpl::Column &toColumn);

    static void BuildTableFromAttribute(const Table &table,
                                        size_t attributeColumnIndex,
                                        const std::string &attribute,
                                        Table &newTable);

    static void BuildTableFromAttributeParallelized(const Table &table,
                                                    size_t attributeColumnIndex,
                                                    const std::string &attribute,
                                                    Table &newTable);

    static void BuildTableFromAttributeNotParallelized(const Table &table,
                                                       size_t attributeColumnIndex,
                                                       const std::string &attribute,
                                                       Table &newTable);

    static void BuildChildNode(const Table &table,
                               size_t minPosition,
                               const std::string &attribute,
                               const std::string &message,
                               BuilderImpl::DecisionNodeSP &childNode);

    static DecisionNodeSP BuildNode(const std::string &message, const Table &table);
};