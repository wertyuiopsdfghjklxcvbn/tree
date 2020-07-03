#pragma once

#include <memory>
#include "llvm/IR/Module.h"


class BinaryGenerator
{
private:
    std::unique_ptr<llvm::Module> mModule;

public:
    BinaryGenerator( std::unique_ptr<llvm::Module> module );
    bool generate();
};