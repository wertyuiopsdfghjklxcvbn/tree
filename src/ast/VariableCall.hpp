#pragma once

#include <string>
#include "Node.hpp"


namespace ast
{

    class VariableCall: public Node
    {
    private:
        std::string mName;

    public:
        VariableCall( const std::string& name );
        virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const override;
        virtual const std::string show() const override;
    };


} // namespace ast