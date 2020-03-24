#include "VariableDeclaration.hpp"

VariableDeclaration::VariableDeclaration( const std::string& type, const std::string& name, const bool& isconstant ):
    mType( type ),
    mName( name ),
    mIsconstant( isconstant )
{
}
const std::string VariableDeclaration::show() const
{
    return mType + " " + mName + " " + std::to_string( mIsconstant );
}