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

    void generate_decl(const Declaration &decl);
    Type *getType(const std::unique_ptr<VariableType> &t);

public:
    static LLVMContext context;

    std::string name;

    Module module;
    llvm::IRBuilder<> builder;

public:
    LLVM_Gen(std::string name, std::vector<DeclarationPtr> &&body);
    ~LLVM_Gen();

    void generate();
};