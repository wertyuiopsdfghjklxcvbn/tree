#include "BinaryExpression.hpp"


BinaryExpression::BinaryExpression( const std::string& operation, std::unique_ptr<Type> leftHandSide, std::unique_ptr<Type> rightHandSide ):
    mOperation( std::move( operation ) ),
    mLeftHandSide( std::move( leftHandSide ) ),
    mRightHandSide( std::move( rightHandSide ) )
{
}

const std::string BinaryExpression::show() const
{
    return ( "BinaryOperation: " + mOperation + "\n" + mLeftHandSide->show() + "\n" + mRightHandSide->show() + "\n" );
}