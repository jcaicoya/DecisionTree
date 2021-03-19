#include <gtest/gtest.h>

#include <CommandLine.h>


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
    auto result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", "../resources/dummy.csv", "3"}));
    EXPECT_EQ(CommandLine::Result::Status::OK, result.getStatus());

    result = CommandLine::ProcessParameters(CommandLine::Parameters({"dectree"}));
    EXPECT_EQ(CommandLine::Result::Status::NO_OK, result.getStatus());

    result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", "../resources/dummy.csv"}));
    EXPECT_EQ(CommandLine::Result::Status::NO_OK, result.getStatus());

    result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", "../resources/dummy.csv", "3", "4"}));
    EXPECT_EQ(CommandLine::Result::Status::NO_OK, result.getStatus());

    result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", "../resources/dummy.csv", "3", "-p"}));
    EXPECT_EQ(CommandLine::Result::Status::OK, result.getStatus());
}


TEST(CommandLineSuite, ExistInputFile)
{
    auto result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", "../resources/dummy.csv", "3"}));
    EXPECT_EQ(CommandLine::Result::Status::OK, result.getStatus());

    result = CommandLine::ProcessParameters(CommandLine::Parameters({"dectree", "dummy.csv", "3"}));
    EXPECT_EQ(CommandLine::Result::Status::NO_OK, result.getStatus());
}


TEST(CommandLineSuite, ParsingIntToString)
{
    auto result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", "../resources/dummy.csv", "3"}));
    EXPECT_EQ(CommandLine::Result::Status::OK, result.getStatus());

    result = CommandLine::ProcessParameters(
        CommandLine::Parameters({"dectree", "../resources/dummy.csv", "three"}));
    EXPECT_EQ(CommandLine::Result::Status::NO_OK, result.getStatus());
}