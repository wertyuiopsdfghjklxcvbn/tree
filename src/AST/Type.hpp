#pragma once

#include <string>

class Type
{
public:
    virtual const std::string show() const = 0;
    virtual ~Type() = 0;
};
