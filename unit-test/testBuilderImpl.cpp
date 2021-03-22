#include <gtest/gtest.h>

#include <BuilderImpl.h>
#include <Configuration.h>


TEST(BuilderSuiteImpl, BuildColumnSummary)
{
    BuilderImpl::Column column {"x", "y", "z", "x", "y", "z", "x", "y", "z", "x"};
    BuilderImpl::ColumnSummary columnSummary;
    BuilderImpl::BuildColumnSummary(column, columnSummary);

    EXPECT_EQ(column.size(), columnSummary._total);
    EXPECT_EQ(4, columnSummary._attributes["x"]);
    EXPECT_EQ(3, columnSummary._attributes["y"]);
    EXPECT_EQ(3, columnSummary._attributes["z"]);
}


TEST(BuilderSuiteImpl, BuildConditionedColumnSummary)
{
    const BuilderImpl::Column outputColumn {"1", "1", "1", "1", "1", "1", "1", "0", "0", "0"};
    const BuilderImpl::Column conditionColumn {"x", "y", "z", "x", "y", "z", "x", "y", "z", "x"};

    std::string conditionValueX = "x";
    BuilderImpl::ColumnSummary columnSummaryX;
    BuilderImpl::BuildConditionedColumnSummary(outputColumn,
                                               conditionColumn,
                                               conditionValueX,
                                               columnSummaryX);
    EXPECT_EQ(4, columnSummaryX._total);
    EXPECT_EQ(3, columnSummaryX._attributes["1"]);
    EXPECT_EQ(1, columnSummaryX._attributes["0"]);

    std::string conditionValueY = "y";
    BuilderImpl::ColumnSummary columnSummaryY;
    BuilderImpl::BuildConditionedColumnSummary(outputColumn,
                                               conditionColumn,
                                               conditionValueY,
                                               columnSummaryY);
    EXPECT_EQ(3, columnSummaryY._total);
    EXPECT_EQ(2, columnSummaryY._attributes["1"]);
    EXPECT_EQ(1, columnSummaryY._attributes["0"]);


    std::string conditionValueZ = "z";
    BuilderImpl::ColumnSummary columnSummaryZ;
    BuilderImpl::BuildConditionedColumnSummary(outputColumn,
                                               conditionColumn,
                                               conditionValueZ,
                                               columnSummaryZ);
    EXPECT_EQ(3, columnSummaryZ._total);
    EXPECT_EQ(2, columnSummaryZ._attributes["1"]);
    EXPECT_EQ(1, columnSummaryZ._attributes["0"]);
}


TEST(BuilderSuiteImpl, BuildNodeFromMode)
{
    const std::string message = "msg";
    const std::string header = "header";
    const BuilderImpl::Column outputColumn {"1", "1", "1", "1", "1", "1", "1", "0", "0", "0"};
    BuilderImpl::ColumnSummary outputColumnSummary;
    BuilderImpl::BuildColumnSummary(outputColumn, outputColumnSummary);

    BuilderImpl::DecisionNodeSP node = BuilderImpl::BuildNodeFromMode(message, header, outputColumnSummary);
    EXPECT_TRUE(node->isLeaf());
    EXPECT_EQ("msg predict the majority output => header = 1", node->getValue());
}


TEST(BuilderSuiteImpl, BuildNodeEqualOutput)
{
    const std::string message = "msg";
    const std::string header = "header";
    const BuilderImpl::Column outputColumn {"1", "1", "1", "1", "1", "1", "1"};
    BuilderImpl::ColumnSummary outputColumnSummary;
    BuilderImpl::BuildColumnSummary(outputColumn, outputColumnSummary);

    BuilderImpl::DecisionNodeSP node =
        BuilderImpl::BuildNodeEqualOutput(message, header, outputColumnSummary);
    EXPECT_TRUE(node->isLeaf());
    EXPECT_EQ("msg predict this unique output => header = 1", node->getValue());
}


