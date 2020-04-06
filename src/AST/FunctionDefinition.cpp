#include "FunctionDefinition.hpp"


FunctionDefinition::FunctionDefinition( const VariableDeclaration& returnType,
                                        const std::list<VariableDeclaration>& arguments,
                                        std::list<std::unique_ptr<Type>>& body ):
    mReturnType( returnType ),
    mArguments( arguments ),
    mBody( std::move( body ) )
{
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