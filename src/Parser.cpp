#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <string>
#include <xutility>

#include "AST/BinaryExpression.hpp"
#include "AST/Error.hpp"
#include "AST/Number.hpp"
#include "AST/VariableCall.hpp"
#include "AST/VariableDeclaration.hpp"
#include "KeyWords.hpp"
#include "Operators.hpp"
#include "Parser.hpp"



Parser::Parser( std::stringstream& fileBuffer ): mFileBuffer( fileBuffer ) {}


std::string Parser::parseName()
{
    unsigned char character;
    std::string name;
    character = mFileBuffer.get();
    while ( std::isalnum( character ) || character == '_' )
    {
        name += character;
        character = mFileBuffer.get();
    }
    if ( !mFileBuffer.eof() )
    {
        mFileBuffer.unget();
    }
    return name;
}


std::string Parser::parseOperation()
{
    unsigned char character;
    std::string operation;
    character = mFileBuffer.get();
    while ( std::ispunct( character ) )
    {
        operation += character;
        character = mFileBuffer.get();
    }
    if ( !mFileBuffer.eof() )
    {
        mFileBuffer.unget();
    }
    return operation;
}


TokenType Parser::parseNumber()
{
    bool pointFounded = false;
    std::string number;

    unsigned char character = mFileBuffer.get();
    while ( std::isdigit( character ) || character == '.' )
    {
        if ( character == '.' && !pointFounded )
        {
            pointFounded = true;
            if ( !std::isdigit( mFileBuffer.peek() ) )
            {
                return { EToken::error, "incorect floating point type" };
            }
        }

        number += character;
        character = mFileBuffer.get();
    }
    if ( !mFileBuffer.eof() )
    {
        mFileBuffer.unget();
    }
    //TODO check after numbers ?

    if ( pointFounded )
    {
        return { EToken::floating_point_literal, number };
    }
    else
    {
        return { EToken::integer_literal, number };
    }
}


TokenType Parser::getNextToken()
{
    if ( mNextTokens.empty() )
    {
        if ( !mFileBuffer.eof() )
        {
            unsigned char nextCharacter = mFileBuffer.peek();
            if ( mFileBuffer.good() )
            {
                //std::cout << "\"" << mFileBuffer.peek() << "\" " << mFileBuffer.eof() << " " << mFileBuffer.fail() << "\n";
                if ( std::isdigit( nextCharacter ) )
                {
                    return parseNumber();
                }
                else if ( std::isalpha( nextCharacter ) || nextCharacter == '_' )
                {
                    std::string name = parseName();

                    if ( Operators().isBinaryOperator( name ) )
                    {
                        return { EToken::binary_operator, name };
                    }
                    else if ( KeyWords().isKeyWord( name ) )
                    {
                        //if(){}
                    }
                    else
                    {
                        TokenType nextToken = getNextToken();
                        switch ( nextToken.first )
                        {
                            case EToken::name:
                            {
                                TokenType afterNextToken = getNextToken();
                                switch ( afterNextToken.first )
                                {
                                    case EToken::opening_round_bracket:
                                    {
                                        //func proto
                                    }
                                    default:
                                    {
                                        mNextTokens.push( afterNextToken );
                                        return { EToken::variable_declaration, name + ' ' + nextToken.second };
                                        //return { EToken::name, name };
                                    }
                                }
                            }
                            case EToken::opening_round_bracket:
                            {
                                //func call
                                //mNextTokens.push( nextToken );
                                //return { EToken::name, name };
                            }
                            default:
                            {
                                mNextTokens.push( nextToken );
                                return { EToken::name, name };
                            }
                        }
                    }
                }
                else if ( nextCharacter == ' ' )
                {
                    mFileBuffer.get();
                    return getNextToken();
                }
                else if ( nextCharacter == '\"' )
                {
                }
                else if ( nextCharacter == '(' )
                {
                    mFileBuffer.get();
                    return { EToken::opening_round_bracket, "opening_round_bracket" };
                }
                else if ( nextCharacter == ')' )
                {
                    mFileBuffer.get();
                    return { EToken::closing_round_bracket, "closing_round_bracket" };
                }
                else if ( std::ispunct( nextCharacter ) )
                {
                    std::string binaryOperator = parseOperation();
                    if ( Operators().isBinaryOperator( binaryOperator ) )
                    {
                        return { EToken::binary_operator, binaryOperator };
                    }
                    else
                    {
                        return { EToken::error, "incorect binary operator: " + binaryOperator };
                    }
                }
                else if ( nextCharacter == '\n' )
                {

                    mFileBuffer.get();
                    //std::cout << mFileBuffer.fail() << "\n";
                    return { EToken::eol, "eol" };
                }
                else
                {
                    std::cout << "nothing was recognized: " << (int)nextCharacter << " " << nextCharacter << ".\n";
                    return { EToken::error, "nothing was recognized" };
                }
            }
            else if ( mFileBuffer.eof() )
            {
                return { EToken::eof, "eof" };
            }
            else if ( mFileBuffer.fail() )
            {
                return { EToken::error, "stream fail bit" };
            }
            else if ( mFileBuffer.bad() )
            {
                return { EToken::error, "stream bad bit" };
            }
            else
            {
                return { EToken::error, "can`t be reached" };
            }
        }
        else
        {
            return { EToken::eof, "eof" };
        }
    }
    else
    {
        TokenType returnValue = mNextTokens.top();
        mNextTokens.pop();
        return returnValue;
    }
}


