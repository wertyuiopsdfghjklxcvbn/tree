#include "StreamParser.hpp"
#include "KeyWords.hpp"
#include "Logging.hpp"
#include "Operators.hpp"


StreamParser::StreamParser( std::stringstream& fileBuffer ): mFileBuffer( fileBuffer ), mNextTokens(), mPreviousToken() {}


std::string StreamParser::parseName()
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


std::string StreamParser::parseOperation()
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


TokenType StreamParser::parseNumber()
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


TokenType StreamParser::getNextToken()
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


void StreamParser::unget()
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


TokenType StreamParser::peekNextToken()
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