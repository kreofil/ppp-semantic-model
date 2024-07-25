// Определения функций, обеспечивающих обработку переменных.

#include "type-spec.h"
#include "var-spec.h"

//==============================================================================
// Обобщенные функции для обработки переменной
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода значения целочисленной переменной
void DebugOutOfVar<struct Var* type>(FILE* file) {
  fprintf(file, "Unknown variable\n");
}

//==============================================================================
// Функции для обработки целочисленной переменной
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода значения целочисленной переменной
void DebugOutOfVar<Var.integer* pVar>(FILE* file) {
  printf("Check point: var int ---> ");
  fprintf(file, "var int, value = %d\n       ", pVar->@value);
  DebugOutOfType<pVar->pType>(file);
}
