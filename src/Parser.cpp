#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <string>
#include <xutility>

#include "AST/BinaryExpression.hpp"
#include "AST/FunctionCall.hpp"
#include "AST/FunctionDefinition.hpp"
#include "AST/Number.hpp"
#include "AST/VariableCall.hpp"
#include "KeyWords.hpp"
#include "Logging.hpp"
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
            int nextCharacter = mFileBuffer.peek();
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
                            //case EToken::name:
                            //{
                            //    TokenType afterNextToken = getNextToken();
                            //    switch ( afterNextToken.first )
                            //    {
                            //        case EToken::opening_round_bracket:
                            //        {
                            //            printError( "function_definition" );
                            //            mNextTokens.push( afterNextToken );
                            //            return { EToken::function_definition, name + ' ' + nextToken.second };
                            //        }
                            //        default:
                            //        {
                            //            printError( "variable_declaration" );
                            //            mNextTokens.push( afterNextToken );
                            //            return { EToken::variable_declaration, name + ' ' + nextToken.second };
                            //        }
                            //    }
                            //}
                            case EToken::opening_round_bracket:
                            {
                                //printError( "call" );
                                mNextTokens.push( nextToken );
                                return { EToken::call, name };
                            }
                            case EToken::call:
                            {
                                //printError( "function_definition" );
                                //mNextTokens.push( nextToken );
                                return { EToken::function_definition, name + ' ' + nextToken.second };
                            }
                            case EToken::name:
                            {
                                //printError( "variable_declaration" );
                                //mNextTokens.push( nextToken );
                                return { EToken::variable_declaration, name + ' ' + nextToken.second };
                            }
                            default:
                            {
                                //printError( "name" );
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
                else if ( nextCharacter == ',' )
                {
                    mFileBuffer.get();
                    return { EToken::comma, "comma" };
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
                    return { EToken::eol, "eol" };
                }
                else
                {
                    printError( "nothing was recognized: " + std::to_string( (int)nextCharacter ) + " " + static_cast<char>( nextCharacter ) );
                    //std::cout << "nothing was recognized: " << (int)nextCharacter << " " << nextCharacter << ".\n";
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


VariableDeclaration Parser::tokenToVariableDeclaration( const TokenType& token ) const
{
    std::string type = token.second.substr( 0, token.second.find( ' ' ) );
    std::string name = token.second.substr( token.second.find( ' ' ) + 1, token.second.size() - 1 );
    return VariableDeclaration( type, name );
}


std::unique_ptr<Type> Parser::tokenToNode( const TokenType& token ) const
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
        case EToken::variable_declaration:
        {
            returnNode = std::make_unique<VariableDeclaration>( tokenToVariableDeclaration( token ) );
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
        case EToken::kv_true:
        case EToken::kv_false:
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


void Parser::pushOperatorToOutputStack( const TokenType& token, std::list<std::unique_ptr<Type>>& outNodes )
{
    std::unique_ptr<Type> rhs = std::move( outNodes.back() );
    outNodes.pop_back();
    std::unique_ptr<Type> lhs = std::move( outNodes.back() );
    outNodes.pop_back();
    outNodes.push_back( std::make_unique<BinaryExpression>( token.second, std::move( lhs ), std::move( rhs ) ) );
}


void Parser::pushFunctionCallToOutputStack( const TokenType& token, std::stack<int>& argsCounters, std::list<std::unique_ptr<Type>>& outNodes )
{
    //printError( token.second + " " + std::to_string( argsCounters.top() ) );
    std::list<std::unique_ptr<Type>> args;
    for ( size_t i = 0; i < argsCounters.top(); ++i )
    {
        args.push_back( std::move( outNodes.back() ) );
        outNodes.pop_back();
    }
    args.reverse();
    outNodes.push_back( std::make_unique<FunctionCall>( token.second, args ) );
    argsCounters.pop();
}


bool Parser::popOperatorStack( std::stack<TokenType>& operatorStack, std::stack<int>& argsCounters, std::list<std::unique_ptr<Type>>& outNodes )
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
std::unique_ptr<Type> Parser::parseBinaryExpression( std::unique_ptr<Type> leftHandSide )
{
    std::list<std::unique_ptr<Type>> outNodes;
    std::stack<TokenType> operatorStack;

    if ( leftHandSide )
    {
        outNodes.push_back( std::move( leftHandSide ) );
    }

    std::stack<int> argsCounters;
    TokenType nextToken = peekNextToken();

    while ( isOperable( nextToken.first ) )
    {
        printError( tokenToString( nextToken.first ) + " " + nextToken.second );
        nextToken = getNextToken();
        if ( nextToken.first != EToken::binary_operator && nextToken.first != EToken::opening_round_bracket &&
             nextToken.first != EToken::closing_round_bracket && nextToken.first != EToken::call && nextToken.first != EToken::comma )
        {
            std::unique_ptr<Type> tempNode = tokenToNode( nextToken );
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
            if ( operatorStack.top().first != EToken::call )
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
        nextToken = peekNextToken();
    }

    if ( ( nextToken.first != EToken::eol ) && ( nextToken.first != EToken::eof ) )
    {
        printError( "expected end of line" );
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


std::unique_ptr<Type> Parser::getFunctionDefinition()
{
    std::unique_ptr<Type> returnValue;
    VariableDeclaration functionReturnValue = tokenToVariableDeclaration( getNextToken() );
    //actually already checked in getNextToken
    if ( getNextToken().first == EToken::opening_round_bracket )
    {
        std::list<VariableDeclaration> arguments;
        TokenType nextToken = getNextToken();
        while ( nextToken.first != EToken::closing_round_bracket )
        {
            if ( nextToken.first == EToken::variable_declaration )
            {
                arguments.push_back( tokenToVariableDeclaration( nextToken ) );
                if ( peekNextToken().first != EToken::comma && peekNextToken().first != EToken::closing_round_bracket )
                {
                    printError( "unexpected token: " + nextToken.second );
                    returnValue = nullptr;
                    break;
                }
            }
            else if ( nextToken.first == EToken::comma )
            {
                if ( peekNextToken().first != EToken::variable_declaration && peekNextToken().first != EToken::closing_round_bracket )
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
            nextToken = getNextToken();
        }
        if ( nextToken.first == EToken::closing_round_bracket )
        {
            //pase function body
            std::list<std::unique_ptr<Type>> t;
            returnValue = std::make_unique<FunctionDefinition>( functionReturnValue, arguments, t );
        }
    }
    else
    {
        printError( "expected opening round bracket" );
        returnValue = nullptr;
    }
    return returnValue;
}


void Parser::parseFile()
{
    TokenType token;
    std::unique_ptr<Type> node = nullptr;
    do
    {
        token = peekNextToken();
        printError( tokenToString( token.first ) + " " + token.second );
        //std::cout << token.first << " " << token.second << "\n";

        if ( isOperable( token.first ) )
        {
            node = parseBinaryExpression();
        }
        else
        {
            switch ( token.first )
            {
                case EToken::eof:
                {
                    getNextToken();
                    break;
                }
                case EToken::eol:
                {
                    getNextToken();
                    continue;
                    break;
                }
                case EToken::variable_declaration:
                {
                    std::unique_ptr<Type> tempNode = tokenToNode( getNextToken() );
                    if ( tempNode )
                    {
                        node = parseBinaryExpression( std::move( tempNode ) );
                    }
                    else
                    {
                        node = nullptr;
                    }
                    //node = parseBinaryExpression( tokenToNode( getNextToken() ) );
                    break;
                }
                case EToken::function_definition:
                {
                    node = getFunctionDefinition();
                    break;
                }
                case EToken::error:
                {
                    printError( token.second );
                    node = nullptr;
                    break;
                }
                default:
                {
                    getNextToken();
                    printError( "unhandled token: " + tokenToString( token.first ) + " " + token.second );
                    //std::cout << "unhandled token: " << token.first << " " << token.second << "\n";
                    //error
                    break;
                }
            }
        }
        if ( node )
        {
            mParsedAST.push_back( std::move( node ) );
        }
        else
        {
            break;
        }

    } while ( token.first != EToken::eof && token.first != EToken::error );


    for ( auto iter = mParsedAST.begin(); iter != mParsedAST.end(); iter++ )
    {
        printError( iter->get()->show() );
        //std::cout << iter->get()->show() << "\n";
    }
}
