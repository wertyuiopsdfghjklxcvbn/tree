#pragma once

#include "Type.hpp"

class Boolean: public Type
{
private:
    bool mValue;

public:
    Boolean( const bool& value );
};
