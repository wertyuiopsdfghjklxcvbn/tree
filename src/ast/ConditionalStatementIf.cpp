#include <string>
#include <tuple>


#include "../Logging.hpp"
#include "ConditionalStatementIf.hpp"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ValueSymbolTable.h"

namespace ast
{

    ConditionalStatementIf::ConditionalStatementIf( std::list<BlockIf>& listOfStatementsIf, std::unique_ptr<CodeBlock>&& elseBody ):
        mListOfStatementsIf( std::move( listOfStatementsIf ) ),
        mElseBody( std::move( elseBody ) )
    {
    }


    llvm::Value* ConditionalStatementIf::generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const
    {
        ValueSymbolTable valueSymbolTable;
        if ( parentAvailableVariables != nullptr )
        {
            valueSymbolTable = ValueSymbolTable( parentAvailableVariables->begin(), parentAvailableVariables->end() );
        }
        for ( const auto& it : *basicBlock->getValueSymbolTable() )
        {
            valueSymbolTable[it.getKey()] = it.getValue();
        }


        std::list<BlockIf>::const_iterator statementsIfIterator = mListOfStatementsIf.begin();
        llvm::BranchInst* returnIfInst = nullptr;
        llvm::BasicBlock* statementBasicBlock = basicBlock;
        llvm::BasicBlock* mergeBasicBlock = llvm::BasicBlock::Create( module.getContext(), "merge", statementBasicBlock->getParent() );
        llvm::BasicBlock* elseBlock;
        while ( statementsIfIterator != mListOfStatementsIf.end() )
        {
            llvm::Value* condition = statementsIfIterator->first->generate( module, statementBasicBlock, &valueSymbolTable );
            llvm::BasicBlock* ifBlock;
            ifBlock = llvm::BasicBlock::Create( module.getContext(), "if", statementBasicBlock->getParent() );
            llvm::BasicBlock* afterIfBlock = ifBlock;
            if ( condition != nullptr )
            {
                if ( statementsIfIterator->second->generate( module, afterIfBlock, &valueSymbolTable ) == nullptr )
                {
                    printError( "error generating if block" );
                    return nullptr;
                }
            }
            afterIfBlock->getInstList().push_back( llvm::BranchInst::Create( mergeBasicBlock ) );


            if ( statementsIfIterator != --mListOfStatementsIf.end() )
            {
                elseBlock = llvm::BasicBlock::Create( module.getContext(), "genElse", basicBlock->getParent() );
            }
            else
            {
                elseBlock = llvm::BasicBlock::Create( module.getContext(), "else", basicBlock->getParent() );
                llvm::BasicBlock* afterElseBlock = elseBlock;
                if ( mElseBody->generate( module, afterElseBlock, &valueSymbolTable ) == nullptr )
                {
                    printError( "error generating else block" );
                    return nullptr;
                }
                afterElseBlock->getInstList().push_back( llvm::BranchInst::Create( mergeBasicBlock ) );
            }
            returnIfInst = llvm::BranchInst::Create( ifBlock, elseBlock, condition );
            statementBasicBlock->getInstList().push_back( returnIfInst );
            statementBasicBlock = elseBlock;
            ++statementsIfIterator;
        }
        mergeBasicBlock->moveAfter( elseBlock );
        basicBlock = mergeBasicBlock;

        return returnIfInst;
    }


    const std::string ConditionalStatementIf::show() const
    {
        std::string returnValue;
        for ( auto& i : mListOfStatementsIf )
        {
            returnValue += "if " + i.first->show() + " " + i.second->show() + " ";
        }
        returnValue += "else " + mElseBody->show();
        return returnValue;
    }


} // namespace ast