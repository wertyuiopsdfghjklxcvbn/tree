#include "Boolean.hpp"
#include <string>
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"



namespace ast
{

    Boolean::Boolean( const bool& value ): mValue( value ) {}


    const std::string Boolean::show() const
    {
        return "Boolean: " + std::to_string( mValue );
    }


    llvm::Value* Boolean::generate( llvm::Module& module, llvm::BasicBlock* basicBlock ) const
    {
        if ( mValue )
        {
            return llvm::ConstantInt::getTrue( module.getContext() );
        }
        else
        {
            return llvm::ConstantInt::getFalse( module.getContext() );
        }
    }


} // namespace ast