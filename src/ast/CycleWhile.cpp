#include "CycleWhile.hpp"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ValueSymbolTable.h"



namespace ast
{


    CycleWhile::CycleWhile( std::unique_ptr<Node> condition, std::unique_ptr<CodeBlock> body ): mCondition( std::move( condition ) ), mBody( std::move( body ) )
    {
    }


    const std::string CycleWhile::show() const
    {
        return "while " + mCondition->show() + '\n' + mBody->show();
    }


    llvm::Value* CycleWhile::generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const
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


        llvm::BasicBlock* conditionBlock = llvm::BasicBlock::Create( module.getContext(), "conditionBlock", basicBlock->getParent() );
        basicBlock->getInstList().push_back( llvm::BranchInst::Create( conditionBlock ) );
        llvm::Value* condition = mCondition->generate( module, conditionBlock, &valueSymbolTable );
        llvm::BasicBlock* loopBlock = llvm::BasicBlock::Create( module.getContext(), "loopBlock", basicBlock->getParent() );
        llvm::BasicBlock* afterLoopBlock = llvm::BasicBlock::Create( module.getContext(), "afterLoop", basicBlock->getParent() );
        if ( condition != nullptr )
        {
            conditionBlock->getInstList().push_back( llvm::BranchInst::Create( loopBlock, afterLoopBlock, condition ) );

            if ( mBody->generate( module, loopBlock, &valueSymbolTable ) != nullptr )
            {
                loopBlock->getInstList().push_back( llvm::BranchInst::Create( conditionBlock ) );
                afterLoopBlock->moveAfter( loopBlock );
                basicBlock = afterLoopBlock;
            }
            else
            {
                return nullptr;
            }
        }
        else
        {
            return nullptr;
        }

        return afterLoopBlock;
    }


} // namespace ast