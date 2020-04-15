#include "Return.hpp"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"


namespace ast
{

    Return::Return( std::unique_ptr<Node> returnValue ): mReturnValue( std::move( returnValue ) ) {}


    llvm::Value* Return::generate( llvm::Module& module, llvm::BasicBlock* basicBlock ) const
    {
        llvm::ReturnInst* returnInst;
        if ( mReturnValue )
        {
            llvm::Value* value = mReturnValue->generate( module, basicBlock );
            if ( value != nullptr )
            {
                returnInst = llvm::ReturnInst::Create( module.getContext(), value );
            }
            else
            {
                //err
                return nullptr;
            }
        }
        else
        {
            returnInst = llvm::ReturnInst::Create( module.getContext() );
        }
        basicBlock->getInstList().push_back( returnInst );
        return nullptr;
    }


    const std::string Return::show() const
    {
        if ( mReturnValue )
        {
            return "Return " + mReturnValue->show();
        }
        else
        {
            return "Return";
        }
    }

} // namespace ast