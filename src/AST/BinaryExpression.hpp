#pragma once

#include <memory>
#include <string>
#include "Type.hpp"


class BinaryExpression: public Type
{
private:
    std::string mOperation;
    std::unique_ptr<Type> mLeftHandSide;
    std::unique_ptr<Type> mRightHandSide;

public:
    BinaryExpression( const std::string& operation, std::unique_ptr<Type> leftHandSide, std::unique_ptr<Type> rightHandSide );

    virtual const std::string show() const override;
};