TokenType Parser::peekNextToken()
{
    if ( mNextTokens.empty() )
    {
        mNextTokens.push( getNextToken() );
    }
    return mNextTokens.top();
}


std::unique_ptr<Type> Parser::tokenToNode( const TokenType& token )
{
    std::unique_ptr<Type> returnNode;
    switch ( token.first )
    {
        case EToken::integer_literal:
        {
            returnNode = std::make_unique<Number>( token.second, false );
            break;
        }
        case EToken::floating_point_literal:
        {
            returnNode = std::make_unique<Number>( token.second, true );
            break;
        }
        case EToken::name:
        {
            returnNode = std::make_unique<VariableCall>( token.second );
            break;
        }
        default:
        {
            returnNode = std::make_unique<Error>( "error converting token" );
            break;
        }
    }
    return returnNode;
}


bool Parser::isOperable( const EToken& token )
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
        case EToken::kv_true:
        case EToken::kv_false:
        case EToken::name:
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


//https://en.wikipedia.org/wiki/Shunting-yard_algorithm
std::unique_ptr<Type> Parser::parseBinaryExpression( std::unique_ptr<Type> leftHandSide )
{
    std::list<std::unique_ptr<Type>> outNodes;
    //std::list<TokenType> out;
    std::stack<TokenType> operatorStack;
    //out.push_back( leftHandSide );
    if ( leftHandSide )
    {
        outNodes.push_back( std::move( leftHandSide ) );
    }

    TokenType nextToken = peekNextToken();
    while ( isOperable( nextToken.first ) )
    {
        nextToken = getNextToken();
        if ( nextToken.first != EToken::binary_operator && nextToken.first != EToken::opening_round_bracket &&
             nextToken.first != EToken::closing_round_bracket )
        {
            outNodes.push_back( tokenToNode( nextToken ) );
            //out.push_back( nextToken );
        }
        //if(nextToken == EToken::comma){}
        else if ( nextToken.first == EToken::binary_operator )
        {
            while ( !operatorStack.empty() )
            {
                if ( ( operatorStack.top().first == EToken::binary_operator ) &&
                     ( Operators().getBinaryOperatorPrecedence( nextToken.second ) <= Operators().getBinaryOperatorPrecedence( operatorStack.top().second ) ) )
                {
                    std::unique_ptr<Type> rhs = std::move( outNodes.back() );
                    outNodes.pop_back();
                    std::unique_ptr<Type> lhs = std::move( outNodes.back() );
                    outNodes.pop_back();
                    outNodes.push_back( std::make_unique<BinaryExpression>( operatorStack.top().second, std::move( lhs ), std::move( rhs ) ) );

                    //out.push_back( operatorStack.top() );
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
            operatorStack.push( nextToken );
        }
        else if ( nextToken.first == EToken::closing_round_bracket )
        {
            bool pe = false;
            while ( !operatorStack.empty() )
            {
                if ( operatorStack.top().first == EToken::opening_round_bracket )
                {
                    pe = true;
                    break;
                }
                else
                {
                    if ( operatorStack.top().first == EToken::binary_operator )
                    {
                        std::unique_ptr<Type> rhs = std::move( outNodes.back() );
                        outNodes.pop_back();
                        std::unique_ptr<Type> lhs = std::move( outNodes.back() );
                        outNodes.pop_back();
                        outNodes.push_back( std::make_unique<BinaryExpression>( operatorStack.top().second, std::move( lhs ), std::move( rhs ) ) );
                    }

                    //out.push_back( operatorStack.top() );
                    operatorStack.pop();
                }
            }
            if ( pe )
            {
                operatorStack.pop();
            }
            else
            {
                //printf( "Error: parentheses mismatched\n" );
                return std::make_unique<Error>( "no opening bracket" );
            }
        }
        else
        {
            return std::make_unique<Error>( "Unknown token: " + nextToken.second );
        }
        nextToken = peekNextToken();
    }
    while ( !operatorStack.empty() )
    {
        if ( operatorStack.top().first == EToken::opening_round_bracket || operatorStack.top().first == EToken::closing_round_bracket )
        {
            //printf( "Error: parentheses mismatched\n" );
            return std::make_unique<Error>( "no closing bracket" );
        }

        if ( operatorStack.top().first == EToken::binary_operator )
        {
            std::unique_ptr<Type> rhs = std::move( outNodes.back() );
            outNodes.pop_back();
            std::unique_ptr<Type> lhs = std::move( outNodes.back() );
            outNodes.pop_back();
            outNodes.push_back( std::make_unique<BinaryExpression>( operatorStack.top().second, std::move( lhs ), std::move( rhs ) ) );
        }

        //out.push_back( operatorStack.top() );
        operatorStack.pop();
    }

    /*for ( auto t : out )
    {
        std::cout << t.first << " " << t.second << "\n";
    }
    std::cout << outNodes.size() << "\n";
    for ( size_t i = 0; i < outNodes.size(); ++i )
    {
        std::cout << outNodes.begin()->get()->show() << "\n";
        outNodes.pop_front();
    }*/
    if ( outNodes.size() == 1 )
    {
        return std::move( outNodes.front() );
    }
    else if ( outNodes.size() > 1 )
    {
        return std::make_unique<Error>( "missing operator after: " + outNodes.front()->show() );
    }
    else
    {
        return std::make_unique<Error>( "nothing to return" );
    }
}


void Parser::parseFile()
{
    TokenType token;
    std::unique_ptr<Type> node = nullptr;
    do
    {
        token = peekNextToken();
        switch ( token.first )
        {
            case EToken::eof:
            {
                getNextToken();
                std::cout << token.first << "\n";
                break;
            }
            case EToken::eol:
            {
                getNextToken();
                std::cout << token.first << "\n";
                break;
            }
            case EToken::integer_literal:
            {
                break;
            }
            case EToken::floating_point_literal:
            {
                break;
            }
            case EToken::opening_round_bracket:
            {
                break;
            }
            case EToken::name:
            {
                break;
            }
            case EToken::variable_declaration:
            {
                getNextToken();

                std::string type = token.second.substr( 0, token.second.find( ' ' ) );
                std::string name = token.second.substr( token.second.find( ' ' ) + 1, token.second.size() - 1 );
                //std::cout <<token.second <<"." << type << "." << name << "\n";
                node = std::make_unique<VariableDeclaration>( type, name );
                break;
            }
            case EToken::error:
            {
                std::cout << token.second << "\n";
                node = std::make_unique<Error>( token.second );
                break;
            }
            default:
            {
                std::cout << "unhandled token: " << token.first << " " << token.second << "\n";
                //error
                break;
            }
        }

        if ( isOperable( token.first ) )
        {
            mParsedAST.push_back( parseBinaryExpression() );
        }
        else if ( token.first == EToken::variable_declaration )
        {
            mParsedAST.push_back( parseBinaryExpression( std::move( node ) ) );
        }
    } while ( token.first != EToken::eof && token.first != EToken::error );


    for ( auto iter = mParsedAST.begin(); iter != mParsedAST.end(); iter++ )
    {
        std::cout << iter->get()->show() << "\n";
    }
}
