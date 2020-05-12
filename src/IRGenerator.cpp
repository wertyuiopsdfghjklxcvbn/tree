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

bool IRGenerator::generate()
{
    llvm::LLVMContext context;
    std::shared_ptr<llvm::Module> module = std::make_shared<llvm::Module>( mModuleName, context );


    //llvm::FunctionType* FT = llvm::FunctionType::get( llvm::Type::getInt32Ty( context ), /*not vararg*/ false );
    //llvm::Function* F = llvm::Function::Create( FT, llvm::Function::ExternalLinkage, "main", *module );
    //llvm::BasicBlock* BB = llvm::BasicBlock::Create( context, "EntryBlock", F );

    llvm::BasicBlock* initialBasicBasicBlock = nullptr;
    if ( mParsedAST->generate( *module, initialBasicBasicBlock, nullptr ) == nullptr )
    {
        printError( "ast generating error", "\n\n" );
    }

    module->print( llvm::errs(), nullptr );

    return true;
}