TEST(BuilderSuiteImpl, BuildNodeJustOutput)
{
    const std::string message = "msg";
    const std::string header = "header";
    const BuilderImpl::Column outputMajorityColumn {"1", "1", "0", "0", "2", "2", "2"};
    BuilderImpl::ColumnSummary outputMajorityColumnSummary;
    BuilderImpl::BuildColumnSummary(outputMajorityColumn, outputMajorityColumnSummary);
    BuilderImpl::DecisionNodeSP node =
        BuilderImpl::BuildNodeJustOutput(message, header, outputMajorityColumnSummary);
    EXPECT_TRUE(node->isLeaf());
    EXPECT_EQ("msg predict the majority output => header = 2", node->getValue());

    const BuilderImpl::Column outputUniqueColumn {"1", "1", "1", "1", "1", "1", "1"};
    BuilderImpl::ColumnSummary outputUniqueColumnSummary;
    BuilderImpl::BuildColumnSummary(outputUniqueColumn, outputUniqueColumnSummary);
    node = BuilderImpl::BuildNodeJustOutput(message, header, outputUniqueColumnSummary);
    EXPECT_TRUE(node->isLeaf());
    EXPECT_EQ("msg predict this unique output => header = 1", node->getValue());
}


TEST(BuilderSuiteImpl, CheckInputRowsAreEqual)
{
    std::vector<BuilderImpl::ColumnSummary> inputColumnSummaries;
    bool inputRowsAreEqual = BuilderImpl::CheckInputRowsAreEqual(inputColumnSummaries);
    EXPECT_TRUE(inputRowsAreEqual);

    inputColumnSummaries.resize(3);
    inputRowsAreEqual = BuilderImpl::CheckInputRowsAreEqual(inputColumnSummaries);
    EXPECT_FALSE(inputRowsAreEqual);

    const BuilderImpl::Column inputColumn {"A", "A", "A", "A", "A", "A", "A"};
    BuilderImpl::ColumnSummary inputColumnSummary;
    BuilderImpl::BuildColumnSummary(inputColumn, inputColumnSummary);
    for (auto &columnSummary : inputColumnSummaries)
    {
        columnSummary = inputColumnSummary;
    }
    inputRowsAreEqual = BuilderImpl::CheckInputRowsAreEqual(inputColumnSummaries);
    EXPECT_TRUE(inputRowsAreEqual);

    const BuilderImpl::Column otherInputColumn {"B", "B", "B", "B", "B", "B", "B"};
    BuilderImpl::ColumnSummary otherInputColumnSummary;
    BuilderImpl::BuildColumnSummary(otherInputColumn, otherInputColumnSummary);
    inputColumnSummaries.push_back(otherInputColumnSummary);
    inputRowsAreEqual = BuilderImpl::CheckInputRowsAreEqual(inputColumnSummaries);
    EXPECT_TRUE(inputRowsAreEqual);

    const BuilderImpl::Column differentInputColumn {"A", "A", "A", "B", "B", "B", "B"};
    BuilderImpl::ColumnSummary differentInputColumnSummary;
    BuilderImpl::BuildColumnSummary(differentInputColumn, differentInputColumnSummary);
    inputColumnSummaries.push_back(differentInputColumnSummary);
    inputRowsAreEqual = BuilderImpl::CheckInputRowsAreEqual(inputColumnSummaries);
    EXPECT_FALSE(inputRowsAreEqual);
}


TEST(BuilderSuiteImpl, CalculateEntropy)
{
    const BuilderImpl::Column column1111 {"1", "1", "1", "1"};
    BuilderImpl::ColumnSummary columnSummary1111;
    BuilderImpl::BuildColumnSummary(column1111, columnSummary1111);
    double entropy = BuilderImpl::CalculateEntropy(columnSummary1111);
    EXPECT_DOUBLE_EQ(0.0, entropy);

    const BuilderImpl::Column column1110 {"1", "1", "1", "0"};
    BuilderImpl::ColumnSummary columnSummary1110;
    BuilderImpl::BuildColumnSummary(column1110, columnSummary1110);
    entropy = BuilderImpl::CalculateEntropy(columnSummary1110);
    EXPECT_DOUBLE_EQ(0.81127812445913283, entropy);

    const BuilderImpl::Column column1100 {"1", "1", "0", "0"};
    BuilderImpl::ColumnSummary columnSummary1100;
    BuilderImpl::BuildColumnSummary(column1100, columnSummary1100);
    entropy = BuilderImpl::CalculateEntropy(columnSummary1100);
    EXPECT_DOUBLE_EQ(1.0, entropy);
}


