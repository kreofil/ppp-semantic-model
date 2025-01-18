#include "parser.h"

//=============================================================================
// Методы сканера
//=============================================================================

//--------------------------------------------------------------------------
// Идентификатор. Не должен быть ключевым словом
_Bool isId(CompilerData *cd) {
    StorePos(cd);
    // Предварительная очистка временного буфера:
    wchar_t tmpValue[bufMaxLen]; tmpValue[0] = L'\0';
    int i = 0;
//_0:
    if(isLetter(cd->symbol) || isSymbol(cd->symbol, L'_')) {
        tmpValue[i++] = cd->symbol;
        NextSym(cd);
        goto _1;
    }
    return false;
_1:
    while(isLetter(cd->symbol)
        || isDigit(cd->symbol) || isSymbol(cd->symbol, L'_'))
    {
        tmpValue[i++] = cd->symbol;
        NextSym(cd);
    }
    // Cохраняем идентификатор или ключевое слово
    tmpValue[i] = L'\0';
    wcscpy(cd->lexValue, tmpValue);
    // Проверка, что это одно из ключевых слов
    if(isOneOfReservedWord(cd, tmpValue)) {
        RestorePos(cd);
        printf("Id: It is key word %ls instead identifier!\n", cd->lexValue);
        return false;
    }
    Ignore(cd);
    printf("Id: It is identifier %ls\n", tmpValue);
    return true;
}

