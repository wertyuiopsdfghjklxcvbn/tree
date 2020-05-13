#pragma once

#include "CodeBlock.hpp"
#include "Node.hpp"


namespace ast
{

    class CycleWhile: public Node
    {
    private:
        std::unique_ptr<Node> mCondition;
        std::unique_ptr<CodeBlock> mBody;

    public:
        CycleWhile( std::unique_ptr<Node> condition, std::unique_ptr<CodeBlock> body );
        virtual const std::string show() const;
        virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const;
    };


} // namespace ast