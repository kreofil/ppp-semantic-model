// main.c
#include <stdio.h>
#include <stdlib.h>

#include "context.h"
#include "name-table.h"

struct Constant.Int intConstExt;
// struct Context<Const<Int> > intConstContextExt;

int main(int argc, char *argv[]) {
  printf("Start\n");

  NameTable nameTable;
  struct Constant.Int intConst;
  struct Context.Const intContext;
  intContext.@ = (Constant*)&intConst;
  Context *pContext;

  InitNameTable(&nameTable);
  DebugOutOfNameTable(&nameTable, stdout);

  pContext = CreateContextConstInt(10);
  // DebugOutOfContext<pContext>(stdout);
  AddElementToNameTable(&nameTable, L"id_int_10", pContext);
  DebugOutOfNameTable(&nameTable, stdout);

  pContext = CreateContextConstInt(20);
  // DebugOutOfContext<pContext>(stdout);
  AddElementToNameTable(&nameTable, L"id_int_20", pContext);
  DebugOutOfNameTable(&nameTable, stdout);

  pContext = CreateContextConstInt(30);
  // DebugOutOfContext<pContext>(stdout);
  AddElementToNameTable(&nameTable, L"id_int_30", pContext);
  DebugOutOfNameTable(&nameTable, stdout);

  pContext = CreateContextConstInt(40);
  // DebugOutOfContext<pContext>(stdout);
  AddElementToNameTable(&nameTable, L"id_int_40", pContext);
  DebugOutOfNameTable(&nameTable, stdout);

  pContext = CreateContextConstInt(50);
  // DebugOutOfContext<pContext>(stdout);
  AddElementToNameTable(&nameTable, L"id_int_50", pContext);
  DebugOutOfNameTable(&nameTable, stdout);

  NameTableElement *pElement = findElementInTable(&nameTable, L"id_int_40");
  if(pElement != NULL) {
    printf("Finded Element is:\n ");
    DebugOutOfNameTableElement(pElement, stdout);
  } else {
    printf("No element using \"id_int_40\" name");
  }

  if((pElement = findElementInTable(&nameTable, L"id_int_10")) != NULL) {
    printf("Finded Element is:\n ");
    DebugOutOfNameTableElement(pElement, stdout);
  } else {
    printf("No element using \"id_int_10\" name");
  }

  if((pElement = findElementInTable(&nameTable, L"id_int_1000")) != NULL) {
    printf("Finded Element is:\n ");
    DebugOutOfNameTableElement(pElement, stdout);
  } else {
    printf("No element using \"id_int_1000\" name\n");
  }

  // pIntContext = (struct Context.Const.Int*)pContext;
  // DebugOutOfContext<(Context*)pIntContext>(stdout);
  exit(EXIT_SUCCESS);
}
