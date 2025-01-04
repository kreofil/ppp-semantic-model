// Определения функций, обеспечивающих обработку констант.

#include "constant.h"

//==============================================================================
// Обобщенные функции для обработки константы
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода значения неизвестной константы
void DebugOutOfConstant<struct Constant* constant>(FILE* file) {
  fprintf(file, "Unknown constant\n");
}

//==============================================================================
// Функции для обработки целочисленной константы
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода значения целочисленной константы
void DebugOutOfConstant<struct Constant.Int *constant>(FILE* file) {
  // printf("Check point: const int ---> ");
  fprintf(file, "const int = %d\n", constant->@constValue);
}

//------------------------------------------------------------------------------
// Создание целочисленной константы в динамической памяти
Constant* CreateConstantInt(int constValue) {
  struct Constant.Int *constInt = create_spec(Constant.Int);
  constInt->@constValue = constValue;
  return (Constant*)constInt;
}

//------------------------------------------------------------------------------
// Получение значения целочисленной константы
int GetConstantInt(struct Constant.Int *constInt) {
  return constInt->@constValue;
}

// Установка значения целочисленной константы
void SetConstantInt(struct Constant.Int *constInt, int constValue) {
  constInt->@constValue = constValue;
}
