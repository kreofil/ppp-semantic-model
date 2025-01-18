#ifndef __global__
#define __global__
// global.h
// Объявление внешних переменных, используемых в семантической модели.

#include "operand.h"
#include "name-table.h"

// Обращение к имени "неименованного" контекста для получения ссылки на него
char* GetUnknownName();

// Обращение к целочисленному типу
Type* GetTypeInt();

// Обращение к нулевой целочисленной константе
Constant* GetConstIntZero();

// Обращение к фиктивному операнду для получения его указателя
Operand* GetUnknownOperand();

// Получение указателя на таблицу зарезервированных имен
NameTable* GetReservedNameTable();

// Получение указателя на таблицу глобальных имен
NameTable* GetGlobalNameTable();

#endif // __global__
