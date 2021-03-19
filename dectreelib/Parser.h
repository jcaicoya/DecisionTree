#pragma once

#include "Table.h"
#include <string>
#include <filesystem>


class Parser
{
public:
    class Result
    {
    public:
        enum class Status { OK, NO_OK };

        Result(Result::Status status, Table &&table)
          : _status(status)
          , _table(table)
        {}

        Status getStatus() const { return _status; }
        Table getTable() { return std::move(_table); }

    private:
        const Status _status;
        Table _table;
    };


    static Result BuildTable(const std::filesystem::path pathToInputFile);

private:
    using Token = std::string;
    using Tokens = std::vector<Token>;
    static Tokens BuildHeaderTokens(const std::string &line);
    static Tokens BuildRowTokens(const std::string &line);
    static Tokens Tokenize(char *begin, const char separators[]);

    friend class UnitTestAccessor;
};