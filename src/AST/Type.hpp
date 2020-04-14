#pragma once

#include <string>
#include "llvm/IR/Value.h"

class Type
{
public:
    virtual const std::string show() const = 0;
    virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock* basicBlock ) const = 0;
    virtual ~Type() = 0;
};
