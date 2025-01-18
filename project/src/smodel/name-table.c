// name-table.c

#include <string.h>
#include "name-table.h"

//==============================================================================
// Определения функций обработки таблицы имен
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода элемента таблицы имен
void DebugOutOfNameTableElement(NameTableElement* pElement, FILE* file) {
  fprintf(file, "%ls\n    ---> ", pElement->name);
  DebugOutOfContext< pElement->pContext >(file);
}

//------------------------------------------------------------------------------
// Функция сравнения имени элемента с образцом
_Bool IsElementEqual(NameTableElement* pElement, wchar_t* samle) {
  // if(!strcmp(pElement->name, samle)) {
  if(!wcscmp(pElement->name, samle)) {
    return 1;
  }
  return 0;
}

//------------------------------------------------------------------------------
// Функция инициализации таблицы имен
void InitNameTable(NameTable* nameTable) {
  nameTable->size = 0;
}

//------------------------------------------------------------------------------
// Добавление элемента в таблицу имен
void AddElementToNameTable(NameTable* nameTable, wchar_t *name, Context *context) {
  wcscpy(nameTable->element[nameTable->size].name, name);
  // strcpy(nameTable->element[nameTable->size].name, name);
  nameTable->element[nameTable->size].pContext = context;
  ++(nameTable->size);
}

//------------------------------------------------------------------------------
// Функция вывода таблицы имен
void DebugOutOfNameTable(NameTable* nameTable, FILE* file) {
  fprintf(file, "Name table. size = %d; max size = %d\n",
          nameTable->size, maxTableLength);
  for(int i = 0; i < nameTable->size; ++i) {
    fprintf(file, "%d: ", i);
    DebugOutOfNameTableElement(&(nameTable->element[i]), file);
    // DebugOutOfContext< nameTable->element[i].pContext >(file);
    // fprintf(file, "\n");
  }
}

//------------------------------------------------------------------------------
// Функция поиска элемента в таблице имен по образцу
NameTableElement* findElementInTable(NameTable* nameTable, wchar_t* sample) {
  for(int i = 0; i < nameTable->size; ++i) {
    if(IsElementEqual(&(nameTable->element[i]), sample)) {
      // DebugOutOfNameTableElement(&(nameTable->element[i]), stdout);
      return &(nameTable->element[i]);
    }
  }
  // printf("No this name in Table\n");
  return NULL;
}
