#pragma once

#include <list>
#include <memory>

#include "ast/CodeBlock.hpp"

class IRGenerator
{
private:
    std::unique_ptr<ast::CodeBlock> mParsedAST;
    const std::string mModuleName;

public:
    IRGenerator( std::unique_ptr<ast::CodeBlock>& parsedAST, const std::string& moduleName );
    bool generate();
};