// main.c
// #include <stdint.h>
#include <stdio.h>
// #include <stdlib.h>
// #include <sys/stat.h>
// #include <sys/sysmacros.h>
// #include <time.h>

#include "instruction.h"
#include "global.h"

// Фиктивный операнд - заглушка при невозможности определить его параметры
// Operand unknownSrcOperand;

InstructionList iList01;
struct Constant.Int val01;

// Проверка формирования глобальной обобщенной переменной в библиотеке
// extern struct Operand uOpd;

int main(int argc, char *argv[]) {
  printf("Start\n");

  InitInstructionList(&iList01);
  DebugOutOfInstructionList(&iList01, stdout);

  Instruction* i01 = CreateInstructionHalt();
  AppendToInstructionList(&iList01, i01);
  i01 = CreateInstructionHalt();
  AppendToInstructionList(&iList01, i01);
  i01 = CreateInstructionHalt();
  AppendToInstructionList(&iList01, i01);

  SetConstantInt(&val01, 10);
  Operand* opd01 = CreateOperandConstant((Constant*)&val01);
  i01 = CreateInstructionExit(opd01);
  AppendToInstructionList(&iList01, i01);

  Type* type03 = GetTypeInt();
  struct Constant* val03 = CreateConstantInt(42);
  Variable* var03 = CreateVariableLocal(type03, val03);
  Operand* opd03 = CreateOperandVariable(var03);
  i01 = CreateInstructionExit(opd03);
  AppendToInstructionList(&iList01, i01);

  // Minus
  i01 = CreateInstructionMinus(opd01);
  AppendToInstructionList(&iList01, i01);
  i01 = CreateInstructionMinus(opd03);
  AppendToInstructionList(&iList01, i01);
  // i01 = CreateInstructionMinus(GetUnknownOperand());
  // AppendToInstructionList(&iList01, i01);

  // Add
  i01 = CreateInstructionAdd(opd01, opd01);
  AppendToInstructionList(&iList01, i01);
  i01 = CreateInstructionAdd(opd01, opd03);
  AppendToInstructionList(&iList01, i01);
  i01 = CreateInstructionAdd(opd03, opd01);
  AppendToInstructionList(&iList01, i01);
  i01 = CreateInstructionAdd(opd03, opd03);
  AppendToInstructionList(&iList01, i01);
  struct Constant* val04 = GetConstIntZero();
  Operand* opd04 = CreateOperandConstant(val04);
  i01 = CreateInstructionAdd(opd04, opd01);
  AppendToInstructionList(&iList01, i01);

  DebugOutOfInstructionList(&iList01, stdout);

  printf("Stop\n");
  return 0;
}
