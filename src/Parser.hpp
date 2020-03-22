#pragma once

#include <list>
#include <memory>
#include <optional>
#include <sstream>
#include "AST/Type.hpp"
#include "EToken.hpp"

//"," //"("
//const std::list<const char*> keyWords = { "import", "return", "constant", "class", "structure", "interface", "true", "false", "using" };
//const std::list<const char*> embededTypes = {"Integer", "FPN", "String"};

typedef std::pair<EToken, std::string> TokenType;


class Parser
{
private:
    std::stringstream& mFileBuffer;
    std::optional<size_t> mIndent;
    std::list<std::unique_ptr<Type>> mParsedAST;
    std::shared_ptr<TokenType> mNextToken;
    //std::list<const char*> types;
    //TODO add imported types ?

    //    void parseIdentifier();
    //    void parseTopLevel();
    //    void parseBlock();

    std::string parseName();
    std::string parseOperation();
    TokenType parseNumber();
    TokenType getNextToken();
    TokenType peekNextToken();
    std::unique_ptr<Type> parsePrimary();
    std::unique_ptr<Type> parseBinaryExpression( std::unique_ptr<Type> leftHandSide, unsigned int precedence );

public:
    Parser( std::stringstream& fileBuffer );

    void parseFile();
};
