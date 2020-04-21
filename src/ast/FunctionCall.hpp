#pragma once

#include <list>
#include <memory>
#include <string>
#include "Node.hpp"


namespace ast
{

    class FunctionCall: public Node
    {
    private:
        std::string mName;
        std::list<std::unique_ptr<Node>> mArguments;

    public:
        FunctionCall( const std::string& name, std::list<std::unique_ptr<Node>>& arguments );
        virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock* basicBlock ) const override;
        virtual const std::string show() const override;
    };

} // namespace ast