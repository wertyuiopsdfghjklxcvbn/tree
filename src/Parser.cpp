#include <algorithm>
#include <cctype>
#include <memory>
#include <optional>
#include <string>

#include "KeyWords.hpp"
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



Parser::Parser( std::stringstream& fileBuffer ): mFileBuffer( fileBuffer ), mIndent( 0 ), mNextTokens(), mPreviousToken() {}


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
    TokenType returnToken = { EToken::error, "default value" };
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
                    returnToken = parseNumber();
                }
                else if ( std::isalpha( nextCharacter ) || nextCharacter == '_' )
                {
                    std::string name = parseName();

                    if ( Operators().isBinaryOperator( name ) )
                    {
                        returnToken = { EToken::binary_operator, name };
                    }
                    else if ( KeyWords().isKeyWord( name ) )
                    {
                        returnToken = { KeyWords().getEToken( name ), name };
                    }
                    else
                    {
                        mPreviousToken = { EToken::name, name };
                        TokenType nextToken = getNextToken();
                        switch ( nextToken.first )
                        {
                            case EToken::opening_round_bracket:
                            {
                                mNextTokens.push( nextToken );
                                returnToken = { EToken::call, name };
                                break;
                            }
                            case EToken::call:
                            {
                                returnToken = { EToken::function_definition, name + ' ' + nextToken.second };
                                break;
                            }
                            case EToken::name:
                            {
                                returnToken = { EToken::variable_declaration, name + ' ' + nextToken.second };
                                break;
                            }
                            default:
                            {
                                mNextTokens.push( nextToken );
                                returnToken = { EToken::name, name };
                                break;
                            }
                        }
                    }
                }
                else if ( nextCharacter == ' ' )
                {
                    //printError( tokenToString( mPreviousToken.first ) + " " + mPreviousToken.second );
                    if ( mPreviousToken != std::nullopt )
                    {
                        if ( mPreviousToken->first == EToken::eol )
                        {
                            std::string whiteSpaces;
                            nextCharacter = mFileBuffer.get();
                            while ( nextCharacter == ' ' )
                            {
                                whiteSpaces += nextCharacter;
                                nextCharacter = mFileBuffer.get();
                            }
                            if ( !mFileBuffer.eof() )
                            {
                                mFileBuffer.unget();
                            }
                            returnToken = { EToken::indent, whiteSpaces };
                        }
                        else
                        {
                            mFileBuffer.get();
                            returnToken = getNextToken();
                        }
                    }
                    else
                    {
                        printError( "mPreviousToken equal nullopt" );
                    }
                }
                else if ( nextCharacter == ',' )
                {
                    mFileBuffer.get();
                    returnToken = { EToken::comma, "comma" };
                }
                else if ( nextCharacter == '\"' )
                {
                }
                else if ( nextCharacter == '(' )
                {
                    mFileBuffer.get();
                    returnToken = { EToken::opening_round_bracket, "opening_round_bracket" };
                }
                else if ( nextCharacter == ')' )
                {
                    mFileBuffer.get();
                    returnToken = { EToken::closing_round_bracket, "closing_round_bracket" };
                }
                else if ( std::ispunct( nextCharacter ) )
                {
                    std::string binaryOperator = parseOperation();
                    if ( Operators().isBinaryOperator( binaryOperator ) )
                    {
                        returnToken = { EToken::binary_operator, binaryOperator };
                    }
                    else
                    {
                        returnToken = { EToken::error, "incorect binary operator: " + binaryOperator };
                    }
                }
                else if ( nextCharacter == '\n' )
                {

                    mFileBuffer.get();
                    returnToken = { EToken::eol, "eol" };
                }
                else
                {
                    printError( "nothing was recognized: " + std::to_string( (int)nextCharacter ) + " " + static_cast<char>( nextCharacter ) );
                    returnToken = { EToken::error, "nothing was recognized" };
                }
            }
            else if ( mFileBuffer.eof() )
            {
                returnToken = { EToken::eof, "eof" };
            }
            else if ( mFileBuffer.fail() )
            {
                returnToken = { EToken::error, "stream fail bit" };
            }
            else if ( mFileBuffer.bad() )
            {
                returnToken = { EToken::error, "stream bad bit" };
            }
            else
            {
                returnToken = { EToken::error, "can`t be reached" };
            }
        }
        else
        {
            returnToken = { EToken::eof, "eof" };
        }
    }
    else
    {
        TokenType returnValue = mNextTokens.top();
        mNextTokens.pop();
        returnToken = returnValue;
    }
    mPreviousToken = returnToken;
    //printError( tokenToString( returnToken.first ) + " " + returnToken.second );
    return returnToken;
}


