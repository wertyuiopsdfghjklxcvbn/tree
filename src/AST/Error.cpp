#include "Error.hpp"


Error::Error( const std::string& message ): mMessage( message ) {}
const std::string Error::show() const {
    return "error: " + mMessage;
}