#include <stdio.h>
#include <string.h>

#include "parser.h"

//=============================================================================
// Методы транслитератора
//=============================================================================

//------------------------------------------------------------------------------
// Проверка символа на латинскую букву
//!! Возможно будет подключена или реализована в другом месте
//!! проверка на кирилицу
_Bool isLetter(wchar_t ch) {
    if((ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z')) {
        return true;
    } else {
        return false;
    }
    //! Нужно посмотреть использование библиотечных функций
}

//------------------------------------------------------------------------------
// Проверка символа на цифру
_Bool isDigit(wchar_t ch) {
    if(ch >= L'0' && ch <= L'9') {
        return true;
    } else {
        return false;
    }
    // Нужно посмотреть использование библиотечных функций
}

//------------------------------------------------------------------------------
// Проверка символа на пустой разделитель (пробел, табуляция...) пробелов
_Bool isSkip(wchar_t ch) {
    if( ch == L' ' || ch == L'\t' || ch == L'\n' || ch == L'\r' || ch == L'\f') {
        return true;
    } else {
        return false;
    }
}

//------------------------------------------------------------------------------
// Проверка символа на пустой разделитель без перевода строки
_Bool isSpace(wchar_t ch) {
    if( ch == L' ' || ch == L'\t') {
        return true;
    } else {
        return false;
    }
}

//------------------------------------------------------------------------------
// Проверка символа на перевод строки
_Bool isLf(wchar_t ch) {
    if( ch == L'\n') {
        return true;
    } else {
        return false;
    }
}

//------------------------------------------------------------------------------
// Проверка символа на ';' или перевод строки (для анализа разделителя)
_Bool isLfOrSemicoln(wchar_t ch) {
    if(ch == L'\n' || ch == L';') {
        return true;
    } else {
        return false;
    }
}

//--------------------------------------------------------------------------
// Метод, проверяющий достижение конца разбираемой строки
_Bool isEnd(wchar_t ch) {
    if(ch == L'\0') {
        return true;
    } else {
        return false;
    }
}

//------------------------------------------------------------------------------
// Проверка символа на конкретное значение
_Bool isSymbol(wchar_t ch, wchar_t pattern) {
    if(ch == pattern) {
        return true;
    } else {
        return false;
    }
}

//--------------------------------------------------------------------------
// Символ, возможный после слэша
_Bool isAfterSlashSymbol(wchar_t ch, wchar_t buf[]) {
    if(isSymbol(ch, L'0')) {      // нулевой символ (nul)
        wcpcpy(buf, L"\0");
    } else if(isSymbol(ch, 'b')) { // возврат на шаг (BS)
        wcpcpy(buf, L"\b");
    } else if(isSymbol(ch, 'f')) { // перевод формата (FF)
        wcpcpy(buf, L"\f");
    } else if(isSymbol(ch, 'n')) { // перевод строки (LF)
        wcpcpy(buf, L"\n");
    } else if(isSymbol(ch, 'r')) { // возврат каретки (CR)
        wcpcpy(buf, L"\r");
    } else if(isSymbol(ch, 's')) { // пробел ( )
        wcpcpy(buf, L" ");
    } else if(isSymbol(ch,'t')) { // горизонтальная табуляция (HT)
        wcpcpy(buf, L"\t");
    } else if(isSymbol(ch,'v')) { // вертикальная табуляция (VT)
        wcpcpy(buf, L"\v");
    } else if(isSymbol(ch,'\\')) { // обратная косая (\)
        wcpcpy(buf, L"\\");
    } else if(isSymbol(ch, '\'')) { // апостроф (')
        wcpcpy(buf, L"'");
    } else if(isSymbol(ch, '"')) { // кавычка (")
        wcpcpy(buf, L"\"");
    } else {
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------
// Символ, недопустимый без слэша
_Bool isUsingSlashSymbol(wchar_t ch) {
    if(isSymbol(ch, L'\\')) { // обратная косая (\)
        return true;
    } else if(isSymbol(ch, L'\'')) { // апостроф (')
        return true;
    } else if(isSymbol(ch, L'"')) { // кавычка (")
        return true;
    } else {
        return false;
    }
}
