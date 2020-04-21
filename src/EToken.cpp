#include "EToken.hpp"


std::string tokenToString( const EToken& token )
{
    std::string returnValue;
    switch ( token )
    {
        case EToken::eof:
        {
            returnValue = "eof";
            break;
        }
        case EToken::eol:
        {
            returnValue = "eol";
            break;
        }
        case EToken::kv_import:
        {
            returnValue = "kv_import";
            break;
        }
        case EToken::kv_return:
        {
            returnValue = "kv_return";
            break;
        }
        case EToken::binary_operator:
        {
            returnValue = "binary_operator";
            break;
        }
        case EToken::kv_constant:
        {
            returnValue = "kv_constant";
            break;
        }
        case EToken::integer_literal:
        {
            returnValue = "integer_literal";
            break;
        }
        case EToken::floating_point_literal:
        {
            returnValue = "floating_point_literal";
            break;
        }
        case EToken::string_literal:
        {
            returnValue = "string_literal";
            break;
        }
        case EToken::boolean_literal:
        {
            returnValue = "boolean_literal";
            break;
        }
        case EToken::opening_round_bracket:
        {
            returnValue = "opening_round_bracket";
            break;
        }
        case EToken::closing_round_bracket:
        {
            returnValue = "closing_round_bracket";
            break;
        }
        case EToken::kv_class:
        {
            returnValue = "kv_class";
            break;
        }
        case EToken::kv_structure:
        {
            returnValue = "kv_structure";
            break;
        }
        case EToken::kv_interface:
        {
            returnValue = "kv_interface";
            break;
        }
        case EToken::name:
        {
            returnValue = "name";
            break;
        }
        case EToken::call:
        {
            returnValue = "call";
            break;
        }
        case EToken::variable_declaration:
        {
            returnValue = "variable_declaration";
            break;
        }
        case EToken::function_definition:
        {
            returnValue = "function_definition";
            break;
        }
        case EToken::comma:
        {
            returnValue = "comma";
            break;
        }
        case EToken::indent:
        {
            returnValue = "indent";
            break;
        }
        /*case EToken::kv_true:
        {
            returnValue = "kv_true";
            break;
        }
        case EToken::kv_false:
        {
            returnValue = "kv_false";
            break;
        }*/
        case EToken::error:
        {
            returnValue = "error";
            break;
        }
        default:
        {
            returnValue = "undefined token name";
            break;
        }
    }
    return returnValue;
}

std::ostream& operator<<( std::ostream& os, const EToken& token )
{
    return os << tokenToString( token );
}