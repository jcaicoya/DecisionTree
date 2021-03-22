#include <gtest/gtest.h>

#include <CommandLine.h>
#include "Common.h"


TEST(CommandLineSuite, BuildParameters)
{
    constexpr int argc = 3;
    const std::string_view programName = "dectree";
    const std::string_view index = "3";

    char *argv[] = {const_cast<char *>(programName.data()),
                    const_cast<char *>(Common::Common::PathToDummyCsvFile.data()),
                    const_cast<char *>(index.data()),
                    NULL};

    auto parameters = CommandLine::BuildParameters(argc, argv);
    EXPECT_EQ(argc, parameters.size());
    EXPECT_EQ("dectree", parameters[0]);
    EXPECT_EQ(Common::PathToDummyCsvFile, parameters[1]);
    EXPECT_EQ("3", parameters[2]);
}


TEST(CommandLineSuite, NumberOfParameters)
{
    auto result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", Common::PathToDummyCsvFile, "3"}));
    EXPECT_EQ(CommandLine::Result::Status::OK, result.getStatus());

    result = CommandLine::ProcessParameters(CommandLine::Parameters({"dectree"}));
    EXPECT_EQ(CommandLine::Result::Status::NO_OK, result.getStatus());

    result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", Common::PathToDummyCsvFile}));
    EXPECT_EQ(CommandLine::Result::Status::NO_OK, result.getStatus());

    result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", Common::PathToDummyCsvFile, "3", "4"}));
    EXPECT_EQ(CommandLine::Result::Status::NO_OK, result.getStatus());

    result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", Common::PathToDummyCsvFile, "3", "-p"}));
    EXPECT_EQ(CommandLine::Result::Status::OK, result.getStatus());
}


TEST(CommandLineSuite, ExistInputFile)
{
    auto result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", Common::PathToDummyCsvFile, "3"}));
    EXPECT_EQ(CommandLine::Result::Status::OK, result.getStatus());

    result = CommandLine::ProcessParameters(CommandLine::Parameters({"dectree", "dummy.csv", "3"}));
    EXPECT_EQ(CommandLine::Result::Status::NO_OK, result.getStatus());
}


TEST(CommandLineSuite, ParsingIntToString)
{
    auto result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", Common::PathToDummyCsvFile, "3"}));
    EXPECT_EQ(CommandLine::Result::Status::OK, result.getStatus());

    result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", Common::PathToDummyCsvFile, "three"}));
    EXPECT_EQ(CommandLine::Result::Status::NO_OK, result.getStatus());
}