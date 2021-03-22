#include "CommandLine.h"
#include <filesystem>
#include <iostream>


CommandLine::Parameters CommandLine::BuildParameters(int argc, char *argv[])
{
    Parameters parameters(argc);
    for (size_t ii = 0; ii < static_cast<size_t>(argc); ii++)
    {
        parameters[ii] = argv[ii];
    }

    return parameters;
}


CommandLine::Result CommandLine::ProcessParameters(CommandLine::Parameters &&inputParameters)
{
    Result noOkResult(Result::Status::NO_OK, std::filesystem::path(), -1, false);

    if (NumberOfRequiredParameters != inputParameters.size() &&
        NumberOfOptionalParameters != inputParameters.size())
    {
        PrintIncorrectNumberOfParameters(inputParameters.size());
        return noOkResult;
    }

    std::filesystem::path pathToInputFile(inputParameters[1]);
    if (!std::filesystem::exists(pathToInputFile))
    {
        PrintFileDoesNotExist(pathToInputFile);
        return noOkResult;
    }

    int outputAttibuteIndex = -1;
    try
    {
        outputAttibuteIndex = std::stoi(inputParameters[2].data());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what();
        PrintParsingToIntError(inputParameters[2]);
        return noOkResult;
    }

    bool parallelize = false;
    if (NumberOfOptionalParameters == inputParameters.size())
    {
        if ("-p" != inputParameters[3])
        {
            PrintIncorrectOptionalParameter(inputParameters[3]);
            return noOkResult;
        }
        else
        {
            parallelize = true;
        }
    }

    return Result(Result::Status::OK, std::move(pathToInputFile), outputAttibuteIndex, parallelize);
}


void CommandLine::PrintIncorrectNumberOfParameters(std::size_t numberOfParameters)
{
    std::cout << "Incorrect number of parameters: provided " << numberOfParameters << ", required "
              << NumberOfRequiredParameters << '\n';
    std::cout << "Usage: dectree input-file output-attribute-index [-p to parallelize execution]\n";
}


void CommandLine::PrintFileDoesNotExist(const std::filesystem::path &pathToInputFile)
{
    std::cout << "File " << pathToInputFile << " does not exist\n";
}


void CommandLine::PrintParsingToIntError(const std::string_view &number)
{
    std::cout << "Parsing error " << number << " to int\n";
}


void CommandLine::PrintIncorrectOptionalParameter(const std::string_view &optionalParameter)
{
    std::cout << "Optional parameter has to be -p and it is " << optionalParameter << '\n';
}