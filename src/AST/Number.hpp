#pragma once

#include <string>
#include "Type.hpp"

class Number: public Type
{
private:
    std::string mNumber;
    bool mIsFpn;

public:
    Number( const std::string& number, const bool& fpn );
    virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock* basicBlock ) const override;
    virtual const std::string show() const override;
};
