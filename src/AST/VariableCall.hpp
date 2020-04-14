#pragma once

#include <string>
#include "Type.hpp"


class VariableCall: public Type
{
private:
    std::string mName;

public:
    VariableCall( const std::string& name );
    virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock* basicBlock ) const override;
    virtual const std::string show() const override;
};