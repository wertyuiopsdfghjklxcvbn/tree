#include <algorithm>
#include <cctype>
#include <memory>
#include <optional>
#include <string>

#include "Logging.hpp"
#include "Operators.hpp"
#include "Parser.hpp"
#include "ast/BinaryExpression.hpp"
#include "ast/Boolean.hpp"
#include "ast/ConditionalStatementIf.hpp"
#include "ast/FunctionCall.hpp"
#include "ast/FunctionDefinition.hpp"
#include "ast/Number.hpp"
#include "ast/Return.hpp"
#include "ast/VariableCall.hpp"



Parser::Parser( std::stringstream& fileBuffer ): mStreamParser( fileBuffer ), mIndent( 0 ) {}



ast::VariableDeclaration Parser::tokenToVariableDeclaration( const TokenType& token ) const
{
    std::string type = token.second.substr( 0, token.second.find( ' ' ) );
    std::string name = token.second.substr( token.second.find( ' ' ) + 1, token.second.size() - 1 );
    return ast::VariableDeclaration( type, name );
}


std::unique_ptr<ast::Node> Parser::tokenToNode( const TokenType& token ) const
{
    std::unique_ptr<ast::Node> returnNode;
    switch ( token.first )
    {
        case EToken::integer_literal:
        {
            returnNode = std::make_unique<ast::Number>( token.second, false );
            break;
        }
        case EToken::floating_point_literal:
        {
            returnNode = std::make_unique<ast::Number>( token.second, true );
            break;
        }
        case EToken::name:
        {
            returnNode = std::make_unique<ast::VariableCall>( token.second );
            break;
        }
        case EToken::variable_declaration:
        {
            returnNode = std::make_unique<ast::VariableDeclaration>( tokenToVariableDeclaration( token ) );
            break;
        }
        case EToken::boolean_literal:
        {

            if ( token.second == "true" )
            {
                returnNode = std::make_unique<ast::Boolean>( true );
            }
            else
            {
                returnNode = std::make_unique<ast::Boolean>( false );
            }
            break;
        }
        default:
        {
            printError( "error converting token " + tokenToString( token.first ) + " " + token.second );
            returnNode = nullptr;
            break;
        }
    }
    return returnNode;
}


bool Parser::isOperable( const EToken& token ) const
{
    bool returnValue = false;
    switch ( token )
    {
        case EToken::integer_literal:
        case EToken::floating_point_literal:
        case EToken::opening_round_bracket:
        case EToken::closing_round_bracket:
        case EToken::binary_operator:
        case EToken::boolean_literal:
        case EToken::name:
        case EToken::call:
        case EToken::comma:
        {
            returnValue = true;
            break;
        }
        default:
        {
            returnValue = false;
            break;
        }
    }
    return returnValue;
}


void Parser::pushOperatorToOutputStack( const TokenType& token, std::list<std::unique_ptr<ast::Node>>& outNodes ) const
{
    std::unique_ptr<ast::Node> rhs = std::move( outNodes.back() );
    outNodes.pop_back();
    std::unique_ptr<ast::Node> lhs = std::move( outNodes.back() );
    outNodes.pop_back();
    outNodes.push_back( std::make_unique<ast::BinaryExpression>( token.second, std::move( lhs ), std::move( rhs ) ) );
}


void Parser::pushFunctionCallToOutputStack( const TokenType& token, std::stack<int>& argsCounters, std::list<std::unique_ptr<ast::Node>>& outNodes ) const
{
    //printError( token.second + " " + std::to_string( argsCounters.top() ) );
    std::list<std::unique_ptr<ast::Node>> args;
    for ( size_t i = 0; i < argsCounters.top(); ++i )
    {
        args.push_back( std::move( outNodes.back() ) );
        outNodes.pop_back();
    }
    args.reverse();
    outNodes.push_back( std::make_unique<ast::FunctionCall>( token.second, args ) );
    argsCounters.pop();
}


