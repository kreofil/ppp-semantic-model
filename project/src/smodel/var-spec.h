// var-spec.h
#ifndef __var_spec__
#define __var_spec__
// var-spec.h
// Объявления переменных - основ специализаций
// и построенных из них специализаций,
// а также прототипов обобщенных функций, обеспечивающих их обработку

#include "var.h"

//==============================================================================
// Основа целочисленной переменной
typedef struct VarInt {int value;} VarInt;
// Специализация целочисленной переменной. Определяется через перечисление
Var + <integer: VarInt;>;

//------------------------------------------------------------------------------
// Функция вывода значения целочисленной переменной
void DebugOutOfVar<Var<integer>* pVar>(FILE* file);

#endif // __var_spec__
