#pragma once

#include <list>
#include "Node.hpp"


namespace ast
{
    class CodeBlock: public Node
    {
    private:
        std::list<std::unique_ptr<Node>> mBody;

    public:
        CodeBlock() = default;
        CodeBlock( std::list<std::unique_ptr<Node>>& body );
        virtual const std::string show() const;
        virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const;
    };

} // namespace ast