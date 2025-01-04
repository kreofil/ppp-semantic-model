#ifndef __variable__
#define __variable__
// variable.h
// Объявление переменной, используемой в семантической модели,
// а также прототипов функций, обеспечивающих ее обработку

#include <stdio.h>
#include "constant.h"
#include "type.h"

//==============================================================================
// Обобщенная переменная
typedef struct Variable {
  int       memAddr;    // Вид памяти, выделяемой для переменной
  Type      *varType;   // Тип переменной
  Constant  *varValue;  // Значение переменной
}<> Variable;
//------------------------------------------------------------------------------
// Специализации переменной, определяемые используемой памятью
Variable + <Global:   void;>;  // Глобальная память
Variable + <External: void;>;  // Внешняя память (другой модуль)
Variable + <Static:   void;>;  // Статическая память
Variable + <Local:    void;>;  // Локальная память (на стеке)
Variable + <Temp:     void;>;  // Временная память (например, внутри выражений)

//------------------------------------------------------------------------------
// Функции для работы с переменной

// Обобщенная функция вывода значения переменной
void DebugOutOfVariable<Variable* variable>(FILE* file);
// Функция вывода всех параметров переменной
void DebugOutOfVariableCommon(Variable* variable, FILE* file);

// Установка типа и значения переменной
void SetVariable(Variable* variable, Type* varType, Constant* varValue);

// Создание глобальной переменной
Variable* CreateVariableGlobal(Type* varType, Constant* varValue);
// Создание внешней переменной
Variable* CreateVariableExternal(Type* varType, Constant* varValue);
// Создание статической переменной
Variable* CreateVariableStatic(Type* varType, Constant* varValue);
// Создание локальной переменной
Variable* CreateVariableLocal(Type* varType, Constant* varValue);
// Создание временной переменной
Variable* CreateVariableTemp(Type* varType, Constant* varValue);

#endif // __variable__
