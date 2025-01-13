#ifndef __parser__
#define __parser__

#include <stdio.h>
#include <string.h>
#include <wchar.h>

// #include "../smodel/smodel.h"
// #include "smodel.h"

#define bufMaxLen 256
#define true 1
#define false 0

typedef struct CompilerData {
    wchar_t *code;              // указатель на строку с артефактом (кодом)
    int pos;                    // текущая позиция в тексте обрабатываемого артефакта
    int line;                   // строка обрабатываемого артефакта
    int column;                 // колонка в строке для обрабатываемого артефакта
    wchar_t lexValue[bufMaxLen];   // подстрока, определяющая текущую набранную лексему
    int errCount;               // счетчик ошибок
    wchar_t symbol;                // текущий символ
    int oldPos;
    int oldLine;
    int oldColumn;
    wchar_t oldSymbol;
    int qualCount;              // Счетчик идентификаторов в составном имени
    wchar_t **keyWords;           // Указатель на таблицу ключевых слов
    // SemanticModel *sm;          // Указатель на семантическую модель языка
} CompilerData;


//=============================================================================
// Методы парсера
//=============================================================================
// Начальный нетерминал
_Bool isArtefact(CompilerData *cd);
// Определение константы
_Bool isConstDefinition(CompilerData *cd);
// Определение функции
_Bool isFuncDefinition(CompilerData *cd);
// Тело функции
_Bool isBody(CompilerData *cd);
// Элемент тела функции
_Bool isElement(CompilerData *cd);
// Выражение в теле функции
_Bool isExpression(CompilerData *cd);
// Префиксное подвыражение в теле функции
_Bool isPrefixExpression(CompilerData *cd);
// Терм
_Bool isTerm(CompilerData *cd);
// Определение типа.
_Bool isTypeDefinition(CompilerData *cd);
// Определение базового типа.
_Bool isBaseType(CompilerData *cd);
// Определение вектора.
_Bool isArray(CompilerData *cd);
// Определение именованной структуры.
_Bool isStruct(CompilerData *cd);
// Определение неименованной структуры.
_Bool isTuple(CompilerData *cd);
// Определение роя.
_Bool isSwarm(CompilerData *cd);
// Определение импорта.
_Bool isImportDefinition(CompilerData *cd);
// Определение константного выражения.
_Bool isConstExpression(CompilerData *cd);
// Константный терм, содержащий константы и константные операции
_Bool isConstTerm(CompilerData *cd);
// Атомарная константа
_Bool isAtom(CompilerData *cd);
// Константный вектор (список данных)
_Bool isConstDataList(CompilerData *cd);
// Константная группа (параллельный список)
_Bool isConstParList(CompilerData *cd);
// Строка символов
_Bool isString(CompilerData *cd);



//=============================================================================
// Методы сканера
//=============================================================================
// Идентификатор. Не должен быть ключевым словом
_Bool isId(CompilerData *cd);
// Проверка, что это одно из ключевых слов
_Bool isOneOfReservedWord(CompilerData *cd, wchar_t *pattern);
// Простое имя, являющееся идентификатором без дополнительных полей
_Bool isSimpleName(CompilerData *cd);
// Квалифицированный идентификатор (идентификаторы, разделенные точками)
_Bool isQualId(CompilerData *cd);
// Зарезервированное слово
_Bool isReservedWord(CompilerData *cd, wchar_t *pattern);
// Комментарий
_Bool isComment(CompilerData *cd);
// Однострочный комментарий.
_Bool isOneStringComment(CompilerData *cd);
// Многострочный комментарий.
_Bool isManyStringComment(CompilerData *cd);
// Левое обозначение
_Bool isLeftAssign(CompilerData *cd);
// Правое обозначение
_Bool isRightAssign(CompilerData *cd);
// Разделитель аргумента и результата
_Bool isArrow(CompilerData *cd);
// Булева константа true | false
_Bool isBoolean(CompilerData *cd);
// Целочисленная константа со знаком или без знака
_Bool isInteger(CompilerData *cd);
// Действительная константа со знаком или без знака
_Bool isFloat(CompilerData *cd);
// Символьная константа
_Bool isChar(CompilerData *cd);
// Одна из базовых функций, задаваемых специальным знаком или их набором
_Bool isBaseFunc(CompilerData *cd);

//=============================================================================
// Методы транслитератора
//=============================================================================
// Проверка символа на латинскую букву
_Bool isLetter(wchar_t ch);
// Проверка символа на цифру
_Bool isDigit(wchar_t ch);
// Проверка символа на пустой разделитель (пробел, табуляция...)
_Bool isSkip(wchar_t ch);
// Проверка символа на пустой разделитель без перевода строки
_Bool isSpace(wchar_t ch);
// Проверка символа на перевод строки
_Bool isLf(wchar_t ch);
// Проверка символа на ';' или перевод строки (для анализа разделителя)
_Bool isLfOrSemicoln(wchar_t ch);
// Проверка символа на конкретное значение
_Bool isSymbol(wchar_t ch, wchar_t pattern);
// Метод, проверяющий достижение конца разбираемой строки
_Bool isEnd(wchar_t ch);
// Символ, возможный после слэша
_Bool isAfterSlashSymbol(wchar_t ch, wchar_t buf[]) ;
// Символ, недопустимый без слэша
_Bool isUsingSlashSymbol(wchar_t ch);

//=============================================================================
// Вспомогательные методы
//=============================================================================
// Читает следующий символ из входной строки
void NextSym(CompilerData *cd);
// Сохранение текущей позиции для возврата назад
void StorePos(CompilerData *cd);
// Восстановление позиции после отката
void RestorePos(CompilerData *cd);
// Пропуск пробельных символов и комментариев
void Ignore(CompilerData *cd);
// Выдача сообщения об ошибке
void Err(CompilerData *cd, wchar_t* s);


// Начальные установки параметров компилятора и его запуск
_Bool StartCompiler(wchar_t *in, size_t in_len);

#endif
