#include <string>

#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"

#include "BinaryGenerator.hpp"
#include "Logging.hpp"



BinaryGenerator::BinaryGenerator( std::unique_ptr<llvm::Module> module ): mModule( std::move( module ) ) {}


bool BinaryGenerator::generate()
{
    std::string targetTriple = llvm::sys::getDefaultTargetTriple();
    printError( targetTriple );
    mModule->setTargetTriple( targetTriple );

    if ( !llvm::verifyModule( *mModule, &llvm::errs() ) )
    {
        printError( "verified" );
        //std::string filename = mModule->getSourceFileName() + ".o";
        std::string filename = "output.o";
        std::error_code errorCode;
        llvm::raw_fd_ostream outFile( filename, errorCode, llvm::sys::fs::OF_None );
        if ( !errorCode )
        {

            llvm::InitializeAllTargetInfos();
            llvm::InitializeAllTargets();
            llvm::InitializeAllTargetMCs();
            llvm::InitializeAllAsmParsers();
            llvm::InitializeAllAsmPrinters();

            std::string Error;
            const llvm::Target* target = llvm::TargetRegistry::lookupTarget( targetTriple, Error );
            if ( target != nullptr )
            {
                std::string cpu = llvm::sys::getHostCPUName();
                printError( cpu );
                std::string Features = "";
                llvm::TargetOptions opt;
                llvm::Optional<llvm::Reloc::Model> RM = llvm::Optional<llvm::Reloc::Model>();
                llvm::TargetMachine* targetMachine = target->createTargetMachine( targetTriple, cpu, Features, opt, RM );
                mModule->setDataLayout( targetMachine->createDataLayout() );

                llvm::WriteBitcodeToFile( *mModule, outFile );
                printError( "wrote to file" );
                return true;
            }
            else
            {
                printError( Error );
                return false;
            }
        }
        else
        {
            printError( "failed to write to file. error: " + errorCode.message() );
            return false;
        }
    }
    else
    {
        printError( "error: not verified" );
        return false;
    }

}
