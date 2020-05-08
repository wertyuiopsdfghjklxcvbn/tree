#pragma once

#include "Node.hpp"


namespace ast
{

    class Boolean: public Node
    {
    private:
        bool mValue;

    public:
        Boolean( const bool& value );
        virtual const std::string show() const override;
        virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const override;
    };

} // namespace ast
