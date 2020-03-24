#pragma once


#include <string>
#include <utility>
#include "Type.hpp"


class VariableDeclaration: public Type
{
private:
    std::string mType;
    std::string mName;
    bool mIsconstant;

public:
    VariableDeclaration( const std::string& type, const std::string& name, const bool& isconstant = false );
    virtual const std::string show() const override;
};