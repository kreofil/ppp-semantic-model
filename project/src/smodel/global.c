// Определение внешних переменных, используемых в семантической модели

#include <stdio.h>
#include "global.h"

//==============================================================================
// Описание базовых типов данных, которые формируются только в одном экземпляре
//==============================================================================

//------------------------------------------------------------------------------
// Имя для неименованного контекста (вместо пустого указателя)
static char* unknownName = "!!!unknown!!!";

// Обращение к имени "неименованного" контекста для получения ссылки на него
char* GetUnknownName() {
  return unknownName;
}

//------------------------------------------------------------------------------
// Целочисленный тип
static struct Type.Int typeInt = {sizeof(int), 2};

// Обращение к целочисленному типу
Type* GetTypeInt() {
  return (Type*)&typeInt;
}

//------------------------------------------------------------------------------
// Нулевая целочисленная константа
static struct Constant.Int constIntZero;

// Обращение к нулевой целочисленной константе
Constant* GetConstIntZero() {
  constIntZero.@constValue = 0;
  return (Constant*)&constIntZero;
}

//==============================================================================
// Описание типовых констант, которые формируются только в одном экземпляре
//==============================================================================

//==============================================================================
// Описание операндов, которые формируются только в одном экземпляре
//==============================================================================

//------------------------------------------------------------------------------
// Фиктивный операнд - заглушка при невозможности определить его параметры
static struct Operand unknownOperand;

// Обращение к фиктивному операнду для получения его указателя
Operand* GetUnknownOperand() {
  return &unknownOperand;
}

//------------------------------------------------------------------------------

//==============================================================================
// Описание внешних таблиц имен, которые формируются только в одном экземпляре
//==============================================================================

//------------------------------------------------------------------------------
// Таблица зарезервированных имен. Их нельзя использовать в другом контексте
static NameTable reservedNameTable;

// Получение указателя на таблицу зарезервированных имен
NameTable* GetReservedNameTable() {
  return &reservedNameTable;
}

//------------------------------------------------------------------------------
// Таблица имен глобальных имен, формируемых в программе
static NameTable globalNameTable;

// Получение указателя на таблицу глобальных имен
NameTable* GetGlobalNameTable() {
  return &globalNameTable;
}