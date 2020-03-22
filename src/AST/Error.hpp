#pragma once

#include <string>

#include "Type.hpp"

class Error: public Type
{
private:
    std::string mMessage;

public:
    Error( const std::string& message );
    virtual const std::string show() const override;
};