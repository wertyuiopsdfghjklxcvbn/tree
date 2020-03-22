#include "Number.hpp"

Number::Number( const std::string& number, const bool& fpn ): mNumber( number ), mIsFpn( fpn ) {}

const std::string Number::show() const
{
    return "Number " + mNumber;
}