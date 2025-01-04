// Определения функций, обеспечивающих обработку типов.

#include "type.h"

//==============================================================================
// Функции для обработки типов
//==============================================================================

//------------------------------------------------------------------------------
// Функция вывода значения неизвестного типа
void DebugOutOfType<struct Type* type>(FILE* file) {
  fprintf(file, "Unknown memory type\n");
}

//------------------------------------------------------------------------------
// Функция вывода значения для целочисленного типа
void DebugOutOfType<struct Type.Int* type>(FILE* file) {
  fprintf(file, "Type Int. Size = %d, Align = %d\n",
          type->typeSize, type->align);
}

// Инициализация целочисленного типа
void InitTypeInt(struct Type.Int* typeInt) {
  typeInt->typeSize = sizeof(int);
  typeInt->align = 2;   // по 32-х разрядному слову (пока для примера)
}

//------------------------------------------------------------------------------
// Создание целочисленного типа с использованием динамической памяти
Type* CreateTypeInt() {
  struct Type.Int* typeInt = create_spec(Type.Int);
  InitTypeInt(typeInt);
  // typeInt->typeSize = sizeof(int);
  // typeInt->align = 2;   // по 32-х разрядному слову (пока для примера)
  return (Type*)typeInt;
}
