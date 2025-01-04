#ifndef __instruction__
#define __instruction__
// instruction.h
// Объявление обобщенной инструкции, определяющей множество команд
// семантической модели.

#include <stdio.h>
#include "operand.h"

//==============================================================================
// Обобщенная операнд
typedef struct Instruction {
  struct Instruction* next;   // Следующая инструкция в списке
}<> Instruction;

//------------------------------------------------------------------------------
// Список инструкций
typedef struct InstructionList {
  struct Instruction* first;   // Первая инструкция в списке
  struct Instruction* last;    // Последняя инструкция в списке
  Operand* result;   // Последний полученный результат в списке инструкций???
} InstructionList;

//------------------------------------------------------------------------------
// Структуры, определяющие различные варианты операндов инструкций

// Для инструкций с одним операндом
typedef struct OneOperand {
  Operand* opd;
} OneOperand;
// Для инструкций с двумя операндами
typedef struct TwoOperands {
  Operand* opd1;
  Operand* opd2;
} TwoOperands;
// Для инструкций с тремя операндами
typedef struct ThreeOperands {
  Operand* opd1;
  Operand* opd2;
  Operand* opd3;
} ThreeOperands;

//------------------------------------------------------------------------------
// Специализации инструкций
Instruction + <Halt: void;>;          // Инструкция останова
Instruction + <Exit: OneOperand;>;   // Инструкция прерывания с кодом ошибки


//------------------------------------------------------------------------------
// Функции для работы со списками инструкций

// Функция вывода списка инструкций
void DebugOutOfInstructionList(InstructionList* iList, FILE* file);
// Инициализация списка инструкций
void InitInstructionList(InstructionList* iList);
// Создание списка инструкций
InstructionList* CreateInstructionList();
// Добавление инструкции в конец списка
void AppendToInstructionList(InstructionList* iList, Instruction* instruction);

// Обобщенная функция вывода параметров инструкции
void DebugOutOfInstruction<struct Instruction* instruction>(FILE* file);

//------------------------------------------------------------------------------
// Создание инструкций различного назначения

// Создание инструкции Halt
Instruction* CreateInstructionHalt();
// Создание инструкции Exit
Instruction* CreateInstructionExit(Operand* opd);

#endif // __instruction__
