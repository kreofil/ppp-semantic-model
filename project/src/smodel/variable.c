// Определения функций, обеспечивающих обработку переменных.

#include "variable.h"

//==============================================================================
// Функции для обработки переменной
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода значения переменной с неизвестным типом памяти
void DebugOutOfVariable<struct Variable* variable>(FILE* file) {
  fprintf(file, "Variable. Unknown memory type\n");
}

//------------------------------------------------------------------------------
// Функция вывода значения для глобальной памяти
void DebugOutOfVariable<struct Variable.Global* variable>(FILE* file) {
  fprintf(file, "Global memory. Address = %d\n", variable->memAddr);
}

//------------------------------------------------------------------------------
// Функция вывода значения для внешней памяти
void DebugOutOfVariable<struct Variable.External* variable>(FILE* file) {
  fprintf(file, "External memory. Table link = %d\n", variable->memAddr);
}

//------------------------------------------------------------------------------
// Функция вывода значения для статической памяти
void DebugOutOfVariable<struct Variable.Static* variable>(FILE* file) {
  fprintf(file, "Static memory. Address = %d\n", variable->memAddr);
}

//------------------------------------------------------------------------------
// Функция вывода значения для локльной (стековой) памяти
void DebugOutOfVariable<struct Variable.Local* variable>(FILE* file) {
  fprintf(file, "Local memory. Address = %d\n", variable->memAddr);
}

//------------------------------------------------------------------------------
// Функция вывода значения для временной памяти
void DebugOutOfVariable<struct Variable.Temp* variable>(FILE* file) {
  fprintf(file, "Temporary memory. Address = %d\n", variable->memAddr);
}

//------------------------------------------------------------------------------
// Функция вывода всех параметров переменной
void DebugOutOfVariableCommon(Variable* variable, FILE* file) {
  fprintf(file, "Variable: type = ");
  DebugOutOfType<variable->varType>(file);
  fprintf(file, "    ");
  DebugOutOfVariable<variable>(file);
  fprintf(file, "    Value. ");
  if(variable->varValue != NULL){
    DebugOutOfConstant<variable->varValue>(file);
  } else {
    fprintf(file, "Unknown\n");
  }

}

//------------------------------------------------------------------------------
// Установка типа и значения переменной
void SetVariable(Variable* variable, Type* varType, Constant* varValue) {
  variable->varType = varType;
  variable->varValue = varValue;
}
//------------------------------------------------------------------------------
// Создание переменной с одним из вариантов памяти

// Создание глобальной переменной
Variable* CreateVariableGlobal(Type* varType, Constant* varValue) {
  struct Variable.Global* variable = create_spec(Variable.Global);
  SetVariable((Variable*)variable, (Type*)varType, (Constant*)varValue);
  // Формирование текущего местоположения в данном типе памяти
  // ... пока:
  variable->memAddr = 0;
  return (Variable*)variable;
}

// Создание внешней переменной
Variable* CreateVariableExternal(Type* varType, Constant* varValue) {
  struct Variable.External* variable = create_spec(Variable.External);
  SetVariable((Variable*)variable, (Type*)varType, (Constant*)varValue);
  // Формирование текущего местоположения в данном типе памяти
  // ... пока:
  variable->memAddr = 0;
  return (Variable*)variable;
}

// Создание статической переменной
Variable* CreateVariableStatic(Type* varType, Constant* varValue) {
  struct Variable.Static* variable = create_spec(Variable.Static);
  SetVariable((Variable*)variable, (Type*)varType, (Constant*)varValue);
  // Формирование текущего местоположения в данном типе памяти
  // ... пока:
  variable->memAddr = 0;
  return (Variable*)variable;
}

// Создание локальной переменной
Variable* CreateVariableLocal(Type* varType, Constant* varValue) {
  struct Variable.Local* variable = create_spec(Variable.Local);
  SetVariable((Variable*)variable, (Type*)varType, (Constant*)varValue);
  // Формирование текущего местоположения в данном типе памяти
  // ... пока:
  variable->memAddr = 0;
  return (Variable*)variable;
}

// Создание временной переменной
Variable* CreateVariableTemp(Type* varType, Constant* varValue) {
  struct Variable.Temp* variable = create_spec(Variable.Temp);
  SetVariable((Variable*)variable, (Type*)varType, (Constant*)varValue);
  // Формирование текущего местоположения в данном типе памяти
  // ... пока:
  variable->memAddr = 0;
  return (Variable*)variable;
}
