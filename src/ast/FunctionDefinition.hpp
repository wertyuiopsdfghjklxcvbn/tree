#pragma once

#include <list>
#include <memory>
#include <string>
#include <utility>
#include "CodeBlock.hpp"
#include "Node.hpp"
#include "VariableDeclaration.hpp"


namespace ast
{

    class FunctionDefinition: public Node
    {
    private:
        VariableDeclaration mReturnType;
        std::list<VariableDeclaration> mArguments;
        std::unique_ptr<CodeBlock> mBody;

    public:
        FunctionDefinition( const VariableDeclaration& returnType, const std::list<VariableDeclaration>& arguments, std::unique_ptr<CodeBlock>&& body );
        virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const override;
        virtual const std::string show() const override;
    };

} // namespace ast