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
void DebugOutOfInstruction<struct Instruction* instruction>(FILE* file) {
  fprintf(file, "Instruction. Unknown Instruction\n");
}

//------------------------------------------------------------------------------
// Отладочная информация об инструкции Halt
void DebugOutOfInstruction<struct Instruction.Halt* instruction>(FILE* file) {
  fprintf(file, "Instruction Нalt\n");
}

//------------------------------------------------------------------------------
// Отладочная информация об инструкции Exit

// Выделение корректных констант инструкции Exit
void DebugOutOfConstantExit<Constant* constant>(FILE* file) {
  fprintf(file, "Forbidden constant type of Exit Instruction (not Int)\n  ");
}
void DebugOutOfConstantExit<Constant.Int* constant>(FILE* file) {
  fprintf(file, "Exit(%d)\n", constant->@constValue);
}

// Выделение корректных типов переменных инструкции Exit
void DebugOutOfVarTypeExit<Type* type>(FILE* file) {
  fprintf(file, "Forbidden variable type of Exit Instruction (not Int)\n  ");
}
void DebugOutOfVarTypeExit<Type.Int* varType>(FILE* file) {
  DebugOutOfType<varType>(file);
}

// Выделение корректных операндов инструкции Exit
void DebugOutOfOperandExit<Operand* opd>(FILE* file) {
  fprintf(file, "Forbidden operand of Exit Instruction\n  ");
}
void DebugOutOfOperandExit<struct Operand.Const* opd>(FILE* file) {
  Constant* constant = opd->@;
  // DebugOutOfConstant<constant>(file);
  DebugOutOfConstantExit<constant>(file);
}
void DebugOutOfOperandExit<struct Operand.Var* opd>(FILE* file) {
  Variable* variable = opd->@;
  Type* varType = variable->varType;
  // DebugOutOfType<varType>(file);
  fprintf(file, "Exit(Variable). ");
  fprintf(file, "    ");
  DebugOutOfVariable<variable>(file);
  fprintf(file, "    Value. ");
  DebugOutOfConstant<variable->varValue>(file);
  fprintf(file, "    Type = ");
  DebugOutOfVarTypeExit<varType>(file);
}


// Непосредственный вывод инструкции Exit
void DebugOutOfInstruction<struct Instruction.Exit* instruction>(FILE* file) {
  fprintf(file, "Instruction Exit\n  ");
  Operand* opd = instruction->@opd;
  // DebugOutOfOperand<opd>(file);
  DebugOutOfOperandExit<opd>(file);
}

//==============================================================================
// Функции, создающие инструкции
//==============================================================================

//------------------------------------------------------------------------------
// Создание инструкции Halt
Instruction* CreateInstructionHalt() {
  struct Instruction.Halt* instruction = create_spec(Instruction.Halt);
  instruction->next = NULL;
  return (Instruction*)instruction;
}

//------------------------------------------------------------------------------
// Создание инструкции Exit
Instruction* CreateInstructionExit(Operand* opd) {
  struct Instruction.Exit* instruction = create_spec(Instruction.Exit);
  instruction->next = NULL;
  instruction->@opd = opd;
  return (Instruction*)instruction;
}