#pragma once

#include <map>
#include <string>

#include "EToken.hpp"


typedef std::map<const std::string, const EToken> KeyWordsType;


//TODO make static
class KeyWords
{
private:
    const KeyWordsType mKeyWords = { { "import", EToken::kv_import }, { "return", EToken::kv_return },       { "constant", EToken::kv_constant },
                                     { "class", EToken::kv_class },   { "structure", EToken::kv_structure }, { "interface", EToken::kv_interface },
                                     { "true", EToken::kv_true },     { "false", EToken::kv_false } };

public:
    bool isKeyWord( const std::string& keyWord ) const;
};
