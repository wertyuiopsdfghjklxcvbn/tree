#include "BinaryExpression.hpp"
#include "../Logging.hpp"
#include "../Operators.hpp"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"



namespace ast
{

    BinaryExpression::BinaryExpression( const std::string& operation, std::unique_ptr<Node> leftHandSide, std::unique_ptr<Node> rightHandSide ):
        mOperation( std::move( operation ) ),
        mLeftHandSide( std::move( leftHandSide ) ),
        mRightHandSide( std::move( rightHandSide ) )
    {
    }


    llvm::Value* BinaryExpression::generate( llvm::Module& module, llvm::BasicBlock* basicBlock ) const
    {
        llvm::Value* lhs = mLeftHandSide->generate( module, basicBlock );
        llvm::Value* rhs = mRightHandSide->generate( module, basicBlock );
        if ( lhs != nullptr && rhs != nullptr )
        {
            //TODO rewrite as enum
            if ( mOperation == "+" )
            {
                if ( basicBlock != nullptr )
                {
                    if ( rhs->getType()->isPointerTy() )
                    {
                        llvm::LoadInst* loadInst = new llvm::LoadInst( rhs->getType()->getPointerElementType(), rhs, "loadTempVariable" );
                        basicBlock->getInstList().push_back( loadInst );
                        rhs = loadInst;
                    }
                    if ( lhs->getType()->isPointerTy() )
                    {
                        llvm::LoadInst* loadInst = new llvm::LoadInst( lhs->getType()->getPointerElementType(), lhs, "loadTempVariable" );
                        basicBlock->getInstList().push_back( loadInst );
                        lhs = loadInst;
                    }
                    //TODO diferent for fpn
                    llvm::Instruction* instruction = llvm::BinaryOperator::Create( llvm::Instruction::Add, lhs, rhs );
                    basicBlock->getInstList().push_back( instruction );
                    return instruction;
                }
                else
                {
                    return nullptr; //make global
                }
            }
            else if ( mOperation == "=" )
            {
                if ( basicBlock != nullptr )
                {
                    llvm::StoreInst* storeInst;
                    if ( rhs->getType()->isPointerTy() )
                    {
                        llvm::LoadInst* loadInst = new llvm::LoadInst( lhs->getType()->getPointerElementType(), rhs, "assignmentTempVariable" );
                        basicBlock->getInstList().push_back( loadInst );
                        storeInst = new llvm::StoreInst( loadInst, lhs );
                    }
                    else
                    {
                        storeInst = new llvm::StoreInst( rhs, lhs );
                    }
                    basicBlock->getInstList().push_back( storeInst );
                    return storeInst;
                }
                else
                {
                    llvm::GlobalVariable* foundValue = module.getGlobalVariable( lhs->getName() );
                    if ( foundValue )
                    {
                        //init global var
                        //rhs->getType();
                        //llvm::Constant* constant = llvm::ConstantInt::get( llvm::Type::getInt32Ty( module.getContext() ), 5 );
                        //foundValue->setInitializer( constant );
                    }
                    else
                    {
                        //err
                    }
                    return nullptr;
                }
            }
            else
            {
                printError( "unknown operator" );
                return nullptr;
            }
        }
        else
        {
            printError( "invalid binary operator parametr" );
            return nullptr;
        }
    }


    const std::string BinaryExpression::show() const
    {
        return ( "BinaryOperation: " + mOperation + "\n" + mLeftHandSide->show() + "\n" + mRightHandSide->show() + "\n" );
    }

} // namespace ast