#include "Number.hpp"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"


namespace ast
{

    Number::Number( const std::string& number, const bool& fpn ): mNumber( number ), mIsFpn( fpn ) {}


    llvm::Value* Number::generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const
    {
        if ( mIsFpn )
        {
            return llvm::ConstantInt::get( llvm::Type::getDoubleTy( module.getContext() ), std::stod( mNumber ) );
        }
        else
        {
            return llvm::ConstantInt::get( llvm::Type::getInt32Ty( module.getContext() ), std::stoi( mNumber ) );
        }
    }


    const std::string Number::show() const
    {
        return "Number " + mNumber;
    }

} // namespace ast