bool Parser::popOperatorStack( std::stack<TokenType>& operatorStack, std::stack<int>& argsCounters, std::list<std::unique_ptr<ast::Node>>& outNodes ) const
{
    bool pe = false;
    while ( !operatorStack.empty() )
    {
        if ( operatorStack.top().first == EToken::opening_round_bracket )
        {
            if ( argsCounters.size() > 0 && argsCounters.top() == -1 )
            {
                argsCounters.pop();
            }
            pe = true;
            break;
        }
        else
        {
            if ( operatorStack.top().first == EToken::binary_operator )
            {
                pushOperatorToOutputStack( operatorStack.top(), outNodes );
            }
            else if ( operatorStack.top().first == EToken::call )
            {
                pushFunctionCallToOutputStack( operatorStack.top(), argsCounters, outNodes );
            }
            operatorStack.pop();
        }
    }
    return pe;
}


//https://en.wikipedia.org/wiki/Shunting-yard_algorithm
std::unique_ptr<ast::Node> Parser::parseBinaryExpression( std::unique_ptr<ast::Node> leftHandSide )
{
    std::list<std::unique_ptr<ast::Node>> outNodes;
    std::stack<TokenType> operatorStack;

    if ( leftHandSide )
    {
        outNodes.push_back( std::move( leftHandSide ) );
    }

    std::stack<int> argsCounters;
    TokenType nextToken = mStreamParser.getNextToken();

    while ( isOperable( nextToken.first ) )
    {
        //printError( tokenToString( nextToken.first ) + " " + nextToken.second );
        //nextToken = getNextToken();
        if ( nextToken.first != EToken::binary_operator && nextToken.first != EToken::opening_round_bracket &&
             nextToken.first != EToken::closing_round_bracket && nextToken.first != EToken::call && nextToken.first != EToken::comma )
        {
            std::unique_ptr<ast::Node> tempNode = tokenToNode( nextToken );
            if ( tempNode )
            {
                outNodes.push_back( std::move( tempNode ) );
                //count first arg
                if ( argsCounters.size() > 0 && argsCounters.top() == 0 )
                {
                    ++( argsCounters.top() );
                }
            }
            else
            {
                return nullptr; //error converting token
            }
        }
        else if ( nextToken.first == EToken::call )
        {
            operatorStack.push( nextToken );
        }
        else if ( nextToken.first == EToken::comma )
        {
            if ( argsCounters.size() > 0 && argsCounters.top() != -1 )
            {
                ++( argsCounters.top() );
            }
            if ( !popOperatorStack( operatorStack, argsCounters, outNodes ) )
            {
                printError( "Error : separator or parentheses mismatched\n" );
                return nullptr;
            }
        }
        else if ( nextToken.first == EToken::binary_operator )
        {
            while ( !operatorStack.empty() )
            {
                if ( ( operatorStack.top().first == EToken::binary_operator ) &&
                     ( Operators().getBinaryOperatorPrecedence( nextToken.second ) <= Operators().getBinaryOperatorPrecedence( operatorStack.top().second ) ) )
                {
                    pushOperatorToOutputStack( operatorStack.top(), outNodes );
                    operatorStack.pop();
                }
                else
                {
                    break;
                }
            }
            operatorStack.push( nextToken );
        }
        else if ( nextToken.first == EToken::opening_round_bracket )
        {
            if ( !operatorStack.empty() && operatorStack.top().first != EToken::call )
            {
                //count first arg
                if ( argsCounters.size() > 0 && argsCounters.top() == 0 )
                {
                    ++( argsCounters.top() );
                }
                argsCounters.push( -1 );
            }
            else
            {
                argsCounters.push( 0 );
            }
            operatorStack.push( nextToken );
        }
        else if ( nextToken.first == EToken::closing_round_bracket )
        {
            if ( popOperatorStack( operatorStack, argsCounters, outNodes ) )
            {
                operatorStack.pop();
            }
            else
            {
                printError( "no opening bracket" );
                return nullptr;
            }
        }
        else
        {
            printError( "Unknown token: " + nextToken.second );
            return nullptr;
        }
        nextToken = mStreamParser.getNextToken();
    }

    if ( ( nextToken.first != EToken::eol ) && ( nextToken.first != EToken::eof ) )
    {
        printError( "expected end of line. Current token: " + tokenToString( nextToken.first ) + " " + nextToken.second + "." );
        return nullptr;
    }

    while ( !operatorStack.empty() )
    {
        if ( operatorStack.top().first == EToken::opening_round_bracket || operatorStack.top().first == EToken::closing_round_bracket )
        {
            printError( "no closing bracket" );
            return nullptr;
        }

        if ( operatorStack.top().first == EToken::binary_operator )
        {
            pushOperatorToOutputStack( operatorStack.top(), outNodes );
        }
        else if ( operatorStack.top().first == EToken::call )
        {
            pushFunctionCallToOutputStack( operatorStack.top(), argsCounters, outNodes );
        }
        operatorStack.pop();
    }

    if ( outNodes.size() == 1 )
    {
        return std::move( outNodes.front() );
    }
    else if ( outNodes.size() > 1 )
    {
        printError( "missing operator after: " + outNodes.front()->show() );
        return nullptr;
    }
    else
    {
        printError( "nothing to return" );
        return nullptr;
    }
}


