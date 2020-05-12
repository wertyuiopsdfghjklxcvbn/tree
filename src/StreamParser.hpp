#pragma once

#include <optional>
#include <sstream>
#include <stack>

#include "EToken.hpp"


typedef std::pair<EToken, std::string> TokenType;


class StreamParser
{
private:
    std::stringstream& mFileBuffer;
    std::stack<TokenType> mNextTokens;
    std::optional<TokenType> mPreviousToken;

    std::string parseName();
    std::string parseOperation();
    TokenType parseNumber();

public:
    StreamParser( std::stringstream& fileBuffer );
    TokenType getNextToken();
    void unget();
    TokenType peekNextToken();
};
