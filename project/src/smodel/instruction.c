// Определения функций, обеспечивающих обработку инструкций.

#include <stdlib.h>
#include "instruction.h"

//==============================================================================
// Функции для обработки списка инструкций
//==============================================================================

//------------------------------------------------------------------------------
// Отладочная функция вывода информации о списке инструкций
void DebugOutOfInstructionList(InstructionList* iList, FILE* file) {
  if(iList->first == NULL) {
    fprintf(file, "Instruction List is empty\n");
  }
  Instruction* tmpInstruction = iList->first;
  while(tmpInstruction != NULL) {
    DebugOutOfInstruction<tmpInstruction>(file);
    tmpInstruction = tmpInstruction->next;
  }
  if(iList->result == NULL) {
    fprintf(file, "Result Operand in the Instruction List is empty\n");
  } else {
    DebugOutOfOperand<iList->result>(file);
  }
}

//------------------------------------------------------------------------------
// Инициализация списка инструкций
void InitInstructionList(InstructionList* iList) {
  iList->first  = NULL;
  iList->last   = NULL;
  iList->result = NULL;
}

//------------------------------------------------------------------------------
// Создание списка инструкций
InstructionList* CreateInstructionList() {
  InstructionList* iList = malloc(sizeof(InstructionList));
  if(iList == NULL) {
    perror("ERROR: CreateInstructionList not executed!!!\n");
    exit(13);
  }
  InitInstructionList(iList);
  return iList;
}

//------------------------------------------------------------------------------
// Добавление инструкции в конец списка
void AppendToInstructionList(InstructionList* iList, Instruction* instruction) {
  if(iList->first == NULL) {
    iList->first = instruction;
    iList->last  = instruction;
  }
  iList->last->next  = instruction;
  iList->last  = instruction;
}

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
