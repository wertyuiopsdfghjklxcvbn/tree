#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <string>
#include <xutility>

#include "AST/BinaryExpression.hpp"
#include "AST/Error.hpp"
#include "AST/Number.hpp"
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
    if ( !mNextToken )
    {
        if ( !mFileBuffer.eof() )
        {
            unsigned char nextCharacter = mFileBuffer.peek();
            //std::cout << "\"" << mFileBuffer.peek() << "\" " << mFileBuffer.eof() << "\n";
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
                    return { EToken::name, name };
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
                return { EToken::eol, "eol" };
            }
            else
            {
                return { EToken::error, "nothing was recognized" };
            }
        }
        else
        {
            return { EToken::eof, "eof" };
        }
    }
    else
    {
        TokenType returnValue = *mNextToken;
        mNextToken.reset();
        return returnValue;
    }
}


TokenType Parser::peekNextToken()
{
    if ( !mNextToken )
    {
        mNextToken = std::make_shared<TokenType>( getNextToken() );
    }
    return *mNextToken;
}


std::unique_ptr<Type> Parser::parsePrimary()
{
    TokenType token = getNextToken();
    std::unique_ptr<Type> returnType = nullptr;
    switch ( token.first )
    {
        case EToken::integer_literal:
        {
            returnType = std::make_unique<Number>( token.second, false );
            break;
        }
        case EToken::floating_point_literal:
        {
            returnType = std::make_unique<Number>( token.second, true );
            break;
        }
        case EToken::error:
        {
            returnType = std::make_unique<Error>( token.second );
            break;
        }
        default:
        {
            std::cout << "parsePrimary: unhandled token: " << token.first << "\n";
            returnType = std::make_unique<Error>( "token \"" + token.second + "\" can`t be here" );
            break;
        }
    }
    return returnType;
}

//https://en.wikipedia.org/wiki/Operator-precedence_parser
std::unique_ptr<Type> Parser::parseBinaryExpression( std::unique_ptr<Type> leftHandSide, unsigned int precedence )
{
    TokenType nextToken = peekNextToken();

    unsigned int nextTokenPrecedence = Operators().getBinaryOperatorPrecedence( nextToken.second );
    while ( ( nextToken.first == EToken::binary_operator ) && ( nextTokenPrecedence >= precedence ) )
    {
        TokenType op = getNextToken();
        std::unique_ptr<Type> rightHandSide = parsePrimary();
        nextToken = peekNextToken();
        while ( ( nextToken.first == EToken::binary_operator ) &&
                ( Operators().getBinaryOperatorPrecedence( nextToken.second ) >= Operators().getBinaryOperatorPrecedence( op.second ) ) )
        {
            rightHandSide = parseBinaryExpression( std::move( rightHandSide ), Operators().getBinaryOperatorPrecedence( nextToken.second ) );
            nextToken = peekNextToken();
        }
        leftHandSide = std::make_unique<BinaryExpression>( op.second, std::move( leftHandSide ), std::move( rightHandSide ) );
    }
    return leftHandSide;
}


void Parser::parseFile()
{
    TokenType token;
    do
    {
        token = getNextToken();
        switch ( token.first )
        {
            case EToken::eof:
            {
                std::cout << token.first << "\n";
                break;
            }
            case EToken::eol:
            {
                std::cout << token.first << "\n";
                break;
            }
            case EToken::integer_literal:
            {
                mParsedAST.push_back( parseBinaryExpression( std::make_unique<Number>( token.second, false ), Operators().getMinimalOperatorPrecedence() ) );
                break;
            }
            case EToken::floating_point_literal:
            {
                mParsedAST.push_back( parseBinaryExpression( std::make_unique<Number>( token.second, true ), Operators().getMinimalOperatorPrecedence() ) );
                break;
            }
            case EToken::name:
            {
                TokenType nextToken = peekNextToken();
                switch ( nextToken.first )
                {
                    case EToken::name:
                    {
                        nextToken = getNextToken();
                        mParsedAST.push_back( parseBinaryExpression( std::make_unique<VariableDeclaration>( token.second, nextToken.second ),
                                                                     Operators().getMinimalOperatorPrecedence() ) );
                        break;
                    }
                    case EToken::eol:
                    {
                        //
                        break;
                    }
                    //TODO call
                    default:
                    {
                        std::cout << "unhandled token: " << token.first << "\n";
                        break;
                    }
                }
                break;
            }
            case EToken::error:
            {
                std::cout << token.second << "\n";
                mParsedAST.push_back( std::make_unique<Error>( token.second ) );
                break;
            }
            default:
            {
                std::cout << "unhandled token: " << token.first << "\n";
                break;
            }
        }
    } while ( token.first != EToken::eof && token.first != EToken::error );


    for ( auto iter = mParsedAST.begin(); iter != mParsedAST.end(); iter++ )
    {
        std::cout << iter->get()->show() << "\n";
    }
}
