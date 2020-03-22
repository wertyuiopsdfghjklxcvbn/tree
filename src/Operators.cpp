#include "Operators.hpp"


bool Operators::isBinaryOperator( const std::string& binaryOperator ) const
{
    BinaryOperatorsType::const_iterator it = mBinaryOperators.find( binaryOperator );
    if ( it != mBinaryOperators.end() )
    {
        return true;
    }
    else
    {
        return false;
    }
}


const unsigned int Operators::getBinaryOperatorPrecedence( const std::string& binaryOperator ) const
{
    BinaryOperatorsType::const_iterator it = mBinaryOperators.find( binaryOperator );
    if ( it != mBinaryOperators.end() )
    {
        return it->second;
    }
    else
    {
        return -1;
    }
}


const unsigned int Operators::getMinimalOperatorPrecedence() const
{
    return 1;
}