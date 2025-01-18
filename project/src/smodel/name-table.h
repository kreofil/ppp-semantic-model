#ifndef __name_table__
#define __name_table__
// name-table.h
// Объявление таблицы имен и ее элемента,
// а также прототипов функций, обеспечивающих обработку

#include <stdio.h>
#include <wchar.h>
#include "context.h"

//------------------------------------------------------------------------------
// Максимальный размер имени контекста
#define maxNameLength 32

// Элемент таблицы имен
typedef struct NameTableElement {
  wchar_t name[maxNameLength];   // Имя элемента таблицы имен
  Context *pContext;      // Указатель на контекст элемента
} NameTableElement;

//------------------------------------------------------------------------------
// Таблица имен. В текщей версии задается массивом
// Максимальный размер таблицы имен
#define maxTableLength 256

// Таблица имен на основе одномерного массива
typedef struct NameTable{
  int size;                  // Размер таблицы имен
  NameTableElement element[maxTableLength]; // Имя элемента таблицы имен
} NameTable;


//==============================================================================
// Функции для работы с таблицей имен
//==============================================================================

// Функция вывода элемента таблицы имен
void DebugOutOfNameTableElement(NameTableElement* pElement, FILE* file);

// Функция сравнения имени элемента с образцом
_Bool IsElementEqual(NameTableElement* pElement, wchar_t* samle);

// Функция инициализации таблицы имен
void InitNameTable(NameTable* nameTable);

// Добавление элемента в таблицу имен
void AddElementToNameTable(NameTable* nameTable, wchar_t *name, Context *context);

// Функция вывода содержимого контекста
void DebugOutOfNameTable(NameTable* nameTable, FILE* file);

// Функция поиска элемента в таблице имен по образцу
NameTableElement* findElementInTable(NameTable* nameTable, wchar_t* sample);

#endif // __name_table__
