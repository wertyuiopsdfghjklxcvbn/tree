#pragma once

#include <list>
#include <memory>
#include <utility>

#include "ast/CodeBlock.hpp"

class IRGenerator
{
private:
    std::unique_ptr<ast::CodeBlock> mParsedAST;
    const std::string mModuleName;

public:
    IRGenerator( std::unique_ptr<ast::CodeBlock>& parsedAST, const std::string& moduleName );
    std::pair<std::unique_ptr<llvm::Module>, std::unique_ptr<llvm::LLVMContext>> generate();
};