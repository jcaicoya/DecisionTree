#include <gtest/gtest.h>

#include <Table.h>
#include <string>


TEST(Suite_Table, Populate)
{
    const std::string header0 = "Header0";
    const std::string header1 = "Header1";
    const std::string header2 = "Header2";
    Table::Headers headers {header0, header1, header2};
    Table table;
    table.setHeaders(std::move(headers));
    ASSERT_EQ(headers.size(), table.getNumberOfInputHeaders());

    const std::string elementZeroZero = "zero-zero";
    const std::string elementZeroOne = "zero-one";
    const std::string elementZeroTwo = "zero-two";
    Table::Row firstRow = {elementZeroZero, elementZeroOne, elementZeroTwo};
    bool insertResult = table.addRow(std::move(firstRow));
    EXPECT_TRUE(insertResult);

    const std::string elementOneZero = "one-zero";
    const std::string elementOneOne = "one-one";
    const std::string elementOneTwo = "one-two";
    Table::Row secondRow = {elementOneZero, elementOneOne, elementOneTwo};
    insertResult = table.addRow(std::move(secondRow));
    EXPECT_TRUE(insertResult);

    const std::string elementTwoZero = "two-zero";
    const std::string elementTwoOne = "two-one";
    const std::string elementTwoTwo = "two-two";
    Table::Row thirdRow = {elementTwoZero, elementTwoOne, elementTwoTwo};
    insertResult = table.addRow(std::move(thirdRow));
    EXPECT_TRUE(insertResult);

    const std::string elementThreeZero = "three-zero";
    const std::string elementThreeOne = "three-one";
    const std::string elementThreeTwo = "three-two";
    Table::Row fourthRow = {elementThreeZero, elementThreeOne, elementThreeTwo};
    insertResult = table.addRow(std::move(fourthRow));
    EXPECT_TRUE(insertResult);

    Table::Row failRow = {"value", "value"};
    insertResult = table.addRow(std::move(failRow));
    EXPECT_FALSE(insertResult);

    constexpr int numberOfRows = 4;
    ASSERT_EQ(numberOfRows, table.getNumberOfRows());

    EXPECT_EQ(header0, table.getInputHeader(0));
    EXPECT_EQ(header1, table.getInputHeader(1));
    EXPECT_EQ(header2, table.getInputHeader(2));

    const auto &columnZero = table.getInputColumn(0);
    EXPECT_EQ(elementZeroZero, columnZero[0]);
    EXPECT_EQ(elementOneZero, columnZero[1]);
    EXPECT_EQ(elementTwoZero, columnZero[2]);
    EXPECT_EQ(elementThreeZero, columnZero[3]);

    const auto &columnOne = table.getInputColumn(1);
    EXPECT_EQ(elementZeroOne, columnOne[0]);
    EXPECT_EQ(elementOneOne, columnOne[1]);
    EXPECT_EQ(elementTwoOne, columnOne[2]);
    EXPECT_EQ(elementThreeOne, columnOne[3]);

    const auto &columnTwo = table.getInputColumn(2);
    EXPECT_EQ(elementZeroTwo, columnTwo[0]);
    EXPECT_EQ(elementOneTwo, columnTwo[1]);
    EXPECT_EQ(elementTwoTwo, columnTwo[2]);
    EXPECT_EQ(elementThreeTwo, columnTwo[3]);

    table.setOutputAttributeIndex(1);
    EXPECT_EQ(header1, table.getOutputHeader());
    const auto &outputColumn = table.getOutputColumn();
    EXPECT_EQ(elementZeroOne, outputColumn[0]);
    EXPECT_EQ(elementOneOne, outputColumn[1]);
    EXPECT_EQ(elementTwoOne, outputColumn[2]);
    EXPECT_EQ(elementThreeOne, outputColumn[3]);

    const auto &inputColumnZero = table.getInputColumn(0);
    EXPECT_EQ(elementZeroZero, inputColumnZero[0]);
    EXPECT_EQ(elementOneZero, inputColumnZero[1]);
    EXPECT_EQ(elementTwoZero, inputColumnZero[2]);
    EXPECT_EQ(elementThreeZero, inputColumnZero[3]);

    const auto &inputColumnOne = table.getInputColumn(1);
    EXPECT_EQ(elementZeroTwo, inputColumnOne[0]);
    EXPECT_EQ(elementOneTwo, inputColumnOne[1]);
    EXPECT_EQ(elementTwoTwo, inputColumnOne[2]);
    EXPECT_EQ(elementThreeTwo, inputColumnOne[3]);
}