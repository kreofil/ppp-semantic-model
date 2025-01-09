// main.c
#include <stdlib.h>

#include "variable.h"
#include "global.h"

struct Type.Int type01;
struct Constant.Int val01;

int main(int argc, char *argv[]) {
  printf("Start\n");
  InitTypeInt(&type01);
  SetConstantInt(&val01, 10);
  Variable* var01 = CreateVariableGlobal((Type*)&type01, (Constant*)&val01);
  DebugOutOfVariableCommon(var01, stdout);
  free(var01);

  struct Type.Int type02;
  struct Constant.Int val02;
  InitTypeInt(&type02);
  SetConstantInt(&val02, 13);
  Variable* var02 = CreateVariableExternal((Type*)&type02, (Constant*)&val02);
  DebugOutOfVariableCommon(var02, stdout);
  free(var02);

  struct Type* type03 = CreateTypeInt();
  struct Constant* val03 = CreateConstantInt(42);
  Variable* var03 = CreateVariableLocal(type03, val03);
  DebugOutOfVariableCommon(var03, stdout);
  free(type03);
  free(val03);
  free(var03);

  type03 = GetTypeInt();
  val03 = CreateConstantInt(666);
  var03 = CreateVariableTemp(type03, val03);
  DebugOutOfVariableCommon(var03, stdout);
  free(val03);
  free(var03);

  var03 = CreateVariableTemp(GetTypeInt(), GetConstIntZero());
  DebugOutOfVariableCommon(var03, stdout);
  free(var03);

  printf("Stop\n");
  return 0;
}
