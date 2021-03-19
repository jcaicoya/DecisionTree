#pragma once

#include <Parser.h>


class UnitTestAccessor
{
public:
    using ParserToken = Parser::Token;
    using ParserTokens = Parser::Tokens;
    static ParserTokens BuildHeaderTokens(const std::string &line) { return Parser::BuildHeaderTokens(line); }
    static ParserTokens BuildRowTokens(const std::string &line) { return Parser::BuildRowTokens(line); }
    static ParserTokens Tokenize(char *begin, const char separators[]) { return Parser::Tokenize(begin, separators); }
};