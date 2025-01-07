// Реализация функций, осуществляющих создание инструкций, проверку корректности
// их операндов и формирующих при необходимости корректный выходной операнд

#include <stdlib.h>
#include "instruction.h"

//==============================================================================
// Функции, создающие безоперандные инструкции
//==============================================================================

//------------------------------------------------------------------------------
// Создание инструкции Halt
Instruction* CreateInstructionHalt() {
  struct Instruction.Halt* instruction = create_spec(Instruction.Halt);
  instruction->next = NULL;
  return (Instruction*)instruction;
}

//==============================================================================
// Функции, создающие однооперандные инструкции
//==============================================================================

//------------------------------------------------------------------------------
// Создание и проверка инструкции Exit
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Проверка на корректность константы в команде Exit
static _Bool isCorrectConstantOfExit<Constant* constant>() {
  return 0;
}
// Проверка на целочисленную константу в команде Exit
static _Bool isCorrectConstantOfExit<Constant.Int* constant>() {
  return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Проверка на корректность типа переменной в команде Exit
static _Bool isCorrectVarTypeOfExit<Type* varType>() {
  return 0;
}
// Проверкак на целочисленный тип
static _Bool isCorrectVarTypeOfExit<Type.Int* varType>() {
  return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Проверка операнда на допустимость. Обработчик обобщения
static _Bool isCorrectOperandOfExit<Operand* opd>() {
  // По умолчанию некорректный операнд
  // fprintf(stderr, "--- inCorrect Operand Of Exit\n");
  return 0;
}
// Проверка операнда - целочисленной константы
static _Bool isCorrectOperandOfExit<Operand.Const* opd>() {
  // fprintf(stderr, "+++ Correct Constant Of Exit\n");
  Constant* constant = opd->@;
  return isCorrectConstantOfExit<constant>();
}
// Проверка операнда - переменной
static _Bool isCorrectOperandOfExit<Operand.Var* opd>() {
  // fprintf(stderr, "+++ Correct Variable Of Exit\n");
  Variable* variable = opd->@;
  Type* varType = variable->varType;
  return isCorrectVarTypeOfExit<varType>();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Создание инструкции Exit
Instruction* CreateInstructionExit(Operand* opd) {
  struct Instruction.Exit* instruction = create_spec(Instruction.Exit);
  instruction->next = NULL;
  // Проверка на корректность входного операнда инструкции Exit
  if(!isCorrectOperandOfExit<opd>()) {
    fprintf(stderr, "Incorrect Operand of Exit Instruction!!!\n");
    DebugOutOfOperand<opd>(stderr);
    // exit(14);
  }
  // fprintf(stderr, "!flag = %d\n", !flag);
  instruction->@opd = opd;
  return (Instruction*)instruction;
}

//==============================================================================
// Функции, создающие двухоперандные инструкции
//==============================================================================

//------------------------------------------------------------------------------
// Проверка на корректность константы в команде Minus
static _Bool isCorrectConstantOfMinus<Constant* constant>(Operand** dst) {
  return 0;
}
// Проверка на целочисленную константу в команде Minus
static _Bool isCorrectConstantOfMinus<Constant.Int* constant>(Operand** dst) {
  // fprintf(stderr, "--- inCorrect Operand Of Minus\n");
  Constant* c = CreateConstantInt(-(constant->@constValue));
  Operand* newDst = CreateOperandConstant(c);
  DebugOutOfOperand<newDst>(stdout);
  *dst = newDst;
  return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Проверка на корректность типа переменной в команде Minus
static _Bool isCorrectVarTypeOfMinus<Type* varType>(Operand** dst) {
  return 0;
}
// Проверка на целочисленный тип
static _Bool isCorrectVarTypeOfMinus<Type.Int* varType>(Operand** dst) {
  // Формирование временной переменной как результата вычитания
  Variable* tmpVar = CreateVariableTemp((Type*)varType, NULL);
  Operand* newDst = CreateOperandVariable(tmpVar);
  *dst = newDst;
  return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Проверка операнда на допустимость. Обработчик обобщения
static _Bool isCorrectOperandOfMinus<Operand* src>(Operand** dst) {
  // По умолчанию некорректный операнд
  // fprintf(stderr, "--- incorrect Operand Of Minus\n");
  return 0;
}
// Проверка операнда на допустимость. Константа
static _Bool isCorrectOperandOfMinus<Operand.Const* src>(Operand** dst) {
  Constant* constant = src->@;
  return isCorrectConstantOfMinus<constant>(dst);
}
// Проверка операнда на допустимость. Переменная
static _Bool isCorrectOperandOfMinus<Operand.Var* src>(Operand** dst) {
  Variable* variable = src->@;
  Type* varType = variable->varType;
  return isCorrectVarTypeOfMinus<varType>(dst);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Создание инструкции Minus
Instruction* CreateInstructionMinus(Operand* src) {
  Operand* dst = NULL; // Возможно лучше присвоить в начале Unknown
  struct Instruction.Minus *instruction = create_spec(Instruction.Minus);
  instruction->next = NULL;
  // Проверка на корректность входного операнда инструкции Minus
  if(!isCorrectOperandOfMinus<src>(&dst)) {
    fprintf(stderr, "Incorrect Source Operand of Minus Instruction!!!\n");
    DebugOutOfOperand<src>(stderr);
    // exit(14);
  }
  instruction->@opd1 = src;
  instruction->@opd0 = dst;
  return (Instruction*)instruction;
}

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
  Constant* constant2 = src1->@;
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