#include "VariableCall.hpp"
#include "../Logging.hpp"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ValueSymbolTable.h"



namespace ast
{

    VariableCall::VariableCall( const std::string& name ): mName( name ) {}


    llvm::Value* VariableCall::generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const
    {
        if ( basicBlock != nullptr )
        {
            llvm::ValueSymbolTable* variables = basicBlock->getValueSymbolTable();
            if ( variables != nullptr )
            {
                llvm::Value* foundValue = variables->lookup( mName );
                if ( foundValue != nullptr )
                {
                    return foundValue;
                }
            }
            if ( parentAvailableVariables != nullptr )
            {
                ValueSymbolTable::iterator foundValue = parentAvailableVariables->find( mName );
                if ( foundValue != parentAvailableVariables->end() )
                {
                    return foundValue->second;
                }
            }
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
        }
        llvm::GlobalValue* foundValue = module.getNamedValue( mName );
        if ( foundValue )
        {
            return foundValue;
        }
        else
        {
            printError( "variable '" + mName + "' wasn't found" );
            return nullptr;
        }
    }


    const std::string VariableCall::show() const
    {
        return "VariableCall " + mName;
    }

} // namespace ast