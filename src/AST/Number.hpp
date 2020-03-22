#pragma once

#include <string>
#include "Type.hpp"

class Number: public Type
{
private:
    std::string mNumber;
    bool mIsFpn;

public:
    Number( const std::string& number, const bool& fpn );

    virtual const std::string show() const override;
};
