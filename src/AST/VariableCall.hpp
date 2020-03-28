#pragma once

#include <string>
#include "Type.hpp"


class VariableCall: public Type
{
private:
    std::string mName;

public:
    VariableCall( const std::string& name );
    virtual const std::string show() const override;
};