std::unique_ptr<ast::Node> Parser::parseKeyWordReturn()
{
    std::unique_ptr<ast::Node> node;
    //eat return
    mStreamParser.getNextToken();
    TokenType nextToken = mStreamParser.peekNextToken();
    if ( isOperable( nextToken.first ) )
    {
        node = parseBinaryExpression();
        if ( node )
        {
            node = std::make_unique<ast::Return>( std::move( node ) );
        }
    }
    else if ( nextToken.first == EToken::eol || nextToken.first == EToken::eof )
    {
        mStreamParser.getNextToken();
        node = std::make_unique<ast::Return>();
    }
    else
    {
        printError( "expected end of line. Current token: " + tokenToString( nextToken.first ) + " " + nextToken.second + "." );
    }
    return node;
}


bool Parser::parseBlock( const size_t& parentBlockIndent, std::unique_ptr<ast::CodeBlock>& codeBlock )
{
    std::list<std::unique_ptr<ast::Node>> outAST;
    TokenType token;
    std::unique_ptr<ast::Node> node = nullptr;
    bool isParsed = true;
    bool isFirstMeaningfulLine = true;
    size_t blockIndent = parentBlockIndent + mIndent;
    do
    {
        token = mStreamParser.peekNextToken();
        printError( "parse block " + std::to_string( parentBlockIndent ) + ": " + tokenToString( token.first ) + " " + token.second );

        if ( token.first == EToken::indent )
        {
            if ( isFirstMeaningfulLine )
            {
                isFirstMeaningfulLine = false;
            }

            if ( token.second.length() < blockIndent )
            {
                //exit from cycle or basic block
                printError( "exit block body" );
                break;
            }

            mStreamParser.getNextToken();
            if ( mIndent == 0 )
            {
                mIndent = token.second.length();
                blockIndent = parentBlockIndent + mIndent;
                printError( "file indent: " + std::to_string( mIndent ) );
            }

            if ( token.second.length() == blockIndent )
            {
                token = mStreamParser.peekNextToken();
                printError( "parse block " + std::to_string( parentBlockIndent ) + ": " + tokenToString( token.first ) + " " + token.second );
                if ( isOperable( token.first ) )
                {
                    node = parseBinaryExpression();
                }
                else if ( token.first == EToken::eof )
                {
                    mStreamParser.getNextToken();
                    break;
                }
                else if ( token.first == EToken::eol )
                {
                    mStreamParser.getNextToken();
                    continue;
                }
                else if ( token.first == EToken::variable_declaration )
                {
                    std::unique_ptr<ast::Node> tempNode = tokenToNode( mStreamParser.getNextToken() );
                    if ( tempNode )
                    {
                        node = parseBinaryExpression( std::move( tempNode ) );
                    }
                }
                else if ( token.first == EToken::kv_if )
                {
                    mStreamParser.getNextToken();

                    std::list<ast::BlockIf> blockIfList;
                    std::unique_ptr<ast::CodeBlock> elseBlock;

                    std::unique_ptr<ast::Node> ifExpression = parseBinaryExpression();
                    if ( ifExpression )
                    {
                        std::unique_ptr<ast::CodeBlock> blockAST;
                        if ( parseBlock( blockIndent, blockAST ) )
                        {
                            blockIfList.push_back( { std::move( ifExpression ), std::move( blockAST ) } );
                        }
                        else
                        {
                            printError( "error parsing block if" );
                            isParsed = false;
                            break;
                        }

                        TokenType nextToken = mStreamParser.peekNextToken();
                        while ( nextToken.first == EToken::indent && nextToken.second.length() == blockIndent )
                        {
                            mStreamParser.getNextToken();

                            nextToken = mStreamParser.peekNextToken();

                            if ( nextToken.first == EToken::kv_elif )
                            {
                                mStreamParser.getNextToken();
                                std::unique_ptr<ast::Node> elIfExpression = parseBinaryExpression();
                                if ( elIfExpression )
                                {
                                    std::unique_ptr<ast::CodeBlock> blockAST;
                                    if ( parseBlock( blockIndent, blockAST ) )
                                    {
                                        blockIfList.push_back( { std::move( elIfExpression ), std::move( blockAST ) } );
                                    }
                                    else
                                    {
                                        printError( "error parsing block elif" );
                                        isParsed = false;
                                        break;
                                    }
                                }
                                else
                                {
                                    printError( "error parsing elif expr" );
                                    isParsed = false;
                                    break;
                                }
                            }
                            else if ( nextToken.first == EToken::kv_else )
                            {
                                mStreamParser.getNextToken();
                                if ( mStreamParser.getNextToken().first == EToken::eol )
                                {
                                    if ( parseBlock( blockIndent, elseBlock ) )
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        printError( "error parsing block else" );
                                        isParsed = false;
                                        break;
                                    }
                                }
                                else
                                {
                                    printError( "expected eol" );
                                    isParsed = false;
                                    break;
                                }
                            }
                            else
                            {
                                mStreamParser.unget();
                                break;
                            }
                            nextToken = mStreamParser.peekNextToken();
                        }
                        if ( isParsed )
                        {
                            if ( elseBlock == nullptr )
                            {
                                elseBlock = std::make_unique<ast::CodeBlock>();
                            }
                            node = std::make_unique<ast::ConditionalStatementIf>( blockIfList, std::move( elseBlock ) );
                        }
                    }
                    else
                    {
                        printError( "error parsing if expr" );
                        isParsed = false;
                        node = nullptr;
                        break;
                    }
                }
                else if ( token.first == EToken::kv_return )
                {
                    node = parseKeyWordReturn();
                }
                else if ( token.first == EToken::error )
                {
                    printError( token.second );
                    isParsed = false;
                    break;
                }
                else
                {
                    mStreamParser.getNextToken();
                    printError( "unhandled token: " + tokenToString( token.first ) + " " + token.second );
                    isParsed = false;
                    break;
                }


                if ( node != nullptr )
                {
                    outAST.push_back( std::move( node ) );
                    node.reset();
                }
                else
                {
                    printError( "top level parsing error" );
                    isParsed = false;
                    break;
                }
            }
            else
            {
                printError( "error: incorrect indent" );
                isParsed = false;
                break;
            }
        }
        else if ( token.first == EToken::eol )
        {
            mStreamParser.getNextToken();
            continue;
        }
        else
        {
            if ( isFirstMeaningfulLine )
            {
                printError( "empty function body" );
            }
            else
            {
                printError( "expected indent. Current token " + tokenToString( token.first ) + " " + token.second + "." );
                isParsed = false;
            }
            break;
        }

    } while ( token.first != EToken::eof && token.first != EToken::error && token.first != EToken::kv_return && isParsed );


    codeBlock = std::make_unique<ast::CodeBlock>( outAST );
    printError( codeBlock->show() );

    return isParsed;
}


