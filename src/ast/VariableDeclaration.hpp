#pragma once


#include <string>
#include <utility>
#include "Node.hpp"


namespace ast
{

    class VariableDeclaration: public Node
    {
    private:
        std::string mType;
        std::string mName;
        bool mIsconstant;

    public:
        VariableDeclaration( const std::string& type, const std::string& name, const bool& isconstant = false );
        virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const override;
        virtual const std::string show() const override;

        const std::string& getType() const;
        const std::string& getName() const;
    };

} // namespace ast