#pragma once

#include <memory>

#include "Type.hpp"

class Return: public Type
{
private:
    std::unique_ptr<Type> mReturnValue;

public:
    Return( std::unique_ptr<Type> returnValue = nullptr );
    virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock* basicBlock ) const override;
    virtual const std::string show() const override;
};