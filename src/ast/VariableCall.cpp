#include "VariableCall.hpp"
#include "../Logging.hpp"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ValueSymbolTable.h"



namespace ast
{

    VariableCall::VariableCall( const std::string& name ): mName( name ) {}


    llvm::Value* VariableCall::generate( llvm::Module& module, llvm::BasicBlock* basicBlock ) const
    {
        if ( basicBlock != nullptr )
        {
            llvm::Function* function = basicBlock->getParent();
            if ( function != nullptr )
            {
                llvm::Function::arg_iterator functionIt = function->arg_begin();
                while ( functionIt != function->arg_end() )
                {
                    if ( functionIt->getName() == mName )
                    {
                        return functionIt;
                    }
                    ++functionIt;
                }
            }
            llvm::ValueSymbolTable* valueSymbolTable = basicBlock->getValueSymbolTable();
            if ( valueSymbolTable != nullptr )
            {
                llvm::Value* foundValue = valueSymbolTable->lookup( mName );
                if ( foundValue != nullptr )
                {
                    return foundValue;
                }
            }
        }
        llvm::GlobalValue* foundValue = module.getNamedValue( mName );
        if ( foundValue )
        {
            return foundValue;
        }
        else
        {
            printError( "variable wasn't found" );
            return nullptr;
        }
    }


    const std::string VariableCall::show() const
    {
        return "VariableCall " + mName;
    }

} // namespace ast