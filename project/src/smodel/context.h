#ifndef __context__
#define __context__
// context.h
// Объявление обобщенного контекста, используемого в семантической модели,
// а также прототипов обобщенных функций, обеспечивающих его обработку

#include <stdio.h>
#include "constant.h"

//------------------------------------------------------------------------------
// Максимальный размер имени контекста
#define maxLength 32

// Обобщенный контекст
typedef struct Context {}<> Context;

//==============================================================================
// Функции для работы с контекстом

// Обобщенная функция вывода содержимого контекста
void DebugOutOfContext<struct Context* context>(FILE* file);

//==============================================================================
// Специализация контекста константы
Context + <constant: Constant;>;

//------------------------------------------------------------------------------
// Функция вывода значения контекста константы
void DebugOutOfContext<Context.constant* context>(FILE* file);

// Функция, создающая контекст для именованной целочисленной константы
Context* CreateContextConstInt(int value);

#endif // __context__
