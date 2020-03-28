#include "VariableCall.hpp"

VariableCall::VariableCall( const std::string& name ): mName( name ) {}

const std::string VariableCall::show() const
{
    return "VariableCall " + mName;
}