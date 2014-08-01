#ifndef _SVM_PROGRAM_UTILS_H_
#define _SVM_PROGRAM_UTILS_H_

#include "SVM_Opcodes.h"
#include "SVM_VirtualMachine.h"

#define SVM_OP_OBJ(code) Shogun::createObject((UInt32)code);

#define SVM_PROGRAM_BEGIN(name) Shogun::Program name; { Shogun::Program* __svm_pro = &name
#define SVM_PRO_OP(opcode) __svm_pro->push_back(Shogun::createObject((Shogun::UInt32)opcode))
#define SVM_PRO_ARG(argument) __svm_pro->push_back(Shogun::createObject(argument))
#define SVM_PROGRAM_END }

#endif