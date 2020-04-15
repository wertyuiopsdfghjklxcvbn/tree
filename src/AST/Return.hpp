#pragma once

#include <memory>

#include "Node.hpp"

namespace ast
{

    class Return: public Node
    {
    private:
        std::unique_ptr<Node> mReturnValue;

    public:
        Return( std::unique_ptr<Node> returnValue = nullptr );
        virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock* basicBlock ) const override;
        virtual const std::string show() const override;
    };

} // namespace ast