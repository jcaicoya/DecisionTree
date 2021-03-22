#pragma once

#include <vector>
#include <string_view>
#include <filesystem>
#include "Configuration.h"


class CommandLine
{
public:
    class Result
    {
    public:
        enum class Status { OK, NO_OK };

        Result(Status status,
               std::filesystem::path &&pathToInputFileName,
               size_t outputAttributeIndex,
               Configuration::ParallelizationMode parallelizationMode)
          : _status(status)
          , _pathToInputFileName(pathToInputFileName)
          , _outputAttibuteIndex(outputAttributeIndex)
          , _parallelizationMode(parallelizationMode)
        {}

        Status getStatus() const { return _status; }
        std::filesystem::path getPathToInputFileName() const { return std::move(_pathToInputFileName); }
        size_t getOutputAttributeIndex() const { return _outputAttibuteIndex; }
        Configuration::ParallelizationMode getParallelizationMode() const { return _parallelizationMode; }

    private:
        Status _status;
        std::filesystem::path _pathToInputFileName;
        size_t _outputAttibuteIndex;
        Configuration::ParallelizationMode _parallelizationMode;
    };

    using Parameters = std::vector<std::string_view>;
    static Parameters BuildParameters(int argc, char *argv[]);
    static Result ProcessParameters(Parameters &&inputParameters);

private:
    static void PrintIncorrectNumberOfParameters(std::size_t numberOfParameters);
    static void PrintFileDoesNotExist(const std::filesystem::path &pathToInputFile);
    static void PrintParsingToIntError(const std::string_view &number);
    static void PrintIncorrectOptionalParameter(const std::string_view &optionalParameter);


    constexpr static std::size_t NumberOfRequiredParameters = 3;
    constexpr static std::size_t NumberOfOptionalParameters = 4;
};