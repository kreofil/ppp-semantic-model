// Определения функций, обеспечивающих обработку инструкций.

#include <stdlib.h>
#include "instruction.h"

//==============================================================================
// Функции вывода отладочной информации по инструкциям
//==============================================================================

//------------------------------------------------------------------------------
// Отладочная функция вывода информации при неизвестной инструкции
void DebugOutOfInstruction<Instruction* instruction>(FILE* file) {
  fprintf(file, "Instruction. Unknown Instruction\n");
}

//------------------------------------------------------------------------------
// Отладочная информация об инструкции Halt
void DebugOutOfInstruction<Instruction.Halt* instruction>(FILE* file) {
  fprintf(file, "Instruction Нalt\n");
}

//------------------------------------------------------------------------------
// Отладочная информация об инструкции Exit
void DebugOutOfInstruction<Instruction.Exit* instruction>(FILE* file) {
  fprintf(file, "Instruction Exit\n  ");
  Operand* opd = instruction->@opd;
  DebugOutOfOperand<opd>(file);
}

//------------------------------------------------------------------------------
// Отладочная информация об инструкции Minus
void DebugOutOfInstruction<Instruction.Minus* instruction>(FILE* file) {
  fprintf(file, "Instruction Minus\n  ");
  Operand* opd = instruction->@opd1;
  DebugOutOfOperand<opd>(file);
  fprintf(file, "     -> ");
  opd = instruction->@opd0;
  DebugOutOfOperand<opd>(file);
}

//------------------------------------------------------------------------------
// Отладочная информация об инструкции Add
void DebugOutOfInstruction<Instruction.Add* instruction>(FILE* file) {
  fprintf(file, "Instruction Add\n  ");
  Operand* opd = instruction->@opd0;
  DebugOutOfOperand<opd>(file);
  fprintf(file, "     = ");
  opd = instruction->@opd1;
  DebugOutOfOperand<opd>(file);
  fprintf(file, "       + ");
  opd = instruction->@opd2;
  DebugOutOfOperand<opd>(file);
}