TEST(BuilderSuiteImpl, CalculateConditionalEntropy)
{
    BuilderImpl::Column outputColumn = {"1", "1", "1", "1", "1", "1", "1", "0", "0", "0"};
    BuilderImpl::Column inputColumn = {"A", "A", "A", "A", "B", "B", "B", "B", "C", "C"};
    BuilderImpl::ColumnSummary inputColumnSummary;
    BuilderImpl::BuildColumnSummary(inputColumn, inputColumnSummary);
    double conditionalEntropy = 0.0;
    BuilderImpl::CalculateConditionalEntropy(outputColumn,
                                             inputColumn,
                                             inputColumnSummary,
                                             conditionalEntropy);

    EXPECT_DOUBLE_EQ(0.32451124978365314, conditionalEntropy);
}


TEST(BuilderSuiteImpl, BuildColumnFromAttribute)
{
    BuilderImpl::Column fromColumn {"1", "1", "1", "1", "1", "1", "1", "0", "0", "0"};
    BuilderImpl::Column attributeColumn {"A", "A", "A", "A", "B", "B", "B", "B", "C", "C"};

    std::string attribute = "A";
    BuilderImpl::Column toColumn;
    BuilderImpl::BuildColumnFromAttribute(fromColumn, attributeColumn, attribute, toColumn);
    ASSERT_EQ(4, toColumn.size());
    EXPECT_EQ("1", toColumn[0]);
    EXPECT_EQ("1", toColumn[1]);
    EXPECT_EQ("1", toColumn[2]);
    EXPECT_EQ("1", toColumn[3]);

    attribute = "B";
    BuilderImpl::BuildColumnFromAttribute(fromColumn, attributeColumn, attribute, toColumn);
    ASSERT_EQ(4, toColumn.size());
    EXPECT_EQ("1", toColumn[0]);
    EXPECT_EQ("1", toColumn[1]);
    EXPECT_EQ("1", toColumn[2]);
    EXPECT_EQ("0", toColumn[3]);

    attribute = "C";
    BuilderImpl::BuildColumnFromAttribute(fromColumn, attributeColumn, attribute, toColumn);
    ASSERT_EQ(2, toColumn.size());
    EXPECT_EQ("0", toColumn[0]);
    EXPECT_EQ("0", toColumn[1]);

    attribute = "D";
    BuilderImpl::BuildColumnFromAttribute(fromColumn, attributeColumn, attribute, toColumn);
    ASSERT_TRUE(toColumn.empty());
}


Table buildTable()
{
    BuilderImpl::Header outputHeader {"Y"};
    BuilderImpl::Column outputColumn {"1", "1", "1", "1", "1", "1", "1", "0", "0", "0"};
    BuilderImpl::Header attributeHeader {"X1"};
    BuilderImpl::Column attributeColumn {"A", "A", "A", "A", "B", "B", "B", "B", "C", "C"};
    BuilderImpl::Header inputHeader {"X2"};
    BuilderImpl::Column inputColumn {"X", "Y", "Z", "X", "Y", "Z", "X", "Y", "Z", "X"};
    
    Table table;
    table.setOutputHeaderAndColumn(outputHeader, std::move(outputColumn));
    table.addInputHeaderAndColumn(attributeHeader, std::move(attributeColumn));
    table.addInputHeaderAndColumn(inputHeader, std::move(inputColumn));

    return table;
}


