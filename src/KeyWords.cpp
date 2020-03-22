#include "KeyWords.hpp"


bool KeyWords::isKeyWord( const std::string& keyWord ) const
{
    KeyWordsType::const_iterator it = mKeyWords.find( keyWord );
    if ( it != mKeyWords.end() )
    {
        return true;
    }
    else
    {
        return false;
    }
}