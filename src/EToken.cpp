#include "EToken.hpp"

std::ostream& operator<<( std::ostream& os, const EToken& token )
{
    //os << ;
    switch ( token )
    {
        case EToken::eof:
        {
            os << "eof";
            break;
        }
        case EToken::eol:
        {
            os << "eol";
            break;
        }
        case EToken::kv_import:
        {
            os << "kv_import";
            break;
        }
        case EToken::kv_return:
        {
            os << "kv_return";
            break;
        }
        case EToken::binary_operator:
        {
            os << "binary_operator";
            break;
        }
        case EToken::kv_constant:
        {
            os << "kv_constant";
            break;
        }
        case EToken::integer_literal:
        {
            os << "integer_literal";
            break;
        }
        case EToken::floating_point_literal:
        {
            os << "floating_point_literal";
            break;
        }
        case EToken::string_literal:
        {
            os << "string_literal";
            break;
        }
        case EToken::boolean_literal:
        {
            os << "boolean_literal";
            break;
        }
        case EToken::opening_round_bracket:
        {
            os << "opening_round_bracket";
            break;
        }
        case EToken::closing_round_bracket:
        {
            os << "closing_round_bracket";
            break;
        }
        case EToken::kv_class:
        {
            os << "kv_class";
            break;
        }
        case EToken::kv_structure:
        {
            os << "kv_structure";
            break;
        }
        case EToken::kv_interface:
        {
            os << "kv_interface";
            break;
        }
        case EToken::name:
        {
            os << "name";
            break;
        }
        case EToken::variable_declaration:
        {
            os << "variable_declaration";
            break;
        }
        case EToken::kv_true:
        {
            os << "kv_true";
            break;
        }
        case EToken::kv_false:
        {
            os << "kv_false";
            break;
        }
        case EToken::error:
        {
            os << "error";
            break;
        }
        default:
        {
            os << "undefined token name";
            break;
        }
    }
    return os;
}