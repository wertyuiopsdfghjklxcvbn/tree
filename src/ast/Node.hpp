#pragma once

#include <string>
#include "llvm/IR/Value.h"


namespace ast
{
    class Node
    {
    public:
        virtual const std::string show() const = 0;
        virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock* basicBlock ) const = 0;
        virtual ~Node() = 0;
    };

} // namespace ast