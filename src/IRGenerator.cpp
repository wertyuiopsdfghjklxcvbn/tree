#include "IRGenerator.hpp"
#include <iostream>
#include <memory>
#include "Logging.hpp"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/ValueSymbolTable.h"
#include "llvm/Support/raw_ostream.h"



IRGenerator::IRGenerator( std::list<std::unique_ptr<ast::Node>>& parsedAST, const std::string& moduleName ): mParsedAST( parsedAST ), mModuleName( moduleName ) {}

bool IRGenerator::generate()
{
    llvm::LLVMContext context;
    std::shared_ptr<llvm::Module> module = std::make_shared<llvm::Module>( mModuleName, context );


    //llvm::FunctionType* FT = llvm::FunctionType::get( llvm::Type::getInt32Ty( context ), /*not vararg*/ false );
    //llvm::Function* F = llvm::Function::Create( FT, llvm::Function::ExternalLinkage, "main", *module );
    //llvm::BasicBlock* BB = llvm::BasicBlock::Create( context, "EntryBlock", F );

    for ( std::list<std::unique_ptr<ast::Node>>::iterator iter = mParsedAST.begin(); iter != mParsedAST.end(); iter++ )
    {
        llvm::Value* t = iter->get()->generate( *module, nullptr );

        printError( iter->get()->show() );
    }


    module->print( llvm::errs(), nullptr );


    return true;
}