//--------------------------------------------------------------------------
// Проверка, что это одно из ключевых слов
// Передаваемый идентификатор сравнивается с ключевыми словами из таблицы
_Bool isOneOfReservedWord(CompilerData *cd, wchar_t *str) {
    for(int i = 0; cd->keyWords[i] != NULL; ++i) {
        if(!wcscmp(cd->keyWords[i], str)) {
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------------
// Простое имя, являющееся идентификатором без дополнительных полей
// Отличается от обычного идентификатора тем, что за ним не может идти точка,
// Характеризующая квалифицированный идентификатор
_Bool isSimpleName(CompilerData *cd) {
//_0:
    if(isId(cd)) {
        ///tmpValue +=value;
        goto _1;
    }
    return false;
_1:
    // Точка для простого имени недопустима
    if(isSymbol(cd->symbol, L'.')) {
        Err(cd, L"SimpleName: Incorrect SimpleName. "
             "The Point (.) may be only in Qual Id.");
        return false;
    }
    // Возврат сформированного идентификатора
    // printf("isSimpleName-->")
    return true;
}

/*
//--------------------------------------------------------------------------
// Квалифицированный идентификатор (идентификаторы, разделенные точками)
// призвольной длины
// Проверка на нужную длину определяется за счет добавления счетчика...
// Допускаются варианты:
// Импортируемое имя
// Поле структуры
// Вложения структур
// Вложение структур и вложений структур в импортируемый артефакт
_Bool Compiler::isQualId(CompilerData *cd) {
    // Фиксация текущей позиции для возврата при неудаче
    //int startPos = iSim;
    std::string tmpValue{""};      // Предварительная очистка временного буфера
    auto count = 0;          // Счетчик идентификаторов
    qualCount = 0;       // Обнуление на всякий случай
//_0:
    if(isId()) {
        tmpValue += lexValue;
        count++;
        goto _1;
    }
    return false;
_1:
    if(isSymbol(cd->symbol, L'.')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _2;
    }
    goto _end;
_2:
    if(isId()) {
        tmpValue += lexValue;
        count++;
        goto _1;
    }
    Err(cd, L"QualId: Incorrect Qual Id");
    return false;
_end:
    lexValue = tmpValue;
    qualCount = count;
    return true;
}
*/

//--------------------------------------------------------------------------
// Зарезервированное слово
_Bool isReservedWord(CompilerData *cd, wchar_t *pattern) {
    ///printf("Reserved:", v, " iSym:", iSym, " Symbol:", symbol)
    // Фиксация текущей позиции для возврата при неудаче
    StorePos(cd);
    // Предварительная очистка временного буфера:
    wchar_t tmpValue[bufMaxLen]; tmpValue[0] = L'\0';
    int i = 0;
//_0:
    if(isLetter(cd->symbol) || isSymbol(cd->symbol, L'_')) {
        // printf("%d:  %lc\n", i, cd->symbol);
        tmpValue[i++] = cd->symbol;
        NextSym(cd);
        goto _1;
    }
    return false;
_1:
    while(isLetter(cd->symbol) || isDigit(cd->symbol) || isSymbol(cd->symbol, L'_')) {
        // printf("%d:  %lc\n", i, cd->symbol);
        tmpValue[i++] = cd->symbol;
        NextSym(cd);
    }
    // Cохраняем идентификатор
    tmpValue[i] = L'\0';
    wcscpy(cd->lexValue, tmpValue);
    // printf("isReservedWord:  %ls\n", cd->lexValue);
    // Проверка, что это заданное ключевое слово
    if(!wcscmp(tmpValue, pattern)) {
        printf("It is key word: %ls\n", tmpValue);
        return true;
    }
    // В противном случае откат к началу
    RestorePos(cd);
    return false;
}

//--------------------------------------------------------------------------
// Комментарий
_Bool isComment(CompilerData *cd) {
    // Одновременная проверка на многострочный и однострочный комментарий.
    // При этом для многострочного комментария допускается вложенность.
//_0:
    if(isOneStringComment(cd)) {
        goto _end;
    }
    if(isManyStringComment(cd)) {
        goto _end;
    }
    return false;
_end:
    return true;
}

//--------------------------------------------------------------------------
// Однострочный комментарий.
_Bool isOneStringComment(CompilerData *cd) {
    // Фиксация текущей позиции для возврата при неудаче
    ///startPos := iSym
    //startSymbol := symbol
    StorePos(cd);
    wchar_t tmpValue[bufMaxLen]; tmpValue[0] = L'\0';
    int i = 0;
    // std::string tmpValue{""};      // Предварительная очистка временного буфера
//_0:
    if(isSymbol(cd->symbol, L'/')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _1;
    }
    return false;
_1:
    if(isSymbol(cd->symbol, L'/')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _2;
    }
    ///iSym = startPos     // Восстановление начальной позиции
    ///symbol = startSymbol
    RestorePos(cd);
    return false;
_2:
    if(isSymbol(cd->symbol, L'\n') || isEnd(cd->code[cd->pos])) {
        tmpValue[i++] += L'\n';
        NextSym(cd);
        goto _end;
    }
    // Любой символ внутри комментария
    tmpValue[i++] += cd->symbol;
    NextSym(cd);
    goto _2;
_end:
    tmpValue[i] = L'\0';
    wcscpy(cd->lexValue, tmpValue);
    Ignore(cd);
    return true;
}

//--------------------------------------------------------------------------
// Многострочный комментарий.
_Bool isManyStringComment(CompilerData *cd) {
    // Фиксация текущей позиции для возврата при неудаче
    StorePos(cd);
    wchar_t tmpValue[bufMaxLen]; tmpValue[0] = L'\0';
    int i = 0;
//_0:
    if(isSymbol(cd->symbol, L'/')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        //fmt.Println("Symbol:", symbol, "-> goto _1")
        goto _1;
    }
    return false;
_1:
    if(isSymbol(cd->symbol, L'*')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _2;
    }
    ///iSym = startPos // Восстановление начальной позиции
    ///symbol = startSymbol
    RestorePos(cd);
    return false;;
_2:
    if(isSymbol(cd->symbol, L'*')) { // Возможное начало завершения комментария
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _3;
    }
    if(isEnd(cd->code[cd->pos])) { // Возможный конец текста. Комментарий не закрыт
        Err(cd, L"ManyStringComment: Incorrect Comment");
        return false;
    }
    if(isManyStringComment(cd)) {
        goto _2;
    }
    // Или любой другой символ, ведущий к заполнению комментария
    tmpValue[i++] += cd->symbol;
    NextSym(cd);
    goto _2;
_3:
    if(isSymbol(cd->symbol, L'/')) { // Закрытие комментария
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _end;
    }
    // Иной символ. Поэтому смотрим на следующий.
    tmpValue[i++] += cd->symbol;
    NextSym(cd);
    goto _2;
_end:
    tmpValue[i] = L'\0';
    wcscpy(cd->lexValue, tmpValue);
    Ignore(cd);
    return true;
}

//--------------------------------------------------------------------------
// Левое обозначение
_Bool isLeftAssign(CompilerData *cd) {
    // Фиксация текущей позиции для возврата при неудаче
    ///startPos := iSym;
    ///startSymbol := Symbol
    StorePos(cd);
    wchar_t tmpValue[bufMaxLen]; tmpValue[0] = L'\0';
    int i = 0;
// 0:
    if(isSymbol(cd->symbol, L'<')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _1;
    }
    return false;
_1:
    if(isSymbol(cd->symbol, L'<')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _end;
    }
    ///iSym = startPos // Восстановление начальной позиции
    ///Symbol = startSymbol
    RestorePos(cd);
    return false;
_end:
    tmpValue[i] = L'\0';
    wcscpy(cd->lexValue, tmpValue);
    Ignore(cd);
    return true;
}

//--------------------------------------------------------------------------
// Правое обозначение
_Bool isRightAssign(CompilerData *cd) {
    // Фиксация текущей позиции для возврата при неудаче
    StorePos(cd);
    wchar_t tmpValue[bufMaxLen]; tmpValue[0] = L'\0';
    int i = 0;
// 0:
    if(isSymbol(cd->symbol, L'>')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _1;
    }
    return false;
_1:
    if(isSymbol(cd->symbol, L'>')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _end;
    }
    ///iSym = startPos // Восстановление начальной позиции
    ///Symbol = startSymbol
    RestorePos(cd);
    return false;
_end:
    tmpValue[i] = L'\0';
    wcscpy(cd->lexValue, tmpValue);
    Ignore(cd);
    return true;
}

/*
//--------------------------------------------------------------------------
// Разделитель аргумента и результата
_Bool Compiler::isArrow(CompilerData *cd) {
    // Фиксация текущей позиции для возврата при неудаче
    StorePos(cd);
    std::string tmpValue{""};      // Предварительная очистка временного буфера
// 0:
    if(isSymbol(cd->symbol, L'-')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _1;
    }
    return false;
_1:
    if(isSymbol(cd->symbol, L'>')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _end;
    }
    ///iSym = startPos // Восстановление начальной позиции
    ///Symbol = startSymbol
    RestorePos(cd);
    return false;
_end:
    lexValue = tmpValue;
    return true;
}

*/
//--------------------------------------------------------------------------
// Булева константа true | false
_Bool isBoolean(CompilerData *cd) {
    ///fmt.Println("Boolean")
    // Фиксация текущей позиции для возврата при неудаче
    ///startPos := iSym;
    ///startSymbol := Symbol
    StorePos(cd);

    //if(isReservedWord("false") || isReservedWord("true")) {
    if(isReservedWord(cd, L"false")) {
        Ignore(cd);
        return true;
    }
    if(isReservedWord(cd, L"true")) {
        Ignore(cd);
        return true;
    }
    ///iSym = startPos     // Восстановление начальной позиции
    ///Symbol = startSymbol
    printf("Value %ls. It is not Boolean\n", cd->lexValue);
    RestorePos(cd);
    return false;
}

//--------------------------------------------------------------------------
// Целочисленная константа со знаком или без знака
_Bool isInteger(CompilerData *cd) {
    //printf("-->isInteger\n");
    // Фиксация текущей позиции для возврата при неудаче
    StorePos(cd);
    // Предварительная очистка временного буфера
    wchar_t tmpValue[bufMaxLen]; tmpValue[0] = L'\0';
    int i = 0;
    // std::string oneSym{""};
// 0:
    //fmt.Println("    i->", symbol)
    if(isSymbol(cd->symbol, L'-') || isSymbol(cd->symbol, L'+')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _1;
    }
    if(isDigit(cd->symbol)) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _2;
    }
    return false;
_1:
    if(isDigit(cd->symbol)) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _2;
    }
    // При откате назад сообщение об ошибке не нужно. Пробуется другая версия.
    // ErrorOut("Integer: Incorrect symbol after sign. Digit wated.");
    ///iSym = startPos     // Восстановление начальной позиции
    ///Symbol = startSymbol
    RestorePos(cd);
    return false;
_2:
    // Накопление цифр целого числа
    if(isDigit(cd->symbol)) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _2;
    }
    // Символы, которые не должны появляться после целого числа
    if(isLetter(cd->symbol)
        || isSymbol(cd->symbol, L'_')
        || isSymbol(cd->symbol, L'.')) {
        // Восстановление начальной позиции
        Err(cd, L"Integer: Incorrect symbol after digits!");
        return false;
    }
    goto _end;
_end:
    tmpValue[i] = L'\0';
    wcscpy(cd->lexValue, tmpValue);
    Ignore(cd);
    return true;
}

