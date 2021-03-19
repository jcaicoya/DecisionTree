#include <gtest/gtest.h>

#include <Builder.h>


TEST(BuilderSuite, BuildAndPrint)
{
    Table::Header outputHeader {"Y"};
    Table::Column outputColumn {"1", "1", "1", "1", "1", "1", "1", "0", "0", "0"};
    Table::Header attributeHeader {"X1"};
    Table::Column attributeColumn {"A", "A", "A", "A", "B", "B", "B", "B", "C", "C"};
    Table::Header inputHeader {"X2"};
    Table::Column inputColumn {"X", "Y", "Z", "X", "Y", "Z", "X", "Y", "Z", "X"};
    Table table;
    table.setOutputHeaderAndColumn(outputHeader, std::move(outputColumn));
    table.addInputHeaderAndColumn(attributeHeader, std::move(attributeColumn));
    table.addInputHeaderAndColumn(inputHeader, std::move(inputColumn));

    auto result = Builder::Build(std::move(table));
    ASSERT_EQ(Builder::Result::Status::OK, result.getStatus());
    auto decisionTree = result.getTree();
    std::string output = decisionTree.toString();
    std::string expectedOutput = "X1 = A predict this unique output => Y = 1\n"
                                 "X1 = B, X2 = X predict this unique output => Y = 1\n"
                                 "X1 = B, X2 = Y predict the majority output => Y = 0\n"
                                 "X1 = B, X2 = Z predict this unique output => Y = 1\n"
                                 "X1 = C predict this unique output => Y = 0\n";

    EXPECT_EQ(expectedOutput, output);
}