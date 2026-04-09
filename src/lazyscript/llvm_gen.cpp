#include "lazyscript/llvm_gen.hpp"

LLVMContext LLVM_Gen::context = LLVMContext();

LLVM_Gen::LLVM_Gen(std::string name, std::vector<DeclarationPtr> &&body)
    : name(std::move(name)),
      module(this->name, LLVM_Gen::context),
      builder(LLVM_Gen::context),
      body(std::move(body))
{
    module.setSourceFileName(name);
}

Type *LLVM_Gen::getType(const std::unique_ptr<VariableType> &t)
{
    return builder.getVoidTy();
}

void LLVM_Gen::generate()
{
    for (const auto &declPtr : body)
    {
        if (declPtr)
        {
            generate_decl(*declPtr);
        }
    }

    module.print(outs(), nullptr);
}

void LLVM_Gen::generate_decl(const Declaration &decl)
{
    if (auto *func = dynamic_cast<const FunctionDeclaration *>(&decl))
    {
        FunctionType *fnType = FunctionType::get(getType(func->type), false);
        Function *fn = Function::Create(fnType, GlobalObject::LinkageTypes::ExternalLinkage, func->name, module);

        BasicBlock *entry = BasicBlock::Create(context, "entry", fn);
        builder.SetInsertPoint(entry);
    }
    else
    {
    }
}

LLVM_Gen::~LLVM_Gen()
{
}
