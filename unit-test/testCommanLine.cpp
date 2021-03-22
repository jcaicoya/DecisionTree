#include <gtest/gtest.h>

#include <CommandLine.h>
#include "PathFinder.h"


TEST(CommandLineSuite, BuildParameters)
{
    constexpr int argc = 3;
    const std::string_view programName = "dectree";
    const std::string_view pathToFile = "../resources/dummy.csv";
    const std::string_view index = "3";

    char *argv[] = {const_cast<char *>(programName.data()),
                    const_cast<char *>(pathToFile.data()),
                    const_cast<char *>(index.data()),
                    NULL};

    auto parameters = CommandLine::BuildParameters(argc, argv);
    EXPECT_EQ(argc, parameters.size());
    EXPECT_EQ("dectree", parameters[0]);
    EXPECT_EQ("../resources/dummy.csv", parameters[1]);
    EXPECT_EQ("3", parameters[2]);
}


TEST(CommandLineSuite, NumberOfParameters)
{
    auto filePath = PathFinder::Find("dummy.csv");
    ASSERT_TRUE(filePath.has_value());

    auto result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", filePath.value().c_str(), "3"}));
    EXPECT_EQ(CommandLine::Result::Status::OK, result.getStatus());

    result = CommandLine::ProcessParameters(CommandLine::Parameters({"dectree"}));
    EXPECT_EQ(CommandLine::Result::Status::NO_OK, result.getStatus());

    result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", filePath.value().c_str()}));
    EXPECT_EQ(CommandLine::Result::Status::NO_OK, result.getStatus());

    result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", filePath.value().c_str(), "3", "4"}));
    EXPECT_EQ(CommandLine::Result::Status::NO_OK, result.getStatus());

    result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", filePath.value().c_str(), "3", "-pi"}));
    EXPECT_EQ(CommandLine::Result::Status::OK, result.getStatus());

    result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", filePath.value().c_str(), "3", "-pa"}));
    EXPECT_EQ(CommandLine::Result::Status::OK, result.getStatus());
}


TEST(CommandLineSuite, ExistInputFile)
{
    auto filePath = PathFinder::Find("dummy.csv");
    ASSERT_TRUE(filePath.has_value());

    auto result = CommandLine::ProcessParameters(CommandLine::Parameters({"dectree", filePath.value().c_str(), "3"}));
    EXPECT_EQ(CommandLine::Result::Status::OK, result.getStatus());

    std::string_view notExistFile("i-am-not-here.txt");
    filePath = PathFinder::Find(std::string(notExistFile));
    ASSERT_FALSE(filePath.has_value());
    result = CommandLine::ProcessParameters(CommandLine::Parameters({"dectree", notExistFile, "3"}));
    EXPECT_EQ(CommandLine::Result::Status::NO_OK, result.getStatus());
}


TEST(CommandLineSuite, ParsingIntToString)
{
    auto filePath = PathFinder::Find("dummy.csv");
    ASSERT_TRUE(filePath.has_value());

    auto result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", filePath->c_str(), "3"}));
    EXPECT_EQ(CommandLine::Result::Status::OK, result.getStatus());

    result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", filePath->c_str(), "three"}));
    EXPECT_EQ(CommandLine::Result::Status::NO_OK, result.getStatus());
}