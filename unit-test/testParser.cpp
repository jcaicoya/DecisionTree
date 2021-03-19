#include <gtest/gtest.h>

#include <Parser.h>
#include "UnitTestAccessor.h"


TEST(Suite_Parser, BuildTable)
{
    const std::filesystem::path pathToInputFile("../resources/dummy.csv");
    Parser::Result result = Parser::BuildTable(pathToInputFile);
    EXPECT_EQ(Parser::Result::Status::OK, result.getStatus());
    auto table = result.getTable();
    EXPECT_EQ(10, table.getNumberOfInputHeaders());
    EXPECT_EQ(5, table.getNumberOfRows());

    std::size_t outputAttributeIndex = 10;
    bool setOutputAttributeIndexResult = table.setOutputAttributeIndex(outputAttributeIndex);
    EXPECT_FALSE(setOutputAttributeIndexResult);

    outputAttributeIndex = 5;
    std::string outputHeader = table.getInputHeader(outputAttributeIndex);
    setOutputAttributeIndexResult = table.setOutputAttributeIndex(outputAttributeIndex);
    EXPECT_TRUE(setOutputAttributeIndexResult);
    EXPECT_EQ(9, table.getNumberOfInputHeaders());
    EXPECT_EQ(5, table.getNumberOfRows());
    EXPECT_EQ(outputHeader, table.getOutputHeader());
}


TEST(Suite_Parser, BuildHeaderTokens)
{
    std::string headerLine =
        "# Age;Workclass;Education;Marital-status;Occupation;Relationship;Race;Sex;Native-country;Income";

    UnitTestAccessor::ParserTokens tokens = UnitTestAccessor::BuildHeaderTokens(headerLine);
    ASSERT_EQ(10, tokens.size());
    EXPECT_EQ("Age", tokens[0]);
    EXPECT_EQ("Workclass", tokens[1]);
    EXPECT_EQ("Education", tokens[2]);
    EXPECT_EQ("Marital-status", tokens[3]);
    EXPECT_EQ("Occupation", tokens[4]);
    EXPECT_EQ("Relationship", tokens[5]);
    EXPECT_EQ("Race", tokens[6]);
    EXPECT_EQ("Sex", tokens[7]);
    EXPECT_EQ("Native-country", tokens[8]);
    EXPECT_EQ("Income", tokens[9]);
}


TEST(Suite_Parser, BuildRowTokens)
{
    std::string rowLine =
        "Middle;State-gov;Bachelors;Never-married;Adm-clerical;Not-in-family;White;Male;United-States;<=50K";

    UnitTestAccessor::ParserTokens tokens = UnitTestAccessor::BuildRowTokens(rowLine);
    ASSERT_EQ(10, tokens.size());
    EXPECT_EQ("Middle", tokens[0]);
    EXPECT_EQ("State-gov", tokens[1]);
    EXPECT_EQ("Bachelors", tokens[2]);
    EXPECT_EQ("Never-married", tokens[3]);
    EXPECT_EQ("Adm-clerical", tokens[4]);
    EXPECT_EQ("Not-in-family", tokens[5]);
    EXPECT_EQ("White", tokens[6]);
    EXPECT_EQ("Male", tokens[7]);
    EXPECT_EQ("United-States", tokens[8]);
    EXPECT_EQ("<=50K", tokens[9]);
}


TEST(Suite_Parser, Tokenizer)
{
    std::string line = "one;two,three";
    char *begin = const_cast<char *>(line.c_str());
    const char separators[] = ";,";

    UnitTestAccessor::ParserTokens tokens = UnitTestAccessor::Tokenize(begin, separators);
    ASSERT_EQ(3, tokens.size());
    EXPECT_EQ("one", tokens[0]);
    EXPECT_EQ("two", tokens[1]);
    EXPECT_EQ("three", tokens[2]);
}