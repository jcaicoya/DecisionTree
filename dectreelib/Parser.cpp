#include "Parser.h"

#include "Table.h"
#include <fstream>
#include <iostream>
#include <string.h>


Parser::Result Parser::BuildTable(const std::filesystem::path pathToInputFile)
{
    std::ifstream fin(pathToInputFile.c_str(), std::ios::in);
    std::string header;
    std::getline(fin, header);
    Tokens headerTokens = BuildHeaderTokens(header);

    if (headerTokens.size() < 2)
    {
        std::cout << "There are not enought headers (found " << headerTokens.size()
                  << ", required at least 2\n";
        return Parser::Result(Parser::Result::Status::NO_OK, Table());
    }

    const size_t numberOfHeaders = headerTokens.size();
    Table table;
    table.setHeaders(std::move(headerTokens));

    std::string currentLine;
    int lineNumber = 1;
    while (std::getline(fin, currentLine))
    {
        lineNumber++;
        Tokens lineTokens = BuildRowTokens(currentLine);
        const size_t numberOfTokens = lineTokens.size();
        bool insertionParserResult = table.addRow(std::move(lineTokens));
        if (!insertionParserResult)
        {
            std::cout << "Line " << lineNumber << " parse error: there are " << numberOfTokens
                      << " elements and header contains " << numberOfHeaders << '\n';
        }
    }

    fin.close();

    return Parser::Result(Parser::Result::Status::OK, std::move(table));
}


Parser::Tokens Parser::Tokenize(const std::string &line, const std::string &separators)
{
    Parser::Tokens tokens;
    size_t currentPosition = 0;
    while (currentPosition < line.size())
    {
        std::size_t nextNoSeparator = FindNextSeparator<false>(currentPosition, line, separators);
        if (nextNoSeparator < line.size())
        {
            std::size_t nextSeparator = FindNextSeparator<true>(nextNoSeparator + 1, line, separators);
            std::size_t tokenLength = nextSeparator - nextNoSeparator;
            tokens.emplace_back(line.substr(nextNoSeparator, tokenLength));
            currentPosition = nextSeparator + 1;
        }
        else
        {
            currentPosition = line.size();
        }
    }

    return tokens;
}


Parser::Tokens Parser::BuildHeaderTokens(const std::string &headerLine)
{
    const std::string separators = " #,;";
    return Tokenize(headerLine, separators);
}


Parser::Tokens Parser::BuildRowTokens(const std::string &rowLine)
{
    const char separators[] = " ,;";
    return Tokenize(rowLine, separators);
}