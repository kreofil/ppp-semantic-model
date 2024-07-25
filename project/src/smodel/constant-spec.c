// Определения функций, обеспечивающих обработку констант.

#include "constant-spec.h"

//==============================================================================
// Обобщенные функции для обработки константы
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода значения целочисленной константы
void DebugOutOfConstant<Constant* constant>(FILE* file) {
  fprintf(file, "Unknown constant\n");
}

//==============================================================================
// Функции для обработки целочисленной константы
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода значения целочисленной константы
void DebugOutOfConstant<Constant.integer* constant>(FILE* file) {
  printf("Check point: const int ---> ");
  fprintf(file, "const int = %d\n", constant->@constValue);
}
