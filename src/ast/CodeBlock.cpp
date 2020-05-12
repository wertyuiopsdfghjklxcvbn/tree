#include "CodeBlock.hpp"
#include "../Logging.hpp"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Module.h"



namespace ast
{
    CodeBlock::CodeBlock( std::list<std::unique_ptr<Node>>& body ): mBody( std::move( body ) ) {}


    const std::string CodeBlock::show() const
    {
        std::string returnValue = "block size: " + std::to_string( mBody.size() ) + '\n';
        for ( const std::unique_ptr<Node>& lineOfCode : mBody )
        {
            returnValue += lineOfCode->show();
        }
        return returnValue;
    }


    llvm::Value* CodeBlock::generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const
    {
        for ( const std::unique_ptr<Node>& lineOfCode : mBody )
        {
            //printError( lineOfCode->show() );
            if ( lineOfCode->generate( module, basicBlock, parentAvailableVariables ) == nullptr )
            {
                printError( "error block generating" );
                return nullptr;
            }
        }
        if ( basicBlock != nullptr )
        {
            return basicBlock;
        }
        else
        {
            //for top level error handling(false positive)
            return llvm::BasicBlock::Create( module.getContext(), "dummy");
        }
    }

} // namespace ast