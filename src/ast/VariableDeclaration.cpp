#include "VariableDeclaration.hpp"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/ValueSymbolTable.h"


namespace ast
{


    VariableDeclaration::VariableDeclaration( const std::string& type, const std::string& name, const bool& isconstant ):
        mType( type ),
        mName( name ),
        mIsconstant( isconstant )
    {
    }


    llvm::Value* VariableDeclaration::generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const
    {
        if ( basicBlock != nullptr )
        {
            const llvm::DataLayout& DL = basicBlock->getParent()->getParent()->getDataLayout();
            llvm::AllocaInst* allocaInst = new llvm::AllocaInst( llvm::Type::getInt32Ty( module.getContext() ), DL.getAllocaAddrSpace(), mName );
            basicBlock->getInstList().push_back( allocaInst );
            return basicBlock->getValueSymbolTable()->lookup( mName );
        }
        else
        {
            llvm::GlobalVariable* globalVariable = new llvm::GlobalVariable( module,
                                                                             llvm::Type::getInt32Ty( module.getContext() ),
                                                                             mIsconstant,
                                                                             llvm::GlobalValue::LinkageTypes::ExternalLinkage,
                                                                             nullptr,
                                                                             mName );
            return globalVariable;
        }
    }


    const std::string VariableDeclaration::show() const
    {
        return mType + " " + mName + " " + std::to_string( mIsconstant );
    }

    const std::string& VariableDeclaration::getType() const
    {
        return mType;
    }
    const std::string& VariableDeclaration::getName() const
    {
        return mName;
    }

} // namespace ast