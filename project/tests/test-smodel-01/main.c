// main.c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>

#include "constant-spec.h"
#include "type-spec.h"
#include "context-spec.h"
#include "name-table.h"

struct Constant<integer> intConstExt;
// struct Context<constant<integer> > intConstContextExt;

int main(int argc, char *argv[]) {
  printf("Start\n");

  NameTable nameTable;
  struct Context<constant<integer> > *pIntContext;
  Context *pContext;

  InitNameTable(&nameTable);
  DebugOutOfNameTable(&nameTable, stdout);

  pContext = CreateContextConstInt(10);
  // DebugOutOfContext<pContext>(stdout);
  AddElementToNameTable(&nameTable, "id_int_10", pContext);
  DebugOutOfNameTable(&nameTable, stdout);

  pContext = CreateContextConstInt(20);
  // DebugOutOfContext<pContext>(stdout);
  AddElementToNameTable(&nameTable, "id_int_20", pContext);
  DebugOutOfNameTable(&nameTable, stdout);

  pContext = CreateContextConstInt(30);
  // DebugOutOfContext<pContext>(stdout);
  AddElementToNameTable(&nameTable, "id_int_30", pContext);
  DebugOutOfNameTable(&nameTable, stdout);

  pContext = CreateContextConstInt(40);
  // DebugOutOfContext<pContext>(stdout);
  AddElementToNameTable(&nameTable, "id_int_40", pContext);
  DebugOutOfNameTable(&nameTable, stdout);

  pContext = CreateContextConstInt(50);
  // DebugOutOfContext<pContext>(stdout);
  AddElementToNameTable(&nameTable, "id_int_50", pContext);
  DebugOutOfNameTable(&nameTable, stdout);

  pContext = CreateContextTypeInt();
  // DebugOutOfContext<pContext>(stdout);
  AddElementToNameTable(&nameTable, "id_type_60", pContext);
  DebugOutOfNameTable(&nameTable, stdout);

  NameTableElement *pElement = findElementInTable(&nameTable, "id_int_40");
  if(pElement != NULL) {
    printf("Finded Element is:\n ");
    DebugOutOfNameTableElement(pElement, stdout);
  } else {
    printf("No element using \"id_int_40\" name");
  }

  if((pElement = findElementInTable(&nameTable, "id_int_10")) != NULL) {
    printf("Finded Element is:\n ");
    DebugOutOfNameTableElement(pElement, stdout);
  } else {
    printf("No element using \"id_int_10\" name");
  }

  if((pElement = findElementInTable(&nameTable, "id_int_1000")) != NULL) {
    printf("Finded Element is:\n ");
    DebugOutOfNameTableElement(pElement, stdout);
  } else {
    printf("No element using \"id_int_1000\" name\n");
  }

  if((pElement = findElementInTable(&nameTable, "id_type_60")) != NULL) {
    printf("Finded Element is:\n ");
    DebugOutOfNameTableElement(pElement, stdout);
  } else {
    printf("No element using \"id_type_60\" name\n");
  }

  // pIntContext = (Context<constant<integer> >*)pContext;
  pIntContext = pContext;
  DebugOutOfContext<pIntContext>(stdout);

  pContext = CreateContextTypeInt();
  // DebugOutOfContext<pContext>(stdout);
  AddElementToNameTable(&nameTable, "id_type_110", pContext);
  DebugOutOfNameTable(&nameTable, stdout);

  // Формирование контеста переменно и ее типа
  struct Type<integer> > *pType = create_spec(Type<integer>);
  pType->size = sizeof(int);
  Context *pVarContext = CreateContextVarInt(pType, 210);
  AddElementToNameTable(&nameTable, "id_var_210", pVarContext);
  DebugOutOfNameTable(&nameTable, stdout);

  exit(EXIT_SUCCESS);
}
