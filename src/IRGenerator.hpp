#pragma once

#include <list>
#include <memory>
#include "ast/Node.hpp"

class IRGenerator
{
private:
    std::list<std::unique_ptr<ast::Node>>& mParsedAST;
    const std::string mModuleName;

public:
    IRGenerator( std::list<std::unique_ptr<ast::Node>>& parsedAST, const std::string& moduleName );
    bool generate();
};