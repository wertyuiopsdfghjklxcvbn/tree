#include "Return.hpp"
#include "../Logging.hpp"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"



namespace ast
{

    Return::Return( std::unique_ptr<Node> returnValue ): mReturnValue( std::move( returnValue ) ) {}


    llvm::Value* Return::generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const
    {
        llvm::ReturnInst* returnInst;
        if ( mReturnValue )
        {
            llvm::Value* value = mReturnValue->generate( module, basicBlock, parentAvailableVariables );
            if ( value != nullptr )
            {
                returnInst = llvm::ReturnInst::Create( module.getContext(), value );
            }
            else
            {
                printError( "return generation error" );
                return nullptr;
            }
        }
        else
        {
            returnInst = llvm::ReturnInst::Create( module.getContext() );
        }
        basicBlock->getInstList().push_back( returnInst );
        return returnInst;
    }


    const std::string Return::show() const
    {
        if ( mReturnValue )
        {
            return "Return " + mReturnValue->show();
        }
        else
        {
            return "Return\n";
        }
    }

} // namespace ast