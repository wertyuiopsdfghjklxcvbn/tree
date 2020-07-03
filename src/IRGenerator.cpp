#include <memory>

#include "IRGenerator.hpp"
#include "Logging.hpp"
#include "llvm/IR/Function.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ValueSymbolTable.h"
#include "llvm/Support/raw_ostream.h"



IRGenerator::IRGenerator( std::unique_ptr<ast::CodeBlock>& parsedAST, const std::string& moduleName ):
    mParsedAST( std::move( parsedAST ) ),
    mModuleName( moduleName )
{
}

std::pair<std::unique_ptr<llvm::Module>, std::unique_ptr<llvm::LLVMContext>> IRGenerator::generate()
{
    std::unique_ptr<llvm::LLVMContext> context = std::make_unique<llvm::LLVMContext>();
    std::unique_ptr<llvm::Module> module = std::make_unique<llvm::Module>( mModuleName, *context );

    llvm::BasicBlock* initialBasicBasicBlock = nullptr;
    if ( mParsedAST->generate( *module, initialBasicBasicBlock, nullptr ) == nullptr )
    {
        printError( "ast generating error", "\n\n" );
    }

    module->print( llvm::errs(), nullptr );

    return { std::move( module ), std::move( context ) };
}
