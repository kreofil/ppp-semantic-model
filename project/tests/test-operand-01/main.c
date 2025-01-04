// main.c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>

#include "operand.h"

struct Type.Int type01;
struct Constant.Int val01;

int main(int argc, char *argv[]) {
  printf("Start\n");
  InitTypeInt(&type01);
  SetConstantInt(&val01, 10);
  Variable* var01 = CreateVariableGlobal((Type*)&type01, (Constant*)&val01);
  Operand* opd01 = CreateOperandVariable(var01);
  DebugOutOfOperand<opd01>(stdout);
  free(var01);
  free(opd01);

  struct Constant.Int val02;
  SetConstantInt(&val02, 13);
  Operand* opd02 = CreateOperandConstant((Constant*)&val02);
  DebugOutOfOperand<opd02>(stdout);
  free(opd02);

  struct Type* type03 = CreateTypeInt();
  struct Constant* val03 = CreateConstantInt(42);
  Variable* var03 = CreateVariableLocal(type03, val03);
  Operand* opd03 = CreateOperandVariable(var03);
  DebugOutOfOperand<opd03>(stdout);
  free(type03);
  free(var03);
  free(opd03);
  opd03 = CreateOperandConstant(val03);
  DebugOutOfOperand<opd03>(stdout);
  free(val03);
  free(opd03);

  printf("Stop\n");
  exit(EXIT_SUCCESS);
}
