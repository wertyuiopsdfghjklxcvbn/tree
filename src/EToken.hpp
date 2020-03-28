#pragma once

#include <ostream>

enum class EToken
{
    eof = 1,
    eol,
    //indent,
    //comment,
    kv_import,
    //kv_pass, //?
    //kv_none, //?
    kv_return,
    binary_operator,
    kv_constant,
    integer_literal,
    floating_point_literal,
    string_literal,
    boolean_literal,
    opening_round_bracket,
    closing_round_bracket,
    kv_class,
    kv_structure,
    kv_interface,
    //type,
    //identifier,
    name,
    variable_declaration,
    kv_true,
    kv_false,
    error
};

std::ostream& operator<< (std::ostream& os, const EToken& token);
