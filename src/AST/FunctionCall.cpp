#include <string>

#include "FunctionCall.hpp"
#include "llvm/IR/Argument.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/ValueSymbolTable.h"
#include "llvm/Support/raw_ostream.h"



FunctionCall::FunctionCall( const std::string& name, std::list<std::unique_ptr<Type>>& arguments ): mName( name ), mArguments( std::move( arguments ) ) {}


llvm::Value* FunctionCall::generate( llvm::Module& module, llvm::BasicBlock* basicBlock ) const
{
    llvm::Function* function = module.getFunction( mName );
    if ( function != nullptr )
    {
        std::vector<llvm::Value*> args;
        for ( const auto& i : mArguments )
        {
            llvm::Value* value = i->generate( module, basicBlock );
            if ( value != nullptr )
            {
                if ( !( value->getType()->isPointerTy() ) )
                {
                    //TODO check constant
                    const llvm::DataLayout& dataLayout = basicBlock->getParent()->getParent()->getDataLayout();
                    llvm::AllocaInst* allocaInst = new llvm::AllocaInst( value->getType(), dataLayout.getAllocaAddrSpace(), "tempLiteralPointer" );
                    basicBlock->getInstList().push_back( allocaInst );
                    llvm::StoreInst* storeInst = new llvm::StoreInst( value, allocaInst );
                    basicBlock->getInstList().push_back( storeInst );
                    value = allocaInst;
                }
                args.push_back( value );
            }
            else
            {
                return nullptr;
            }
        }

        llvm::CallInst* callInst = llvm::CallInst::Create( llvm::FunctionCallee( function ), args, "functionCallResult", basicBlock );
        return callInst;
    }
    else
    {
        return nullptr;
    }
}


const std::string FunctionCall::show() const
{
    return mName + " " + std::to_string( mArguments.size() );
}