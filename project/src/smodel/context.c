// context-spec.c
// Определения функций, обеспечивающих обработку контекста.

#include <string.h>
#include "context.h"

//==============================================================================
// Обобщенные функции для обработки контекста
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода содержимого контекста
void DebugOutOfContext<Context* context>(FILE* file) {
  // fprintf(file, L"Context:Unknown context\n");
  fprintf(file, "Context:Unknown context\n");
}

//==============================================================================
// Функции для обработки контекста константы
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода значения контекста константы
void DebugOutOfContext<Context.Const* context>(FILE* file) {
  printf("Context.Const: ");
  // DebugOutOfConstant<context->@>(file);
  Constant* c = context->@;
  DebugOutOfConstant<c>(file);
}

//------------------------------------------------------------------------------
// Функция, создающая контекст для именованной целочисленной константы
Context* CreateContextConstInt(int value) {
  struct Context.Const *context = create_spec(Context.Const);
  struct Constant.Int *constant = create_spec(Constant.Int);
  constant->@constValue = value;
  context->@ = constant;
  return (Context*)context;
}

//==============================================================================
// Функции для обработки контекста переменной
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода контекста переменной
void DebugOutOfContext<Context.Var* context>(FILE* file) {
  Variable* v = context->@;
  DebugOutOfVariableCommon(v, file);
}

//------------------------------------------------------------------------------
// Функция создающая контекст для любой переменной,
// подключаемой к этому контексту
Context* CreateContextVar(Variable* variable) {
  struct Context.Var *context = create_spec(Context.Var);
  context->@ = variable;
  return (Context*)context;
}