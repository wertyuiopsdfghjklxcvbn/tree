#include "FunctionCall.hpp"
#include <string>

FunctionCall::FunctionCall( const std::string& name, std::list<std::unique_ptr<Type>>& arguments ): mName( name ), mArguments( std::move( arguments ) ) {}

const std::string FunctionCall::show() const
{
    return mName + " " + std::to_string( mArguments.size() );
}