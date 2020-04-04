#pragma once

#include <list>
#include <memory>
#include <string>
#include <utility>
#include "Type.hpp"
#include "VariableDeclaration.hpp"


class FunctionDefinition: public Type
{
private:
    VariableDeclaration mReturnType;
    std::list<VariableDeclaration> mArguments;
    std::list<std::unique_ptr<Type>> mBody;

public:
    FunctionDefinition( const VariableDeclaration& returnType, const std::list<VariableDeclaration>& arguments, std::list<std::unique_ptr<Type>>& body );
    virtual const std::string show() const override;
};