std::unique_ptr<ast::Node> Parser::getFunctionDefinition()
{
    std::unique_ptr<ast::Node> returnValue;
    ast::VariableDeclaration functionReturnValue = tokenToVariableDeclaration( mStreamParser.getNextToken() );
    //actually already checked in getNextToken
    if ( mStreamParser.getNextToken().first == EToken::opening_round_bracket )
    {
        std::list<ast::VariableDeclaration> arguments;
        TokenType nextToken = mStreamParser.getNextToken();
        while ( nextToken.first != EToken::closing_round_bracket )
        {
            if ( nextToken.first == EToken::variable_declaration )
            {
                arguments.push_back( tokenToVariableDeclaration( nextToken ) );
                if ( mStreamParser.peekNextToken().first != EToken::comma && mStreamParser.peekNextToken().first != EToken::closing_round_bracket )
                {
                    printError( "unexpected token: " + nextToken.second );
                    returnValue = nullptr;
                    break;
                }
            }
            else if ( nextToken.first == EToken::comma )
            {
                if ( mStreamParser.peekNextToken().first != EToken::variable_declaration &&
                     mStreamParser.peekNextToken().first != EToken::closing_round_bracket )
                {
                    printError( "unexpected token: " + nextToken.second );
                    returnValue = nullptr;
                    break;
                }
            }
            else
            {
                printError( "unexpected token: " + nextToken.second );
                returnValue = nullptr;
                break;
            }
            nextToken = mStreamParser.getNextToken();
        }
        nextToken = mStreamParser.getNextToken();
        if ( nextToken.first == EToken::eol )
        {
            std::unique_ptr<ast::CodeBlock> body;
            if ( parseBlock( 0, body ) )
            {
                returnValue = std::make_unique<ast::FunctionDefinition>( functionReturnValue, arguments, std::move( body ) );
            }
            else
            {
                printError( "error parsing function body" );
                returnValue = nullptr;
            }
        }
        else if ( nextToken.first == EToken::eof )
        {
            returnValue = std::make_unique<ast::FunctionDefinition>( functionReturnValue, arguments, std::make_unique<ast::CodeBlock>() );
        }
        else
        {
            printError( "expected end of line. Current token: " + tokenToString( nextToken.first ) + " " + nextToken.second + "." );
        }
    }
    else
    {
        printError( "expected opening round bracket" );
        returnValue = nullptr;
    }
    return returnValue;
}


