#include "CommandLine.h"
#include <filesystem>
#include <iostream>


CommandLine::Parameters CommandLine::BuildParameters(int argc, char *argv[])
{
    Parameters parameters(argc);
    for (size_t ii = 0; static_cast<std::size_t>(ii) < argc; ii++)
    {
        parameters[ii] = argv[ii];
    }

    return parameters;
}


CommandLine::Result CommandLine::ProcessParameters(CommandLine::Parameters &&inputParameters)
{

    Result noOkResult(Result::Status::NO_OK, std::filesystem::path(), -1, Configuration::ParallelizationMode::None);

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
        PrintParsingToIntError(inputParameters[2]);
        return noOkResult;
    }

    Configuration::ParallelizationMode parallelizationMode = Configuration::ParallelizationMode::None;
    if (NumberOfOptionalParameters == inputParameters.size())
    {
        if ("-pi" == inputParameters[3])
        {
            parallelizationMode = Configuration::ParallelizationMode::Inner;
        }
        else if ("-pa" == inputParameters[3])
        {
            parallelizationMode = Configuration::ParallelizationMode::Complete;
        }
        else
        {
            PrintIncorrectOptionalParameter(inputParameters[3]);
            return noOkResult;
        }
    }

    return Result(Result::Status::OK, std::move(pathToInputFile), outputAttibuteIndex, parallelizationMode);
}


void CommandLine::PrintIncorrectNumberOfParameters(std::size_t numberOfParameters)
{
    std::cout << "Incorrect number of parameters: provided " << numberOfParameters << ", required "
              << NumberOfRequiredParameters << '\n';
    std::cout << "Usage: dectree input-file output-attribute-index [-pi|-pa to parallelize execution in inner or all mode]\n";
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
    std::cout << "Optional parameter has to be -pi or -pa and it is " << optionalParameter << '\n';
}