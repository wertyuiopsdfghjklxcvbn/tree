#include "FunctionPrototype.hpp"

FunctionPrototype::FunctionPrototype( const VariableDeclaration& returnType, const std::list<VariableDeclaration>& arguments ):
    mReturnType( returnType ),
    mArguments( arguments )
{
}