void executeBuildTableFromAttribute()
{
    Table fromTable = buildTable();
    std::string attribute = "A";
    Table toTable;
    BuilderImpl::BuildTableFromAttribute(fromTable, 0, attribute, toTable);
    EXPECT_EQ("Y", toTable.getOutputHeader());
    const auto &toTableOutputColumn = toTable.getOutputColumn();
    ASSERT_EQ(4, toTableOutputColumn.size());
    EXPECT_EQ("1", toTableOutputColumn[0]);
    EXPECT_EQ("1", toTableOutputColumn[1]);
    EXPECT_EQ("1", toTableOutputColumn[2]);
    EXPECT_EQ("1", toTableOutputColumn[3]);

    ASSERT_EQ(1, toTable.getNumberOfInputHeaders());
    const auto &toTableInputHeader = toTable.getInputColumn(0);
    ASSERT_EQ(4, toTableInputHeader.size());
    EXPECT_EQ("X", toTableInputHeader[0]);
    EXPECT_EQ("Y", toTableInputHeader[1]);
    EXPECT_EQ("Z", toTableInputHeader[2]);
    EXPECT_EQ("X", toTableInputHeader[3]);
}


TEST(BuilderSuiteImpl, BuildTableFromAttribute)
{
    Configuration::SetParallelize(Configuration::ParallelizationMode::None);
    executeBuildTableFromAttribute();
    Configuration::SetParallelize(Configuration::ParallelizationMode::Inner);
    executeBuildTableFromAttribute();
    Configuration::SetParallelize(Configuration::ParallelizationMode::Complete);
    executeBuildTableFromAttribute();
}


void executeBuildNode()
{
    Table table = buildTable();

    std::string message {"root"};
    BuilderImpl::DecisionNodeSP rootNode = BuilderImpl::BuildNode(message, table);

    ASSERT_TRUE(!!rootNode);
    EXPECT_EQ(message, rootNode->getValue());
    ASSERT_EQ(3, rootNode->getNumberOfChildren());

    BuilderImpl::DecisionNodeSP X1ANode = rootNode->getNthChild(0);
    ASSERT_TRUE(!!X1ANode);
    EXPECT_EQ("root, X1 = A predict this unique output => Y = 1", X1ANode->getValue());
    EXPECT_TRUE(X1ANode->isLeaf());

    BuilderImpl::DecisionNodeSP X1BNode = rootNode->getNthChild(1);
    ASSERT_TRUE(!!X1BNode);
    EXPECT_EQ("root, X1 = B", X1BNode->getValue());
    EXPECT_EQ(3, X1BNode->getNumberOfChildren());

    BuilderImpl::DecisionNodeSP X1BX2XNode = X1BNode->getNthChild(0);
    ASSERT_TRUE(!!X1BX2XNode);
    EXPECT_EQ("root, X1 = B, X2 = X predict this unique output => Y = 1", X1BX2XNode->getValue());
    EXPECT_TRUE(X1BX2XNode->isLeaf());

    BuilderImpl::DecisionNodeSP X1BX2YNode = X1BNode->getNthChild(1);
    ASSERT_TRUE(!!X1BX2YNode);
    EXPECT_EQ("root, X1 = B, X2 = Y predict the majority output => Y = 0", X1BX2YNode->getValue());
    EXPECT_TRUE(X1BX2YNode->isLeaf());

    BuilderImpl::DecisionNodeSP X1BX2ZNode = X1BNode->getNthChild(2);
    ASSERT_TRUE(!!X1BX2ZNode);
    EXPECT_EQ("root, X1 = B, X2 = Z predict this unique output => Y = 1", X1BX2ZNode->getValue());
    EXPECT_TRUE(X1BX2ZNode->isLeaf());

    BuilderImpl::DecisionNodeSP X1CNode = rootNode->getNthChild(2);
    ASSERT_TRUE(!!X1CNode);
    EXPECT_EQ("root, X1 = C predict this unique output => Y = 0", X1CNode->getValue());
    EXPECT_TRUE(X1CNode->isLeaf());
}


TEST(BuilderSuiteImpl, BuildNode)
{
    Configuration::SetParallelize(Configuration::ParallelizationMode::None);
    executeBuildNode();
    Configuration::SetParallelize(Configuration::ParallelizationMode::Inner);
    executeBuildNode();
    //Configuration::SetParallelize(Configuration::ParallelizationMode::Complete);
    //executeBuildNode();
}