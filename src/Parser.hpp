#pragma once

#include <list>
#include <memory>

#include "StreamParser.hpp"
#include "ast/Node.hpp"
#include "ast/VariableDeclaration.hpp"
#include "ast/CodeBlock.hpp"


class Parser
{
private:
    StreamParser mStreamParser;
    size_t mIndent;


    ast::VariableDeclaration tokenToVariableDeclaration( const TokenType& token ) const;
    std::unique_ptr<ast::Node> tokenToNode( const TokenType& token ) const;
    bool isOperable( const EToken& token ) const;

    void pushOperatorToOutputStack( const TokenType& token, std::list<std::unique_ptr<ast::Node>>& outNodes ) const;
    void pushFunctionCallToOutputStack( const TokenType& token, std::stack<int>& argsCounters, std::list<std::unique_ptr<ast::Node>>& outNodes ) const;
    bool popOperatorStack( std::stack<TokenType>& operatorStack, std::stack<int>& argsCounters, std::list<std::unique_ptr<ast::Node>>& outNodes ) const;
    std::unique_ptr<ast::Node> parseBinaryExpression( std::unique_ptr<ast::Node> leftHandSide = nullptr );

    std::unique_ptr<ast::Node> parseKeyWordReturn();

    bool parseBlock( const size_t& currentBlockIndent, std::unique_ptr<ast::CodeBlock>& codeBlock ); //specify function or cycle or basic
    std::unique_ptr<ast::Node> getFunctionDefinition();


public:
    Parser( std::stringstream& fileBuffer );

    bool parseFile( std::unique_ptr<ast::CodeBlock>& codeBlock );
};
