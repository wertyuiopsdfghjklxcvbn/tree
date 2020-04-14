#pragma once

#include <list>
#include <memory>
#include <string>
#include "Type.hpp"



class FunctionCall: public Type
{
private:
    std::string mName;
    std::list<std::unique_ptr<Type>> mArguments;

public:
    FunctionCall( const std::string& name, std::list<std::unique_ptr<Type>>& arguments );
    virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock* basicBlock ) const override;
    virtual const std::string show() const override;
};
