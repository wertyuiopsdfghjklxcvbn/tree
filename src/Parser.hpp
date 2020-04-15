#pragma once

#include <list>
#include <memory>
#include <optional>
#include <sstream>
#include <stack>

#include "EToken.hpp"
#include "ast/Node.hpp"
#include "ast/VariableDeclaration.hpp"


//"," //"("
//const std::list<const char*> keyWords = { "import", "return", "constant", "class", "structure", "interface", "true", "false", "using" };
//const std::list<const char*> embededTypes = {"Integer", "FPN", "String"};

typedef std::pair<EToken, std::string> TokenType;


class Parser
{
private:
    std::stringstream& mFileBuffer;
    size_t mIndent;
    //std::list<std::unique_ptr<Node>> mParsedAST;
    std::stack<TokenType> mNextTokens;
    TokenType mPreviousToken;
    //    void parseTopLevel();


    std::string parseName();
    std::string parseOperation();
    TokenType parseNumber();
    TokenType getNextToken();
    TokenType peekNextToken();

    ast::VariableDeclaration tokenToVariableDeclaration( const TokenType& token ) const;
    std::unique_ptr<ast::Node> tokenToNode( const TokenType& token ) const;
    bool isOperable( const EToken& token ) const;

    void pushOperatorToOutputStack( const TokenType& token, std::list<std::unique_ptr<ast::Node>>& outNodes ) const;
    void pushFunctionCallToOutputStack( const TokenType& token, std::stack<int>& argsCounters, std::list<std::unique_ptr<ast::Node>>& outNodes ) const;
    bool popOperatorStack( std::stack<TokenType>& operatorStack, std::stack<int>& argsCounters, std::list<std::unique_ptr<ast::Node>>& outNodes ) const;
    std::unique_ptr<ast::Node> parseBinaryExpression( std::unique_ptr<ast::Node> leftHandSide = nullptr );

    bool parseBlock( const size_t& currentBlockIndent, std::list<std::unique_ptr<ast::Node>>& outAST ); //specify function or cycle or basic
    std::unique_ptr<ast::Node> getFunctionDefinition();


public:
    Parser( std::stringstream& fileBuffer );

    bool parseFile( std::list<std::unique_ptr<ast::Node>>& parsedAST );
};
