// Реализация функций, осуществляющих создание инструкций, проверку корректности
// их операндов и формирующих при необходимости корректный выходной операнд
// Формирование инструкций, содержащих 3 и более операндов.

#include <stdlib.h>
#include "instruction.h"
// #include "global.h"

//==============================================================================
// Функции, создающие трехоперандные инструкции
//==============================================================================

//------------------------------------------------------------------------------
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Проверка констант на допустимость. Они должны быть одного типа
// Обобщенная функция проверки двух констант.
static _Bool isCorrectConstantsOfAdd
              <Constant* constant1, Constant* constant2>(Operand** dst) {
  return 0;
}
// Две целочисленные константы
static _Bool isCorrectConstantsOfAdd
            <Constant.Int* constant1, Constant.Int* constant2>(Operand** dst) {
  Constant* cResult =
            CreateConstantInt(constant1->@constValue + constant2->@constValue);
  Operand* newDst = CreateOperandConstant(cResult);
  DebugOutOfOperand<newDst>(stdout);
  *dst = newDst;
  return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Проверка комбинации: константа - переменная
static _Bool isCorrectConstVarOfAdd
              <Constant* constant1, Type* varType2>(Operand** dst) {
  return 0;
}
// Две целочисленная константа и целочисленная переменная
static _Bool isCorrectConstVarOfAdd
              <Constant.Int* constant1, Type.Int* varType2>(Operand** dst) {
  // Формирование временной переменной как результата сложения
  Variable* tmpVar = CreateVariableTemp((Type*)varType2, NULL);
  Operand* newDst = CreateOperandVariable(tmpVar);
  *dst = newDst;
  return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Проверка комбинации: переменная - константа
static _Bool isCorrectVarConstOfAdd
              <Type* varType1, Constant* constant2>(Operand** dst) {
  return 0;
}
// Две целочисленная константа и целочисленная переменная
static _Bool isCorrectVarConstOfAdd
              <Type.Int* varType1, Constant.Int* constant2>(Operand** dst) {
  // Формирование временной переменной как результата сложения
  Variable* tmpVar = CreateVariableTemp((Type*)varType1, NULL);
  Operand* newDst = CreateOperandVariable(tmpVar);
  *dst = newDst;
  return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Проверка комбинации для двух переменных
static _Bool isCorrectVarVarOfAdd
              <Type* varType1, Type* varType2>(Operand** dst) {
  return 0;
}
// Две целочисленные переменные
static _Bool isCorrectVarVarOfAdd
              <Type.Int* varType1, Type.Int* varType2>(Operand** dst) {
  // Формирование временной переменной как результата сложения
  Variable* tmpVar = CreateVariableTemp((Type*)varType1, NULL);
  Operand* newDst = CreateOperandVariable(tmpVar);
  *dst = newDst;
  return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Проверка операндов на допустимость. Обработчик обобщения
// Пока ориентирован на Add. Но в перспективе возможны и другие команды
// с той же комбинацией операндов.
static _Bool isCorrectOperandsOfAdd
               <Operand* src1, Operand* src2>(Operand** dst) {
  return 0;
}
// Проверка, что оба операнда - константы
static _Bool isCorrectOperandsOfAdd
                <Operand.Const* src1, Operand.Const* src2>(Operand** dst) {
  Constant* constant1 = src1->@;
  Constant* constant2 = src2->@;
  return isCorrectConstantsOfAdd<constant1, constant2>(dst);
}
// Проверка, что первый операнд - константа, второй - переменная
static _Bool isCorrectOperandsOfAdd
                <Operand.Const* src1, Operand.Var* src2>(Operand** dst) {
  Constant* constant1 = src1->@;
  Variable* variable2 = src2->@;
  Type* varType2 = variable2->varType;
  return isCorrectConstVarOfAdd<constant1, varType2>(dst);
}
// Проверка, что первый операнд - переменная, второй - константа
static _Bool isCorrectOperandsOfAdd
                <Operand.Var* src1, Operand.Const* src2>(Operand** dst) {
  Variable* variable1 = src1->@;
  Type* varType1 = variable1->varType;
  Constant* constant2 = src2->@;
  return isCorrectVarConstOfAdd<varType1, constant2>(dst);
}
// Проверка, что оба операнда - переменные
static _Bool isCorrectOperandsOfAdd
                <Operand.Var* src1, Operand.Var* src2>(Operand** dst) {
  Variable* variable1 = src1->@;
  Type* varType1 = variable1->varType;
  Variable* variable2 = src2->@;
  Type* varType2 = variable2->varType;
  return isCorrectVarVarOfAdd<varType1, varType2>(dst);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Создание инструкции сложения
Instruction* CreateInstructionAdd(Operand* src1, Operand* src2) {
  Operand* dst = NULL; // Возможно лучше присвоить в начале Unknown
  struct Instruction.Add *instruction = create_spec(Instruction.Add);
  instruction->next = NULL;
  // Проверка на корректность входных операндов инструкции Add
  if(!isCorrectOperandsOfAdd<src1, src2>(&dst)) {
    fprintf(stderr, "Incorrect Source Operands of Add Instruction!!!\n");
    DebugOutOfOperand<src1>(stderr);
    DebugOutOfOperand<src2>(stderr);
    // exit(14);
  }
  instruction->@opd1 = src1;
  instruction->@opd2 = src2;
  instruction->@opd0 = dst;
  return (Instruction*)instruction;
}
