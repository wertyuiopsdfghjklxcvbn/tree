#pragma once


#include <list>
#include <utility>
#include "Node.hpp"


namespace ast
{

    typedef std::pair<std::unique_ptr<Node>, std::list<std::unique_ptr<Node>>> BlockIf;

    class ConditionalStatementIf: public Node
    {
    private:
        std::list<BlockIf> mListOfStatementsIf;
        std::list<std::unique_ptr<Node>> mElseBody;

    public:
        ConditionalStatementIf( std::list<BlockIf>& listOfStatementsIf, std::list<std::unique_ptr<Node>>& elseBody );
        virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const override;
        virtual const std::string show() const override;
    };

} // namespace ast