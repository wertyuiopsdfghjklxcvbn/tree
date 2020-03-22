#pragma once

#include <list>
#include <map>
#include <string>


typedef std::map<const std::string, const unsigned int> BinaryOperatorsType;


//TODO make static
class Operators
{
private:
    const BinaryOperatorsType mBinaryOperators = { { "+", 10 }, { "-", 10 }, { "*", 20 }, { "=", 5 }, { "and", 40 }, { "+=", 0 }, { "in", 0 }, { "or", 40 } };
    //const std::list<const char*> mUnaryOperators = { "not" };

public:
    bool isBinaryOperator( const std::string& binaryOperator ) const;
    const unsigned int getBinaryOperatorPrecedence( const std::string& binaryOperator ) const;
    const unsigned int getMinimalOperatorPrecedence() const;
};
