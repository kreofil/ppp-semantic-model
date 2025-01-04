#ifndef __constant__
#define __constant__
// constant.h
// Объявление обобщенной константы, используемой в семантической модели,
// а также прототипов обобщенных функций, обеспечивающих ее обработку

#include <stdio.h>

//------------------------------------------------------------------------------
// Обобщенная константа
typedef struct Constant {}<> Constant;

//==============================================================================
// Функции для работы со значением

// Обобщенная функция вывода значения константы
void DebugOutOfConstant<struct Constant* constant>(FILE* file);

//==============================================================================
// Основа целочисленной константы
typedef struct ConstantInt {int constValue;} ConstantInt;
// Специализация целочисленной константы
Constant + <Int: ConstantInt;>;

//------------------------------------------------------------------------------
// Функция вывода значения целочисленной константы
void DebugOutOfConstant<Constant.Int* constant>(FILE* file);

// Создание целочисленной константы в динамической памяти
Constant* CreateConstantInt(int constValue);

// Получение значения целочисленной константы
int GetConstantInt(struct Constant.Int *constInt);

// Установка значения целочисленной константы
void SetConstantInt(struct Constant.Int *constInt, int constValue);

#endif // __constant__
