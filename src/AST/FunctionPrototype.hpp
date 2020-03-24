#pragma once

#include <list>
#include <string>
#include <utility>
#include "Type.hpp"
#include "VariableDeclaration.hpp"

class FunctionPrototype: public Type
{
private:
    VariableDeclaration mReturnType;
    std::list<VariableDeclaration> mArguments;

public:
    FunctionPrototype( const VariableDeclaration& returnType, const std::list<VariableDeclaration>& arguments );
    virtual const std::string show() const override;
};