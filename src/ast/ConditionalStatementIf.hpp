#pragma once


#include <list>
#include <utility>
#include "CodeBlock.hpp"
#include "Node.hpp"


namespace ast
{

    typedef std::pair<std::unique_ptr<Node>, std::unique_ptr<CodeBlock>> BlockIf;

    class ConditionalStatementIf: public Node
    {
    private:
        std::list<BlockIf> mListOfStatementsIf;
        std::unique_ptr<CodeBlock> mElseBody;

    public:
        ConditionalStatementIf( std::list<BlockIf>& listOfStatementsIf, std::unique_ptr<CodeBlock>&& elseBody );
        virtual llvm::Value* generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const override;
        virtual const std::string show() const override;
    };

} // namespace ast