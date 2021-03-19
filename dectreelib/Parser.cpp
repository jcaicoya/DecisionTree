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


Parser::Tokens Parser::Tokenize(char *begin, const char separators[])
{
    Parser::Tokens tokens;
    const char *token = strtok(begin, separators);
    while (token)
    {
        tokens.emplace_back(token);
        token = strtok(NULL, separators);
    }

    return tokens;
}


Parser::Tokens Parser::BuildHeaderTokens(const std::string &headerLine)
{
    char *begin = const_cast<char *>(headerLine.c_str()) + 2;
    const char separators[] = " ,;";
    return Tokenize(begin, separators);
}


Parser::Tokens Parser::BuildRowTokens(const std::string &rowLine)
{
    char *begin = const_cast<char *>(rowLine.c_str());
    const char separators[] = " ,;";
    return Tokenize(begin, separators);
}