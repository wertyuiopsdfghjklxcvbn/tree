#pragma once

#include <list>
#include <memory>
#include <optional>
#include <sstream>
#include <stack>
#include "AST/Type.hpp"
#include "AST/VariableDeclaration.hpp"
#include "EToken.hpp"

//"," //"("
//const std::list<const char*> keyWords = { "import", "return", "constant", "class", "structure", "interface", "true", "false", "using" };
//const std::list<const char*> embededTypes = {"Integer", "FPN", "String"};

typedef std::pair<EToken, std::string> TokenType;


class Parser
{
private:
    std::stringstream& mFileBuffer;
    size_t mIndent;
    std::list<std::unique_ptr<Type>> mParsedAST;
    std::stack<TokenType> mNextTokens;
    TokenType mPreviousToken;
    //    void parseTopLevel();
    

    std::string parseName();
    std::string parseOperation();
    TokenType parseNumber();
    TokenType getNextToken();
    TokenType peekNextToken();

    VariableDeclaration tokenToVariableDeclaration( const TokenType& token ) const;
    std::unique_ptr<Type> tokenToNode( const TokenType& token ) const;
    bool isOperable( const EToken& token ) const;

    void pushOperatorToOutputStack( const TokenType& token, std::list<std::unique_ptr<Type>>& outNodes ) const;
    void pushFunctionCallToOutputStack( const TokenType& token, std::stack<int>& argsCounters, std::list<std::unique_ptr<Type>>& outNodes ) const;
    bool popOperatorStack( std::stack<TokenType>& operatorStack, std::stack<int>& argsCounters, std::list<std::unique_ptr<Type>>& outNodes ) const;
    std::unique_ptr<Type> parseBinaryExpression( std::unique_ptr<Type> leftHandSide = nullptr );

    bool parseBlock( const size_t& currentBlockIndent, std::list<std::unique_ptr<Type>>& outAST );//specify function or cycle or basic
    std::unique_ptr<Type> getFunctionDefinition();

    
public:
    Parser( std::stringstream& fileBuffer );

    void parseFile();
};
