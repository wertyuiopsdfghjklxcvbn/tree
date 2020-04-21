#pragma once

#include <map>
#include <string>

#include "EToken.hpp"


typedef std::map<std::string, EToken> KeyWordsType;


//TODO make static
class KeyWords
{
private:
    const KeyWordsType mKeyWords = { { "import", EToken::kv_import },     { "return", EToken::kv_return },       { "constant", EToken::kv_constant },
                                     { "class", EToken::kv_class },       { "structure", EToken::kv_structure }, { "interface", EToken::kv_interface },
                                     { "true", EToken::boolean_literal }, { "false", EToken::boolean_literal } };

public:
    bool isKeyWord( const std::string& keyWord ) const;
    EToken getEToken( const std::string& keyWord ) const;
};