/*
//--------------------------------------------------------------------------
// Действительная константа со знаком или без знака
_Bool Compiler::isFloat(CompilerData *cd) {
    // Фиксация текущей позиции для возврата при неудаче
    StorePos(cd);
    std::string tmpValue{""};      // Предварительная очистка временного буфера
    std::string oneSym{""};
// 0:
    //fmt.Println("    f->", Symbol)
    if(isSymbol(cd->symbol, L'-') || isSymbol(cd->symbol, L'+')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _1;
    }
    if(isDigit(symbol)) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _2;
    }
    return false;
_1:
    // Обязательная первая цифра после знака
    if(isDigit(symbol)) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _2;
    }
    ///iSym = startPos     // Восстановление начальной позиции
    ///Symbol = startSymbol
    RestorePos(cd);
    // ErrorOut("Real: Incorrect symbol after sign. Digit wated.");
    return false;
_2:
    // Накопление цифр целой части числа
    if(isDigit(symbol)) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _2;
    }
    // Точка, отделяющая целую часть от дробной
    if(isSymbol(cd->symbol, L'.')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _3;
    }
    // Начало Порядка числа
    if(isSymbol(cd->symbol, L'E') || isSymbol(cd->symbol, L'e')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _5;
    }
    ///iSym = startPos // Восстановление начальной позиции
    ///Symbol = startSymbol
    RestorePos(cd);
    // При откате назад сообщение об ошибке не нужно. Пробуется другая версия.
    //ErrorOut("Real: Incorrect Real Number.");
    return false;
_3:
    // Первая (обязательная) цифра дробной части
    if(isDigit(symbol)) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _4;
    }
_4:
    // Накопление цифр дробной части числа
    if(isDigit(symbol)) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _4;
    }
    // Начало Порядка числа
    if(isSymbol(cd->symbol, L'E') || isSymbol(cd->symbol, L'e')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _5;
    }
    // Символы, которые не должны появляться после числа
    if(isLetter(symbol) || isSymbol(cd->symbol, L'_') || isSymbol(cd->symbol, L'.')) {
        // iSim = startPos; // Восстановление начальной позиции
        Err(cd, L"Integer: Incorrect symbol" + (oneSym = symbol) + " after digits");
        return false;
    }
    goto _end;  // Произошло накопление действительного числа без порядка
_5:
    // Возможен знак перед порядком
    if(isSymbol(cd->symbol, L'-') || isSymbol(cd->symbol, L'+')) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _6;
    }
    // startSymbolЗнак порядка отсутствует
    if(isDigit(symbol)) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _7;
    }
_6:
    // Обязательная первая цифра после знака порядка
    if(isDigit(symbol)) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _7;
    }
    // iSim = startPos; // Восстановление начальной позиции
    Err(cd, L"Real: Incorrect symbol " + (oneSym = symbol) + " after sign. Digit wated");
    return false;;
_7:
    // Накопление цифр порядка
    if(isDigit(symbol)) {
        tmpValue[i++] += cd->symbol;
        NextSym(cd);
        goto _7;
    }
    // Символы, которые не должны появляться после числа
    if(isLetter(symbol) || isSymbol(cd->symbol, L'_') || isSymbol(cd->symbol, L'.')) {
        // iSim = startPos; // Восстановление начальной позиции
        Err(cd, L"Integer: Incorrect symbol " + (oneSym = symbol) + " after digits.");
        return false;
    }
    goto _end;  // Произошло накопление действительного числа
_end:
    lexValue = tmpValue;
    Ignore(cd);
    return true;
}

//--------------------------------------------------------------------------
// Символьная константа
_Bool Compiler::isChar(CompilerData *cd) {
    std::string tmpValue{""};      // Предварительная очистка временного буфера
// 0:
    if(isSymbol(cd->symbol, L'\'')) {
        NextSym(cd);
        goto _1;
    }
    return false;
_1:
    if(isSymbol(cd->symbol, L'\\')) { // Проверка на слэш-префикс
        NextSym(cd);
        goto _2;
    }
    // Допустимый частный случай для символа без слэша - кавычки
    if(isSymbol(cd->symbol, L'"')) { // Проверка на кавычку без слэша
        lexValue = symbol;
        NextSym(cd);
        goto _3;
    }
    if(isUsingSlashSymbol()) { // Если символ, недопустимый без слэша
        Err(cd, L"Char: Slash before this symbol is necessary.");
        return false;
    }
    // Если любой другой символ, допустимый без слэша, то прыгаем через него
    lexValue = symbol;
        NextSym(cd);
        goto _3;
_2:
    if(isAfterSlashSymbol(tmpValue)) { // Символ, допустимый только после слэша
        lexValue = tmpValue;
        ///fmt.Println("Character:", Value)
        NextSym(cd);
        goto _3;
    }
_3:
    if(isSymbol(cd->symbol, L'\'')) { // Закрытие символа
        NextSym(cd);
        goto _end;
    }
    Err(cd, L"Char: The character '" + lexValue + "' is not closed.");
    return false;
_end:
    Ignore(cd);
    return true;
}

//--------------------------------------------------------------------------
// Одна из базовых функций, задаваемых специальным знаком или их набором
// Каждой такой функции соответствует свой функциональный объект в семантической модели
_Bool Compiler::isBaseFunc(CompilerData *cd) {
    // Фиксация текущей позиции для возврата при неудаче
    StorePos(cd);
//_0:
    if(isSymbol(cd->symbol, L'+')) { // Проверки базовых функций, начинающихся со знака '+'
        NextSym(cd);
        if(isSymbol(cd->symbol, L'+')) {
            // инкремент
            *ppdf = sm.GetDeclarationInc();
            NextSym(cd);
            goto _end;
        }
        // плюс
        *ppdf = sm.GetDeclarationAdd();
        //NextSym(cd); -- Уже взяли следующий символ
        goto _end;
    }
    if(isSymbol(cd->symbol, L'-')) { // Проверки базовых функций, начинающихся со знака '-'
        NextSym(cd);
        if(isSymbol(cd->symbol, L'-')) {
            // декремент
            *ppdf = sm.GetDeclarationDec();
            NextSym(cd);
            goto _end;
        }
        // минус
        *ppdf = sm.GetDeclarationSubMin();
        //NextSym(cd); -- Уже взяли следующий символ
        goto _end;
    }
    // Наличие базовой функции не подтвердилось.
    return false;
_end:
    return true;
}
*/