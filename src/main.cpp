#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "IRGenerator.hpp"
#include "Parser.hpp"



const std::string fileExtension( ".ma" );


std::filesystem::path deleteLastSeparator( const char* path )
{
    std::filesystem::path returnPath;
    std::string rootPathAsString( path );
    char LastChar = rootPathAsString[rootPathAsString.size() - 1];

    if ( LastChar == '\\' || LastChar == '/' )
    {
        returnPath = std::filesystem::path( rootPathAsString.substr( 0, rootPathAsString.size() - 1 ) );
    }
    else
    {
        returnPath = std::filesystem::path( rootPathAsString );
    }
    return returnPath;
}

bool isDirectoryExist( const std::filesystem::path& directoryPath )
{
    bool returnValue = false;
    std::error_code errorCode;
    if ( std::filesystem::is_directory( directoryPath, errorCode ) )
    {
        returnValue = true;
    }
    else
    {
        std::cout << errorCode << "\n";
        returnValue = false;
    }
    return returnValue;
}

bool isFileExist( const std::filesystem::path& filePath )
{
    bool returnValue = false;
    std::error_code errorCode;
    if ( std::filesystem::is_regular_file( filePath, errorCode ) )
    {
        returnValue = true;
    }
    else
    {
        std::cout << errorCode << "\n";
        returnValue = false;
    }
    return returnValue;
}

std::filesystem::path computeRootFileName( const std::filesystem::path& rootPath )
{
    std::filesystem::path returnPath;
    returnPath = rootPath.stem();
    returnPath += fileExtension;
    return returnPath;
}

std::filesystem::path computeRootFilePath( const std::filesystem::path& rootPath )
{
    return rootPath / computeRootFileName( rootPath );
}

bool readFileBuffer( const std::filesystem::path& rootFilePath, std::stringstream& outBuffer )
{
    bool returnValue = false;
    if ( isFileExist( rootFilePath ) )
    {
        std::ifstream file( rootFilePath );
        if ( file.is_open() )
        {
            outBuffer << file.rdbuf();
            returnValue = true;
        }
        else
        {
            std::cout << "can`t open the file " << rootFilePath << "\n";
            returnValue = false;
        }
        file.close();
    }
    else
    {
        std::cout << "root file not found " << rootFilePath << "\n";
        returnValue = false;
    }
    return returnValue;
}



int main( const int argc, const char* argv[] )
{
    if ( argc > 1 )
    {
        std::filesystem::path rootPath = deleteLastSeparator( argv[1] );

        std::cout << rootPath << "\n";

        if ( isDirectoryExist( rootPath ) )
        {
            std::filesystem::path rootFilePath = computeRootFilePath( rootPath );

            std::cout << rootFilePath << "\n";
            std::stringstream fileBuffer;
            if ( readFileBuffer( rootFilePath, fileBuffer ) )
            {
                std::unique_ptr<ast::CodeBlock> parsedAST;
                Parser parser( fileBuffer );
                if ( parser.parseFile( parsedAST ) )
                {
                    std::cout
                        << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
                    std::string pathAsString = rootFilePath.generic_string();
                    IRGenerator irGenerator( parsedAST, pathAsString );
                    irGenerator.generate();
                }
                else
                {
                    std::cout << "fail to parse\n";
                }

                //parseFile( fileBuffer );
            }
        }
        else
        {
            std::cout << "root directory not found " << rootPath << "\n";
        }
    }
    else
    {
        std::cout << "no arguments\n";
    }
}