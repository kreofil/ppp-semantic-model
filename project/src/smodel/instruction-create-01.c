// Реализация функций, осуществляющих создание инструкций, проверку корректности
// их операндов и формирующих при необходимости корректный выходной операнд
// Формирование инструкций, содержащих от 0 до 2 операндов.

#include <stdlib.h>
#include "instruction.h"
#include "global.h"

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
  fprintf(stderr, "--- incorrect Operand Of Minus\n");
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
  // fprintf(stderr, "--- incorrect Src Operand Of Minus Instruction\n");
  *dst = GetUnknownOperand();
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
  Operand* dst = GetUnknownOperand();
  // Operand* dst = NULL; // Возможно лучше присвоить в начале Unknown
  struct Instruction.Minus *instruction = create_spec(Instruction.Minus);
  instruction->next = NULL;
  // Проверка на корректность входного операнда инструкции Minus
  if(!isCorrectOperandOfMinus<src>(&dst)) {
    fprintf(stderr, "Incorrect Source Operand of Minus Instruction!!!\n");
    fprintf(stderr, "Result Operand of Minus Instruction:\n    ");
    DebugOutOfOperand<src>(stderr);
    // exit(14);
  }
  instruction->@opd1 = src;
  instruction->@opd0 = dst;
  return (Instruction*)instruction;
}
