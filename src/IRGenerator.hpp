#pragma once

#include <list>
#include <memory>
#include "AST/Type.hpp"

class IRGenerator
{
private:
    std::list<std::unique_ptr<Type>>& mParsedAST;
    const std::string mModuleName;

public:
    IRGenerator( std::list<std::unique_ptr<Type>>& parsedAST, const std::string& moduleName );
    bool generate();
};