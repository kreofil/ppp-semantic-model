#ifndef __context_spec__
#define __context_spec__
// context-spec.h
// Объявления контекстов - основ специализаций и построенных из них специализаций,
// а также прототипов обобщенных функций, обеспечивающих их обработку

// #include <stdio.h>
#include "constant.h"
#include "type.h"
#include "var.h"
#include "context.h"

//==============================================================================
// Специализация контекста константы
Context + <constant: Constant;>;
// Специализация контекста типа
Context + <type: Type;>;
// Специализация контекста переменной
Context + <var: Var;>;


//------------------------------------------------------------------------------
// Функция вывода значения контекста константы
void DebugOutOfContext<Context.constant* context>(FILE* file);

// Функция, создающая контекст для целочисленной константы
Context* CreateContextConstInt(int value);

//------------------------------------------------------------------------------
// Функция вывода значения контекста типа
void DebugOutOfContext<Context.type* context>(FILE* file);

// Функция, создающая контекст для целочисленного типа
Context* CreateContextTypeInt();

//------------------------------------------------------------------------------
// Функция вывода значения контекста переменной
void DebugOutOfContext<Context.var* context>(FILE* file);

// Функция, создающая контекст для целочисленной переменной
Context* CreateContextVarInt(Type* pType, int value);

#endif // __context_spec__
