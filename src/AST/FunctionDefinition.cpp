#include "FunctionDefinition.hpp"
#include "llvm/IR/Argument.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"


namespace ast
{


    FunctionDefinition::FunctionDefinition( const VariableDeclaration& returnType,
                                            const std::list<VariableDeclaration>& arguments,
                                            std::list<std::unique_ptr<Node>>& body ):
        mReturnType( returnType ),
        mArguments( arguments ),
        mBody( std::move( body ) )
    {
    }


    llvm::Value* FunctionDefinition::generate( llvm::Module& module, llvm::BasicBlock* basicBlock ) const
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
        if ( !mBody.empty() )
        {
            llvm::BasicBlock* functionBasicBlock = llvm::BasicBlock::Create( module.getContext(), "EntryBlock", function );
            std::list<std::unique_ptr<Node>>::const_iterator bodyIt = mBody.begin();
            while ( bodyIt != mBody.end() )
            {
                bodyIt->get()->generate( module, functionBasicBlock );
                ++bodyIt;
            }
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
        return "FunctionDefinition: " + mReturnType.show() + " " + std::to_string( mArguments.size() ) + s + " " + std::to_string( mBody.size() );
    }

} // namespace ast