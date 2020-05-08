#pragma once

#include <string>
#include "Node.hpp"

namespace ast
{

    class Number: public Node
    {
    private:
        std::string mNumber;
        bool mIsFpn;

    public:
        Number( const std::string& number, const bool& fpn );
        virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const override;
        virtual const std::string show() const override;
    };

} // namespace ast