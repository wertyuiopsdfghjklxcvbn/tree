#pragma once

#include <map>
#include <string>

#include "llvm/IR/Value.h"


namespace ast
{
    typedef std::map<std::string, llvm::Value*> ValueSymbolTable;

    class Node
    {
    public:
        virtual const std::string show() const = 0;
        virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const = 0;
        virtual ~Node() = 0;
    };

} // namespace ast