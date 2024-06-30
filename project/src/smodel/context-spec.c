// context-spec.c
// Определения функций, обеспечивающих обработку контекста.

#include <string.h>
#include "constant-spec.h"
#include "type-spec.h"
#include "var-spec.h"
#include "context-spec.h"

//==============================================================================
// Обобщенные функции для обработки контекста
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода содержимого контекста
void DebugOutOfContext<struct Context* context>(FILE* file) {
  fprintf(file, "Unknown context\n");
}

//==============================================================================
// Функции для обработки контекста константы
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода значения контекста константы
void DebugOutOfContext<struct Context<constant>* context>(FILE* file) {
  printf("Check point: CONST context ---> ");
  // struct Context<constant>* pContext = &context->@;
  // DebugOutOfConstant<pContext>(file);
  DebugOutOfConstant<&(context->@)>(file);
}

//------------------------------------------------------------------------------
// Функция, создающая контекст для именованной целочисленной константы
Context* CreateContextConstInt(int value) {
  struct Context<constant<integer> > *context =
          create_spec(Context<constant<integer> >);
  context->@.@constValue = value;
  return (Context*)context;
}

//==============================================================================
// Функции для обработки контекста типа
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода значения контекста типа
void DebugOutOfContext<Context<type>* context>(FILE* file) {
  printf("Check point: TYPE context ---> ");
  DebugOutOfType<&(context->@)>(file);
}

//------------------------------------------------------------------------------
// Функция, создающая контекст для целочисленного типа
Context* CreateContextTypeInt() {
  struct Context<type<integer> > *context =
          create_spec(Context<type<integer> >);
  context->@.size = sizeof(int);
  return (Context*)context;
}

//==============================================================================
// Функции для обработки контекста переменной
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода значения контекста переменной
void DebugOutOfContext<Context<var>* context>(FILE* file) {
  printf("Check point: VAR context ---> ");
  DebugOutOfVar<&(context->@)>(file);
}

//------------------------------------------------------------------------------
// Функция, создающая контекст для целочисленной переменной
Context* CreateContextVarInt(Type* pType, int value) {
  struct Context<var<integer> > *context =
          create_spec(Context<var<integer> >);
  context->@.pType = pType;
  context->@.@value = value;
  return (Context*)context;
}

