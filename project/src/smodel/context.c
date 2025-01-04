// context-spec.c
// Определения функций, обеспечивающих обработку контекста.

#include <string.h>
// #include "constant.h"
#include "context.h"

//==============================================================================
// Обобщенные функции для обработки контекста
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода содержимого контекста
void DebugOutOfContext<Context* context>(FILE* file) {
  fprintf(file, "Unknown context\n");
}

//==============================================================================
// Функции для обработки контекста константы
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода значения контекста константы
void DebugOutOfContext<Context.constant* context>(FILE* file) {
  printf("Check point: constant context ---> ");
  // struct Context<constant>* pContext = &context->@;
  // DebugOutOfConstant<pContext>(file);
  DebugOutOfConstant<&(context->@)>(file);
}

//------------------------------------------------------------------------------
// Функция, создающая контекст для именованной целочисленной константы
Context* CreateContextConstInt(int value) {
  struct Context.constant.Int *context =
          create_spec(Context.constant.Int);
  context->@.@constValue = value;
  return (Context*)context;
}
