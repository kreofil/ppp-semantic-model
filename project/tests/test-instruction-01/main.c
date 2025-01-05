// main.c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>

#include "instruction.h"

InstructionList iList01;
struct Constant.Int val01;

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

  struct Type* type03 = CreateTypeInt();
  struct Constant* val03 = CreateConstantInt(42);
  Variable* var03 = CreateVariableLocal(type03, val03);
  Operand* opd03 = CreateOperandVariable(var03);
  i01 = CreateInstructionExit(opd03);
  AppendToInstructionList(&iList01, i01);

  i01 = CreateInstructionMinus(opd01);
  AppendToInstructionList(&iList01, i01);

  i01 = CreateInstructionMinus(opd03);
  AppendToInstructionList(&iList01, i01);

  DebugOutOfInstructionList(&iList01, stdout);

  printf("Stop\n");
  exit(EXIT_SUCCESS);
}
