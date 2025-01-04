// Определения функций, обеспечивающих обработку операндов.

#include "operand.h"

//==============================================================================
// Функции для обработки операндов
//==============================================================================

//------------------------------------------------------------------------------
// Отладочная функция вывода информации о неизвестном операнде
void DebugOutOfOperand<struct Operand* operand>(FILE* file) {
  fprintf(file, "Operand. Unknown operand context\n");
}

//------------------------------------------------------------------------------
// Отладочная функция вывода информации об операнде - переменной
void DebugOutOfOperand<struct Operand.Var* operand>(FILE* file) {
  fprintf(file, "Operand is Variable. ");
  Variable* v = operand->@;
  DebugOutOfVariableCommon(v, file);
}

//------------------------------------------------------------------------------
// Отладочная функция вывода информации об операнде - константе
void DebugOutOfOperand<struct Operand.Const* operand>(FILE* file) {
  fprintf(file, "Operand is Constant. ");
  Constant* c = operand->@;
  DebugOutOfConstant<c>(file);
}

// Создание операнда - переменной
Operand* CreateOperandVariable(Variable* variable) {
  struct Operand.Var* operand = create_spec(Operand.Var);
  operand->@ = variable;
  return (Operand*)operand;
}

// Создание операнда - константы
Operand* CreateOperandConstant(Constant* constant) {
  struct Operand.Const* operand = create_spec(Operand.Const);
  operand->@ = constant;
  return (Operand*)operand;
}
