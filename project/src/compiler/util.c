#include <stdio.h>

#include "parser.h"

//=============================================================================
// Вспомогательные методы
//=============================================================================

//------------------------------------------------------------------------------
// Читает следующий символ из входной строки
void NextSym(CompilerData *cd) {
    // printf("%d: %lc\n", cd->pos, cd->code[cd->pos]);
    if(isLf(cd->code[cd->pos])) {
        cd->line++;
        cd->column = 1;
    } else {
        cd->column++;
    }
    cd->pos++;
    if(!isEnd(cd->code[cd->pos])) {
        cd->symbol = cd->code[cd->pos];
    } else {
        // cd->symbol = -1;
        cd->symbol = L'\0';
    }
}

// Сохранение текущей позиции для возврата назад
void StorePos(CompilerData *cd) {
    cd->oldPos = cd->pos;
    cd->oldSymbol = cd->symbol;
    cd->oldLine = cd->line;
    cd->oldColumn = cd->column;
}

// Восстановление позиции после отката
void RestorePos(CompilerData *cd) {
    cd->pos = cd->oldPos;
    cd->symbol = cd->oldSymbol;
    cd->line = cd->oldLine;
    cd->column = cd->oldColumn;
}

//--------------------------------------------------------------------------
// Пропуск пробельных символов и комментариев
void Ignore(CompilerData *cd) {
_0:
    if(isSymbol(cd->symbol, L' ') || isSymbol(cd->symbol, L'\n')) {
        NextSym(cd);
        goto _0;
    }
    else if(isComment(cd)) {
        goto _0;
    }
}

//--------------------------------------------------------------------------
// Выдача сообщения об ошибке
void Err(CompilerData *cd, wchar_t* s) {
    cd->errCount++;
    printf("[%d,%d]: %ls\n", cd->line+1, cd->column+1, s);
}