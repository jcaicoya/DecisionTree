#include <CommandLine.h>
#include <Parser.h>
#include <Table.h>
#include <Builder.h>
#include <Configuration.h>
#include <iostream>
#include <chrono>


int main(int argc, char *argv[])
{
    auto commandLineResult =
        CommandLine::ProcessParameters(CommandLine::BuildParameters(argc, argv));
    if (CommandLine::Result::Status::NO_OK == commandLineResult.getStatus())
    {
        std::cout << "Error processing command line arguments\n";
        return 0;
    }

    std::filesystem::path pathToInputFile = commandLineResult.getPathToInputFileName();
    Parser::Result parserResult = Parser::BuildTable(pathToInputFile);
    if (Parser::Result::Status::NO_OK == parserResult.getStatus())
    {
        std::cout << "Error parsing " << pathToInputFile << "...\n";
        return 0;
    }

    Table table = parserResult.getTable();
    const size_t numberOfRows = table.getNumberOfRows();
    if (0 == numberOfRows)
    {
        std::cout << "Error there are not parsed data\n";
        return 0;
    }

    const size_t numberOfHeaders = table.getNumberOfInputHeaders();
    const size_t outputAttributeIndex = commandLineResult.getOutputAttributeIndex();

    bool setOutputAttributeResult = table.setOutputAttributeIndex(outputAttributeIndex);
    if (!setOutputAttributeResult)
    {
        std::cout << "Error output-attribute-index is out of range " << outputAttributeIndex
                  << " >= " << numberOfHeaders << '\n';
        return 0;
    }

    Configuration::SetParallelize(commandLineResult.getParallelizationMode());

    auto start = std::chrono::high_resolution_clock::now();
    Builder::Result result = Builder::Build(std::move(table));
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Building tree time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " milliseconds ";
    switch (Configuration::GetParallelizationMode())
    {
        case Configuration::ParallelizationMode::Inner:
            std::cout << " using inner parallelization\n";
            break;
        case Configuration::ParallelizationMode::Complete:
            std::cout << " using complete parallelization\n";
            break;
        default:
            std::cout << " without parallelization\n";
    }


    if (Builder::Result::Status::NO_OK == result.getStatus())
    {
        std::cout << "Error building decision tree \n";
        return 0;
    }

    auto decisionTree = result.getTree();
    decisionTree.print();

    return 0;
}
