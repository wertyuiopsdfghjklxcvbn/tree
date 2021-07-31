#include <typeinfo>

#include "../Logging.hpp"
#include "FunctionDefinition.hpp"
#include "llvm/IR/Argument.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"



namespace ast
{


    FunctionDefinition::FunctionDefinition( const VariableDeclaration& returnType,
                                            const std::list<VariableDeclaration>& arguments,
                                            std::unique_ptr<CodeBlock>&& body ):
        mReturnType( returnType ),
        mArguments( arguments ),
        mBody( std::move( body ) )
    {
    }


    llvm::Value* FunctionDefinition::generate( llvm::Module& module, llvm::BasicBlock*& basicBlock, ValueSymbolTable* parentAvailableVariables ) const
    {
        //TODO mReturnType.getType
        std::vector<llvm::Type*> args;
        for ( const auto& i : mArguments )
        {
            i.getType();
            args.push_back( llvm::PointerType::getUnqual( llvm::Type::getInt32Ty( module.getContext() ) ) );
        }
        llvm::FunctionType* functionType = llvm::FunctionType::get( llvm::Type::getInt32Ty( module.getContext() ), args, false );
        llvm::Function* function = llvm::Function::Create( functionType, llvm::Function::ExternalLinkage, mReturnType.getName(), module );

        std::list<VariableDeclaration>::const_iterator it = mArguments.begin();
        llvm::Function::arg_iterator functionIt = function->arg_begin();
        while ( it != mArguments.end() )
        {
            //TODO check equal args names
            functionIt->setName( it->getName() );
            ++it;
            ++functionIt;
        }

        llvm::BasicBlock* functionBasicBlock = llvm::BasicBlock::Create( module.getContext(), "EntryBlock", function );
        if ( mBody->generate( module, functionBasicBlock, parentAvailableVariables ) == nullptr )
        {
            printError( "function body generation error" );
            return nullptr;
        }
        return function;
    }


    const std::string FunctionDefinition::show() const
    {
        std::string s;
        for ( auto& i : mArguments )
        {
            s += i.show() + " ";
        }
        return std::string( typeid( *this ).name() ) + ": " + mReturnType.show() + " " + std::to_string( mArguments.size() ) + s + " " + mBody->show();
    }

} // namespace ast