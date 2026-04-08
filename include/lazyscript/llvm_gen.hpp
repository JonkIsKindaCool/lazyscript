#pragma once

#include "lazyscript/ast.hpp"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <vector>

using namespace llvm;

class LLVM_Gen
{
private:
    std::vector<DeclarationPtr> body;
public:
    LLVM_Gen();
    ~LLVM_Gen();
};