void Parser::unget()
{
    if ( mPreviousToken != std::nullopt )
    {
        printError( tokenToString( mPreviousToken->first ) + " " + mPreviousToken->second );
        mNextTokens.push( mPreviousToken.value() );
        mPreviousToken.reset();
    }
    else
    {
        printError( "warning: empty mPreviousToken" );
    }
}


TokenType Parser::peekNextToken()
{
    if ( mNextTokens.empty() )
    {
        if ( mPreviousToken != std::nullopt )
        {
            TokenType token = mPreviousToken.value();
            mNextTokens.push( getNextToken() );
            mPreviousToken = token;
        }
        else
        {
            mNextTokens.push( getNextToken() );
        }
    }
    return mNextTokens.top();
}


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
    TokenType nextToken = getNextToken();

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
        nextToken = getNextToken();
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



bool Parser::parseBlock( const size_t& parentBlockIndent, std::list<std::unique_ptr<ast::Node>>& outAST )
{
    TokenType token;
    std::unique_ptr<ast::Node> node = nullptr;
    bool isParsed = true;
    bool isFirstMeaningfulLine = true;
    size_t blockIndent = parentBlockIndent + mIndent;
    do
    {
        token = peekNextToken();
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

            getNextToken();
            if ( mIndent == 0 )
            {
                mIndent = token.second.length();
                blockIndent = parentBlockIndent + mIndent;
                printError( "file indent: " + std::to_string( mIndent ) );
            }

            if ( token.second.length() == blockIndent )
            {
                token = peekNextToken();
                printError( "parse block " + std::to_string( parentBlockIndent ) + ": " + tokenToString( token.first ) + " " + token.second );
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
                            std::unique_ptr<ast::Node> tempNode = tokenToNode( getNextToken() );
                            if ( tempNode )
                            {
                                node = parseBinaryExpression( std::move( tempNode ) );
                            }
                            else
                            {
                                node = nullptr;
                            }
                            break;
                        }
                        case EToken::kv_if:
                        {
                            getNextToken();

                            std::list<ast::BlockIf> blockIfList;
                            std::list<std::unique_ptr<ast::Node>> elseBlock;

                            std::unique_ptr<ast::Node> ifExpression = parseBinaryExpression();
                            if ( ifExpression )
                            {
                                std::list<std::unique_ptr<ast::Node>> blockAST;
                                if ( parseBlock( blockIndent, blockAST ) )
                                {
                                    blockIfList.push_back( { std::move( ifExpression ), std::move( blockAST ) } );
                                }
                                else
                                {
                                    printError( "error parsing block if" );
                                    isParsed = false;
                                    node = nullptr;
                                    break;
                                }

                                TokenType nextToken = peekNextToken();
                                while ( nextToken.first == EToken::indent && nextToken.second.length() == blockIndent )
                                {
                                    getNextToken();

                                    nextToken = peekNextToken();

                                    if ( nextToken.first == EToken::kv_elif )
                                    {
                                        getNextToken();
                                        std::unique_ptr<ast::Node> elIfExpression = parseBinaryExpression();
                                        if ( elIfExpression )
                                        {
                                            std::list<std::unique_ptr<ast::Node>> blockAST;
                                            if ( parseBlock( blockIndent, blockAST ) )
                                            {
                                                blockIfList.push_back( { std::move( elIfExpression ), std::move( blockAST ) } );
                                            }
                                            else
                                            {
                                                printError( "error parsing block elif" );
                                                isParsed = false;
                                                node = nullptr;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            printError( "error parsing elif expr" );
                                            isParsed = false;
                                            node = nullptr;
                                            break;
                                        }
                                    }
                                    else if ( nextToken.first == EToken::kv_else )
                                    {
                                        getNextToken();
                                        if ( getNextToken().first == EToken::eol )
                                        {

                                            if ( parseBlock( blockIndent, elseBlock ) )
                                            {
                                                break;
                                            }
                                            else
                                            {
                                                printError( "error parsing block else" );
                                                isParsed = false;
                                                node = nullptr;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            printError( "expected eol" );
                                            isParsed = false;
                                            node = nullptr;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        unget();
                                        break;
                                    }
                                    printError( tokenToString( peekNextToken().first ) + " " + peekNextToken().second );
                                    nextToken = peekNextToken();
                                }
                                printError( tokenToString( peekNextToken().first ) + " " + peekNextToken().second );
                                node = std::make_unique<ast::ConditionalStatementIf>( blockIfList, elseBlock );
                            }
                            else
                            {
                                printError( "error parsing if expr" );
                                isParsed = false;
                                node = nullptr;
                                break;
                            }
                            break;
                        }
                        case EToken::kv_return:
                        {
                            getNextToken();
                            TokenType nextToken = peekNextToken();
                            if ( isOperable( nextToken.first ) )
                            {
                                node = parseBinaryExpression();
                                if ( node )
                                {
                                    node = std::make_unique<ast::Return>( std::move( node ) );
                                }
                                else
                                {
                                    isParsed = false;
                                    node = nullptr;
                                }
                            }
                            else if ( nextToken.first == EToken::eol || nextToken.first == EToken::eof )
                            {
                                node = std::make_unique<ast::Return>();
                                getNextToken();
                            }
                            else
                            {
                                printError( "expected end of line. Current token: " + tokenToString( nextToken.first ) + " " + nextToken.second + "." );
                                node = nullptr;
                            }
                            break;
                        }
                        case EToken::error:
                        {
                            printError( token.second );
                            node = nullptr;
                            isParsed = false;
                            break;
                        }
                        default:
                        {
                            getNextToken();
                            printError( "unhandled token: " + tokenToString( token.first ) + " " + token.second );
                            isParsed = false;
                            break;
                        }
                    }
                }
                if ( node )
                {
                    outAST.push_back( std::move( node ) );
                }
                else
                {
                    break;
                }
            }
            else
            {
                isParsed = false;
                printError( "error: incorrect indent" );
                break;
            }
        }
        else if ( token.first == EToken::eol )
        {
            getNextToken();
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
                isParsed = false;
                printError( "expected indent. Current token " + tokenToString( token.first ) + " " + token.second + "." );
            }
            break;
        }

    } while ( token.first != EToken::eof && token.first != EToken::error && token.first != EToken::kv_return );


    for ( auto iter = outAST.begin(); iter != outAST.end(); iter++ )
    {
        printError( iter->get()->show() );
    }
    return isParsed;
}


