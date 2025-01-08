#ifndef __global__
#define __global__
// global.h
// Объявление внешних переменных, используемых в семантической модели.

#include "operand.h"

// Обращение к целочисленному типу
Type* GetTypeInt();

// Обращение к нулевой целочисленной константе
Constant* GetConstIntZero();

// Обращение к фиктивному операнду для получения его указателя
Operand* GetUnknownOperand();

#endif // __global__
