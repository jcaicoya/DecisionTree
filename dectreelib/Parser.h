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
    static Tokens Tokenize(const std::string &line, const std::string &separators);

    template <bool NextSeparator>
    inline static size_t FindNextSeparator(size_t fromPosition, const std::string &line, const std::string &separators);
    
    friend class UnitTestAccessor;
};


template <bool NextSeparator>
inline size_t Parser::FindNextSeparator(size_t fromPosition, const std::string &line, const std::string &separators)
{
    bool found = false;
    size_t nextPosition = fromPosition;
    while (!found && nextPosition < line.size())
    {
        bool currentCharIsASeparator = (std::string::npos != separators.find(line[nextPosition]));
        if ((NextSeparator && currentCharIsASeparator) ||
            (!NextSeparator && ! currentCharIsASeparator))
        {
            found = true;
        }
        else
        {
            nextPosition++;
        }
    }

    return nextPosition;
}