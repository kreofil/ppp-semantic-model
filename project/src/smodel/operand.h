#ifndef __operand__
#define __operand__
// operand.h
// Объявление операнда, используемого в семантической модели
// в качестве аргументов и результатов операций (команд)
// Функции, осуществляющие работу с операндами

#include <stdio.h>
#include "variable.h"

//==============================================================================
// Обобщенный операнд
typedef struct Operand {}<> Operand;
//------------------------------------------------------------------------------
// Специализации операнда
Operand + <Var: Variable*;>;    // Операнд является переменной
Operand + <Const: Constant*;>;  // Операнд является константой

//------------------------------------------------------------------------------
// Функции для работы с операндом

// Обобщенная функция вывода параметров операндов
void DebugOutOfOperand<struct Operand* operand>(FILE* file);

// Создание операнда - переменной
Operand* CreateOperandVariable(Variable* variable);
// Создание операнда - константы
Operand* CreateOperandConstant(Constant* constant);

#endif // __operand__