bool Parser::parseFile( std::unique_ptr<ast::CodeBlock>& codeBlock )
{
    std::list<std::unique_ptr<ast::Node>> parsedAST;
    bool isParsed = true;
    TokenType token;
    std::unique_ptr<ast::Node> node = nullptr;
    do
    {
        token = mStreamParser.peekNextToken();
        printError( "parse file: " + tokenToString( token.first ) + " " + token.second );

        if ( isOperable( token.first ) )
        {
            node = parseBinaryExpression();
        }
        else if ( token.first == EToken::eof )
        {
            mStreamParser.getNextToken();
            break;
        }
        else if ( token.first == EToken::eol )
        {
            mStreamParser.getNextToken();
            continue;
        }
        else if ( token.first == EToken::variable_declaration )
        {
            std::unique_ptr<ast::Node> tempNode = tokenToNode( mStreamParser.getNextToken() );
            if ( tempNode )
            {
                node = parseBinaryExpression( std::move( tempNode ) );
            }
        }
        else if ( token.first == EToken::function_definition )
        {
            node = getFunctionDefinition();
        }
        else if ( token.first == EToken::error )
        {
            printError( token.second );
            isParsed = false;
        }
        else
        {
            mStreamParser.getNextToken();
            printError( "parse file unhandled token: " + tokenToString( token.first ) + " " + token.second );
            isParsed = false;
        }


        if ( node != nullptr )
        {
            parsedAST.push_back( std::move( node ) );
            node.reset();
        }
        else
        {
            printError( "top level parsing error" );
            isParsed = false;
            break;
        }

    } while ( token.first != EToken::eof && token.first != EToken::error && isParsed );

    codeBlock = std::make_unique<ast::CodeBlock>( parsedAST );

    printError( codeBlock->show() );
    return isParsed;
}
