#pragma once

#include <memory>
#include <string>
#include "Node.hpp"

namespace ast
{

    class BinaryExpression: public Node
    {
    private:
        std::string mOperation;
        std::unique_ptr<Node> mLeftHandSide;
        std::unique_ptr<Node> mRightHandSide;

    public:
        BinaryExpression( const std::string& operation, std::unique_ptr<Node> leftHandSide, std::unique_ptr<Node> rightHandSide );
        virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const override;
        virtual const std::string show() const override;
    };

} // namespace ast