std::unique_ptr<ast::Node> Parser::getFunctionDefinition()
{
    std::unique_ptr<ast::Node> returnValue;
    ast::VariableDeclaration functionReturnValue = tokenToVariableDeclaration( getNextToken() );
    //actually already checked in getNextToken
    if ( getNextToken().first == EToken::opening_round_bracket )
    {
        std::list<ast::VariableDeclaration> arguments;
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
        nextToken = getNextToken();
        if ( nextToken.first == EToken::eol )
        {
            std::list<std::unique_ptr<ast::Node>> body;
            parseBlock( 0, body );
            returnValue = std::make_unique<ast::FunctionDefinition>( functionReturnValue, arguments, body );
        }
        else if ( nextToken.first == EToken::eof )
        {
            std::list<std::unique_ptr<ast::Node>> body;
            returnValue = std::make_unique<ast::FunctionDefinition>( functionReturnValue, arguments, body );
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


bool Parser::parseFile( std::list<std::unique_ptr<ast::Node>>& parsedAST )
{
    bool isParsed = true;
    TokenType token;
    std::unique_ptr<ast::Node> node = nullptr;
    do
    {
        token = peekNextToken();
        printError( "parse file: " + tokenToString( token.first ) + " " + token.second );

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
                    std::unique_ptr<ast::Node> tempNode = tokenToNode( getNextToken() );
                    if ( tempNode )
                    {
                        node = parseBinaryExpression( std::move( tempNode ) );
                    }
                    else
                    {
                        node = nullptr;
                    }
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
                    isParsed = false;
                    break;
                }
                default:
                {
                    getNextToken();
                    printError( "parse file unhandled token: " + tokenToString( token.first ) + " " + token.second );
                    isParsed = false;
                    break;
                }
            }
        }
        if ( node )
        {
            parsedAST.push_back( std::move( node ) );
            //mParsedAST.push_back( std::move( node ) );
        }
        else
        {
            break;
        }

    } while ( token.first != EToken::eof && token.first != EToken::error );


    //for ( auto iter = mParsedAST.begin(); iter != mParsedAST.end(); iter++ )
    for ( auto iter = parsedAST.begin(); iter != parsedAST.end(); iter++ )
    {
        printError( iter->get()->show() );
    }
    return isParsed;
}
