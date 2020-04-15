#pragma once

#include "Node.hpp"


namespace ast
{

    class Boolean: public Node
    {
    private:
        bool mValue;

    public:
        Boolean( const bool& value );
    };

} // namespace ast
