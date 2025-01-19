#include "parser.h"
#include "name-table.h"
#include "global.h"

//-----------------------------------------------------------------------------
// Список ключевых и зарезервированных слов,
// которые не могут использоваться в качестве идентификатора
const wchar_t *keyWords[] = {
    L"array",
    L"const",
    L"false",
    L"func",
    L"safe",
    L"stream",
    L"struct",
    L"swarm",
    L"true",
    L"var",
    L"vector",
    L"type",
    NULL
};

//-----------------------------------------------------------------------------
// Начальные установки параметров компилятора и его запуск
_Bool StartCompiler(wchar_t *in, size_t in_len) {
  CompilerData cd;
  cd.code = in;
  cd.pos = 0;
  cd.line = 0;
  cd.column = 1;
  cd.lexValue[0] = L'\0';
  cd.errCount = 0;
  cd.symbol = cd.code[0];
  cd.qualCount = 0;
  cd.keyWords = (wchar_t**)keyWords;

  InitGlobalData();   // Инициализация глобальных данных

  printf("***** SOURCE CODE *****\n\n");
  printf("%ls\n", cd.code);
  printf("\n***** COMPILER STARTED *****\n");

  if(isUnit(&cd)) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
    return false;
  }

  // sm->DebugOut();
  printf("\n***** RESERVED NAME TABLE *****\n");
  DebugOutOfNameTable(GetReservedNameTable(), stdout);
  printf("\n***** GLOBAL NAME TABLE *****\n");
  DebugOutOfNameTable(GetGlobalNameTable(), stdout);
  return true;    // while
}

//=============================================================================
// Начальный нетерминал
//=============================================================================

//-----------------------------------------------------------------------------
// Первоначальное определение очень простое...
// Unit = {/ (VarDefinition | Statement) [';'|{'\n'}] /}
_Bool isUnit(CompilerData *cd) {
  wchar_t artefactName[bufMaxLen]; artefactName[0] = L'\0';
  Ignore(cd);   // пропуск пробелов и комментариев перед первой лексемой
//_0: Проверка на описание переменной или оператор
  if (isVarDefinition(cd)) {
    goto _1;
  }
  if (isStatement(cd)) {
    goto _1;
  }
  // Начальный терминал не может начинаться с неправильных нетерминалов
  Err(cd, L"Unit 00: It is not unit");
  return false;
_1: // Проверка на точку с запятой, перевод строки, конец файла.
  if(isSymbol(cd->symbol, L';')) {
    NextSym(cd);
    Ignore(cd);
    goto _2;
  }
  if(isEnd(cd->symbol)) {
    goto _end;
  }
  printf("symbol = %lc(%d)\n", cd->symbol, cd->symbol);
  Err(cd, L"Unit 01: Expected ';', EOF");
  return false;
_2:
  if (isVarDefinition(cd)) {
    goto _1;
  }
  if (isStatement(cd)) {
    goto _1;
  }
  if(isEnd(cd->symbol)) {
    goto _end;
  }
  Err(cd, L"Unit 02: Expected VarDefinition, Statement, EOF");
  return false;
_end:
  Ignore(cd);   // пропуск пробельных символов и комментариев
  return true;
}

//-----------------------------------------------------------------------------
// Определение переменной (в начале очень простое, но затем...)
// VarDefinition = var Id {',' Id} ':' TypeId
_Bool isVarDefinition(CompilerData *cd) {
  // Список имен объявляемых переменных
  wchar_t varNamesList[varMaxSize][bufMaxLen];
  int varCount = 0;   // Счетчик накапливаемых переменных
  NameTable* globalNameTable = GetGlobalNameTable();
  NameTable* reservedNameTable = GetReservedNameTable();
  Type* varType = NULL; // Стоит заменить на неопределенный тип (ввести)
  Constant* constValue = NULL; // Стоит заменить на неопределенное значение
  // wchar_t artefactName[bufMaxLen];
  // artefactName[0] = L'\0';
// 0:
  if(isReservedWord(cd, L"var")) {
    Ignore(cd);
    goto _1;
  }
  return false;
_1: // Проверяется наличие простого имени, определяемой переменной
  if (isSimpleName(cd)) {
    Ignore(cd);
    // Проверка, что имя не встретилось раньше в этом описании
    for(int i = 0; i < varCount; ++i) {
      if(!wcscmp(varNamesList[i], cd->lexValue)) {
        Err(cd, L"VarDefinition 01.01: Second name definition");
        return false;
      }
    }
    // Проверка на то что имя отсутсвует в таблице имен
    if(findElementInTable(globalNameTable, cd->lexValue) != NULL) {
      Err(cd,
          L"VarDefinition 01.02: Second name definition in the global table");
      return false;
    }
    // Проверка на то что имя отсутсвует в таблице зарезервированных имен
    if(findElementInTable(reservedNameTable, cd->lexValue) != NULL) {
      Err(cd,
          L"VarDefinition 01.03: This name is in the reserved name table");
      return false;
    }
    // Фиксация имени в списке накапливаемых имен, если ОК
    wcscpy(varNamesList[varCount++], cd->lexValue);
    goto _2;
  }
  Err(cd, L"VarDefinition 01: Expected name of variable");
  return false;
_2: //  ',' | ':'
  if(isSymbol(cd->symbol, L',')) {
    NextSym(cd);
    Ignore(cd);
    goto _1;
  }
  if(isSymbol(cd->symbol, L':')) {
    NextSym(cd);
    Ignore(cd);
    goto _3;
  }
  Err(cd, L"VarDefinition 02: Expected ',' or ':'");
  return false;
_3: // Тип переменной (пока только именованный скаляр)
  if (isReservedWord(cd, L"int")) {
    varType = GetTypeInt();
    constValue = GetConstIntZero();
    Ignore(cd);
    goto _end;
  }
  // if (isSimpleName(cd)) {
  //   Ignore(cd);
  //   goto _end;
  // }
  Err(cd, L"VarDefinition 03: Expected name of type");
  return false;
_end:
  // После накопления переменных и их типа данные можно занести в таблицу имен
  // Создается контекст переменной для каждого имени
  for(int i = 0; i < varCount; ++i) {
    // Создание контекста
    // ??! Тип пока в лоб. Нужно не забыть поменять после добавления проверки типа
    Variable* variable = CreateVariableGlobal(varType, constValue);
    Context* context = CreateContextVar(variable);
    // Добавление контекста в таблицу имен
    AddElementToNameTable(globalNameTable, varNamesList[i], context);
  }
  Ignore(cd);   // пропуск пробельных символов и комментариев
  return true;
}

//-----------------------------------------------------------------------------
// Оператор (присваивания, тривиальный, но затем...)
// Statement = id '=' (id | integer)
_Bool isStatement(CompilerData *cd) {
// 0:
  if(isSimpleName(cd)) {
    Ignore(cd);
    goto _1;
  }
  return false;
_1:
  if(isSymbol(cd->symbol, L'=')) {
    NextSym(cd);
    Ignore(cd);
    goto _2;
  }
  Err(cd, L"Statement 01: Expected '='");
_2:
  if(isSimpleName(cd)) {
    Ignore(cd);
    goto _end;
  }
  if(isInteger(cd)) {
    Ignore(cd);
    goto _end;
  }
_end:
  Ignore(cd);   // пропуск пробельных символов и комментариев
  return true;
}


//-----------------------------------------------------------------------------
//! Artefact = ConstDefinition | FuncDefinition | ProtoDefinition
//!          | TypeDefinition  | ImportDefinition
_Bool isArtefact(CompilerData *cd) {
  wchar_t artefactName[bufMaxLen];
  artefactName[0] = L'\0';
  Ignore(cd);   // пропуск пробелов и комментариев перед первой лексемой
//_0: Проверка на имя артефакта или начало его определения
  // В начале проверяется константа, чтобы обойти проверку обозначений
  // ключевые слова для true и false
  if (isConstDefinition(cd)) {
      goto _3;
  }
  // Проверка на обозначение артефакта
  if(isSimpleName(cd)) {
    wcscpy(artefactName, cd->lexValue); // Фиксация имени артефакта
    Ignore(cd); // пропуск пробельных символов и комментариев
    goto _1;
  }
  // Далее пошли варианты с описанием альтернативных артефактов
  // с последующим постфиксным обозначением
  Err(cd, L"Artefact 0: It is not artefact");
  return false;
_1: // Проверка на лексему левого обозначения.
  if(isLeftAssign(cd)) {
      Ignore(cd);
      goto _2;
  }
  Err(cd, L"Artefact: It is not Left Assignment (<<)");
  return false;
_2:
  // Далее пошли варианты с описанием альтернативных артефактов
  // после префиксного обозначения
  if (isConstDefinition(cd)) {
      // Добавление артефакта в таблицу описаний (или замена старого артефакта)
      //smodel.ExportNameTab[artefactName] = *context
      goto _5;
  }
_3: // Должно быть постфиксное обозначение
  if(isRightAssign(cd)) {
      Ignore(cd);
      goto _4;
  }
  Err(cd, L"Artefact: It is not Right Assignment (>>)");
  return false;
_4: // И завершающее имя...
  if(isSimpleName(cd)) {
      //smodel.SetArtefactName() // Фиксация имени артефакта
      //smodel.RemoveDeclaration()    // Удаление описания из таблицы описаний
      wcscpy(artefactName, cd->lexValue); // Фиксация имени артефакта
      Ignore(cd);
      goto _5;
  }
_5: // В конце возможна точка с запятой
  if(isSymbol(cd->symbol, L';')) {
      NextSym(cd);
      Ignore(cd);
      goto _6;
  }
  goto _6;
_6: // Завершение просмотра на конец строки
  if(isEnd(cd->symbol)) {
      goto _end;
  }
  printf("%d\n", cd->symbol);
  Err(cd, L"Artefact: Incorrect symbols at  the end of Artefact");
  return false;
_end:
  Ignore(cd);   // пропуск пробельных символов и комментариев
  // Добавление имени артефакта в семантическую модель
  // sm.SetArtefactName(artefactName);
  // Добавление артефакта в таблицу описаний (или замена старого артефакта)
  // pdcl->SetName(artefactName);
  // if(sm.AddToExportNameTable(pdcl)) {
      // return true;
  // }
  // Err(cd, L"Artefact: Incorrect extention of export table");
  // return false;
  return true;
}

//=============================================================================
// Методы парсера
//=============================================================================

//--------------------------------------------------------------------------
// Определение константы
_Bool isConstDefinition(CompilerData *cd) {
    // Const* pcv;
//_0: Проверка текущей лексемы на ключевое слово const
    // if(isReservedWord("const")) {
    //     Ignore(cd);
    //     goto _1;
    // }
    if(isConstExpression(cd)) {
        goto _end;
    }
    return false;
_end:
    // *ppdcl = sm.newDeclarationConst(pcv);
    return true;
}

/*
//--------------------------------------------------------------------------
// Определение функции
_Bool Compiler::isFuncDefinition(CompilerData *cd) {
    std::string argName{""};
    //std::string argTypeName{""};
    //std::string resTypeName{""};
    Type* pta = NULL;  // Указатель на тип аргумента
    Type* ptr = NULL;  // Указатель на тип результата
    DeclarationType* adt = NULL; // Указатель на объявление аргумента
    DeclarationType* rdt = NULL; // Указатель на объявление результата
    Func* pf = NULL;
//_0: Проверка текущей лексемы на ключевое слово isConstDefinitionfunc
    // Проверка на то, что функция начинается с символа "\"
    if(isSymbol(cd->symbol, L'\\')) {
        NextSym(cd);
        Ignore(cd);
        // В принципе после этого шага можно начинать формирование описания функции
        pf = sm.NewFunc();
        *ppf = pf;
        Ignore(cd);
        goto _1;
    }
    if(isReservedWord("func")) {
        // В принципе после этого шага можно начинать формирование описания функции
        pf = sm.NewFunc();
        *ppf = pf;
        Ignore(cd);
        goto _1;
    }
    return false;
_1: // Проверяется наличие идентификатора аргумента
    if (isId()) {
        // В принципе локальное имя может быть любым кроме ключевых слов
        argName = lexValue;
        pf->SetArgTypeName(argName);
        Ignore(cd);
        goto _2;
    }
    if(isArrow()) {
        // Отсутствует аргумент. Поэтому нужно сгенерировать на выходе объявление сигнала.
        std::string argTypeName{"signal"};
        /////adt = &dclSignal("signal", &typeSignal);

        adt = sm.FindTypeDeclaration(argTypeName); // Поиск именованного типа
        Ignore(cd);
        goto _6;
    }
    Err(cd, L"FuncDefinition: Argument declaration or arrow wated");
    return false;
_2: // Проверка на наличие типа аргумента
    if(isSymbol(cd->symbol, L'@')) {
        NextSym(cd);
        Ignore(cd);
        goto _3;
    }
    // Здесь также возможен кортеж
    //...
    Err(cd, L"FuncDefinition: Incorrect declaration of argument type");
    return false;
_3: // Проверка наличия имени типа аргумента
    if(isQualId()) {
        // Поиск соответствующего имени и анализ его на имя типа
        /// Пока ограничимся поиском в том же пакете
        //argTypeName = lexValue;
        adt = sm.FindTypeDeclaration(lexValue); // Поиск именованного типа
        Ignore(cd);
        goto _4;
    }
    if(isTuple(&pta)) { // кортеж
        adt = sm.newDeclarationType(pta); // Создается неименованный кортеж
        Ignore(cd);
        goto _4;
    }
    Err(cd, L"FuncDefinition: Incorrect declaration of argument type");
    return false;
_4: // Проверка разделителя аргумента и результата
    if(isArrow()) {
        Ignore(cd);
        goto _6;
    }
    Err(cd, L"FuncDefinition: arrow after argument wated");
    return false;
_6: // Проверка имени типа
    if(isQualId()) {
        // Фиксация типа результата по факту
        //resTypeName = lexValue;
        rdt = sm.FindTypeDeclaration(lexValue); // Поиск именованного типа
        Ignore(cd);
        // К данному моменту получены все данные для формирования прототипа
        goto _7;
    }
    if(isTuple(&ptr)) { // кортеж
        rdt = sm.newDeclarationType(ptr); // Создается неименованный кортеж
        Ignore(cd);
        goto _7;
    }
    Err(cd, L"FuncDefinition: Incorrect declaration of result type");
    return false;
_7:
    pf->SetArgType(adt->GetType());
    pf->SetResType(rdt->GetType());
    // Можно установить объявления в акторах аргумента и возврата
    // Формирование обязательной начальной вершины РИГ, фиксирующей объявление аргумента
    // Всегда располагается в нулевой позиции вектора вершин
    pf->AddActor(new ActorArg{adt, 0});
    // Добавление обязательной вершины возврата из функции.
    // Она следует за начальной вершиной.
    pf->AddActor(new ActorRet{rdt, 1});
    // Имя необходимо занести в локальную таблицу имен, а также установить его объявление
    // Для хранения используется объявление актора, связанного с данным именем.
    // В данном случае это актор аргумента функции.
    DeclarationActor* pda = new DeclarationActor{argName, pf->GetActor(0)};
    pf->AddToLocalNameTable(pda);
    // Или тело функции или завершение неполного описания
    if(isBody(*pf)) {
        goto _8;
    }
    if(isSymbol(cd->symbol, L';')) {
        Ignore(cd);
        goto _end;
    }
    // Неполное описание можно завершить без точки с запятой
    goto _end;
_8:
    // Полное описание можно завершить без точки с запятой
    if (isSymbol(cd->symbol, L';')) {
        Ignore(cd);
        goto _end;
    }
    goto _end;
_end:
    // Формирование прототипа функции без увязки с уже существующими прототипами.
    FuncProto* funcProto = sm.NewFuncProto();
    if((adt != NULL)&&(rdt != NULL)) {
        // Выполняется добавление сигнатуры в список сигнатур
        funcProto->AddSig(Signature(adt, rdt));
    } else {
        Err(cd, L"FuncDefinition: Incorrect type name of argument or result");
        return false;
    }
    *ppdcl = sm.newDeclarationFuncProto(funcProto);

    //pf->DebugOut();
    return true;
}

// Тело функции
_Bool Compiler::isBody(CompilerData *cd) {
    ////Func* pf = NULL;
//_0: Проверка текущей лексемы на ключевое слово type
    if(isSymbol(cd->symbol, L'{')) { // Начало тела функции
        NextSym(cd);
        Ignore(cd);
        goto _1;
    }
    return false;
_1:
    // Проверка на базовый именованный тип
    if(isElement(f)) {
        Ignore(cd);
        goto _2;
    }
    Err(cd, L"isBody: A element of body was expected");
    return false;
_2:
    if(isSymbol(cd->symbol, L';')) {
        NextSym(cd);
        Ignore(cd);
        goto _3;
    }
    if(isSymbol(cd->symbol, L'}')) {
        NextSym(cd);
        Ignore(cd);
        goto _end;
    }
    Err(cd, L"isBody: ';' or '}' was expected");
    return false;
_3:
    if(isSymbol(cd->symbol, L'}')) {
        NextSym(cd);
        Ignore(cd);
        goto _end;
    }
    // Проверка на базовый именованный тип
    if(isElement(f)) {
        Ignore(cd);
        goto _2;
    }
    Err(cd, L"isBody: element of body or '}' was expected");
    return false;
_end:
    return true;
}

// Элемент тела функции
_Bool Compiler::isElement(CompilerData *cd) {
    // Сохранение позиции для возможного отката назад
    auto ipos{pos};
    auto isymbol{symbol};
    auto iline{line};
    auto icolumn{column};
    ///auto erFlag = false;     // флаг для отката или ошибки
//_0:
    if(isId()) { // Левое обозначение выражения
        Ignore(cd);
        goto _1;
    }
    if(isExpression(f)) { // Выражение
        Ignore(cd);
        goto _3;
    }
    return false;
_1:
    if(isLeftAssign()) { // Левое обозначение
        Ignore(cd);
        goto _2;
    }
    // Здесь возможен откат назад для того, чтобы перейти к анализу
    // Выражения без левого обозначения (но, возможно, с правым обозначением)
    pos = ipos;
    symbol = isymbol;
    line = iline;
    column = icolumn;
    if(isExpression(f)) { // Выражение
        Ignore(cd);
        goto _3;
    }
    Err(cd, L"Element: It is not Left Assignment (<<)");
    return false;
_2:
    if(isExpression(f)) { // Выражение
        Ignore(cd);
        goto _end;
    }
    Err(cd, L"Element: Expression was expected");
    return false;
_3:
    if(isRightAssign()) { // Левое обозначение
        Ignore(cd);
        goto _4;
    }
    goto _end;  // Выражение без обозначений возможны
_4:
    if(isId()) { // Правое обозначение выражения
        Ignore(cd);
        goto _end;
    }
    Err(cd, L"Element: Right Name of Expression was expected");
    return false;
_end:
    return true;
}

// Выражение в теле функции
_Bool Compiler::isExpression(CompilerData *cd) {
    Actor* aLeft = NULL;
    Actor* aRight = NULL;
    ActorInterpret* pai1 = NULL;
    ActorInterpret* pai2 = NULL;
//_0:
    if(isTerm(f, &aLeft)) { // Терм
        Ignore(cd);
        goto _1;
    }
    return false;
_1:
    if(isSymbol(cd->symbol, L':')) {
        // Можно начать формирование одноаргументного оператора интерпретации
        pai1 = new ActorInterpret{f.ActorNumber()};
        f.AddActor(pai1);
        pai1->SetArg(aLeft);
        NextSym(cd);
        Ignore(cd);
        goto _2;
    }
    if(isPrefixExpression(f)) { // Префиксный оператор интерпретации
        Ignore(cd);
        goto _3;
    }
    Err(cd, L"Expression: Prefix Expression or ':' were expected");
    return false;
_2:
    if(isTerm(f, &aRight)) { // Терм
        pai1->SetFunc(aRight);
        Ignore(cd);
        goto _3;
    }
    Err(cd, L"Expression: Term was expected");
    return false;
_3:
    if(isSymbol(cd->symbol, L':')) {
        // Можно начать формирование одноаргументного оператора интерпретации
        pai2 = new ActorInterpret{f.ActorNumber()};
        f.AddActor(pai2);
        pai2->SetArg(pai1);
        pai1 = pai2;
        NextSym(cd);
        Ignore(cd);
        goto _2;
    }
    if(isPrefixExpression(f)) { // Префиксный оператор интерпретации
        Ignore(cd);
        goto _3;
    }
    goto _end;
_end:
    return true;
}

// Префиксное подвыражение в теле функции
_Bool Compiler::isPrefixExpression(CompilerData *cd) {
    ///Actor* aLeft = NULL;
    Actor* aRight = NULL;
//_0:
    if(isSymbol(cd->symbol, L'^')) {
        NextSym(cd);
        Ignore(cd);
        goto _1;
    }
    return false;
_1:
    if(isTerm(f, &aRight)) { // Терм
        Ignore(cd);
        goto _2;
    }
    Err(cd, L"Prefix Expression: Term was expected");
    return false;
_2:
    if(isPrefixExpression(f)) { // Префиксный оператор интерпретации
        Ignore(cd);
        goto _end;
    }
    goto _end;
_end:
    return true;
}

// Терм
_Bool Compiler::isTerm(CompilerData *cd) {
    Const* pcv;
    DeclarationFunc* pdf;
//_0:
    if(isReservedWord("return")) {
        // Актор возврата уже существует и стоит на второй позиции.
        // Поэтому его формировать не нужно. Но необходимо передать оператору интерпретации
        *ppa = f.GetActor(1);
        Ignore(cd);
        goto _end;
    }
    if(isBaseFunc(&pdf)) {
        // Создание актора, определяющего функцию для базовой функции
        ActorFunc* paf = new ActorFunc{pdf, f.ActorNumber()};
        f.AddActor(paf);
        *ppa = paf;
        Ignore(cd);
        goto _end;
    }
    if(isAtom(&pcv)) {
        // Создание константного актора, который добавляется в список акторов
        Declaration* pdc = sm.newDeclarationConst(pcv);
        ActorConst* pac = new ActorConst{pdc, f.ActorNumber()};
        f.AddActor(pac);
        *ppa = pac;
        Ignore(cd);
        goto _end;
    }
    if(isQualId()) {
        // Некоторое имя, которое может находиться в одной из таблиц имен
        auto name = lexValue;
        if(DeclarationActor* pda = f.FindLocalDeclaration(name); pda != NULL) {
            // Имя является обозначением актора, на который ссылается.
            *ppa = pda->GetActor();
        }
        Ignore(cd);
        goto _end;
    }
    return false;
_end:
    return true;
}

/!!
//--------------------------------------------------------------------------
// Определение прототипа функции.
_Bool Compiler::isProtoDefinition() {

    return false;
}
!!/

//--------------------------------------------------------------------------
// Определение типа.
_Bool Compiler::isTypeDefinition(CompilerData *cd) {
    Type* ptv;
//_0: Проверка текущей лексемы на ключевое слово type или значок "@"
    if(isSymbol(cd->symbol, L'@')) {
        NextSym(cd);
        Ignore(cd);
        goto _1;
    }
    if(isReservedWord("type")) {
        Ignore(cd);
        goto _1;
    }
    return false;
_1:
    // Проверка на базовый именованный тип
    if(isBaseType(&ptv)) {
        Ignore(cd);
        goto _end;
    }
    // Проверка на вектор
    if(isArray(&ptv)) {
        Ignore(cd);
        goto _end;
    }
    // Проверка на структуру
    if(isStruct()) {
        Ignore(cd);
        goto _end;
    }
    // Проверка на кортеж
    if(isTuple(&ptv)) {
        Ignore(cd);
        goto _end;
    }
    // Проверка на рой (толпу)
    if(isSwarm(&ptv)) {
        Ignore(cd);
        goto _end;
    }
    // Именованный тип (идентификатор или квалифицированны ид. на два имени
    // Одно из определений должно быть именованным типом
    //...
    Err(cd, L"isTypeDefinition: A type definition have been expected");
    return false;
_end:
    *ppdcl = sm.newDeclarationType(ptv);
    return true;
}

//--------------------------------------------------------------------------
// Определение базового типа.
_Bool Compiler::isBaseType(CompilerData *cd) {
//_0:
    if(isReservedWord("signal")) { // сигнальный тип
        *pptv = sm.GetTypeSignal();
        Ignore(cd);
        goto _end;
    }
    if(isReservedWord("int")) { // целочисленный тип
        *pptv = sm.GetTypeInt();
        Ignore(cd);
        goto _end;
    }
    if(isReservedWord("_Bool")) { // булевский тип
        *pptv = sm.GetTypeBool();
        Ignore(cd);
        goto _end;
    }
    if(isReservedWord("float")) { // действительный тип
        *pptv = sm.GetTypeFloat();
        Ignore(cd);
        goto _end;
    }
    // Остальные будут дописаны позднее
    return false;
_end:
    return true;
}

//--------------------------------------------------------------------------
// Определение вектора.
// Vector = "(" [Integer] ")" TypeName
// TypeName = BaseType | QualId
_Bool Compiler::isArray(CompilerData *cd) {
    // Сохранение позиции для возможного отката назад
    auto ipos{pos};
    auto isymbol{symbol};
    auto iline{line};
    auto icolumn{column};
    auto erFlag = false;     // флаг для отката или ошибки
//_0:
    if(isSymbol(cd->symbol, L'(')) {
        NextSym(cd);
        Ignore(cd);
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _1;
    }
    return false;
_1:
    if(isInteger()) { // размерность вектора
        Ignore(cd);
        erFlag = true;
        goto _2;
    }
/!!
    if(isSymbol(cd->symbol, L')')) {
        NextSym(cd);
        Ignore(cd);
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _3;
    }
!!/
    // Откат или сообщение об ошибке
    if(erFlag) {
        Err(cd, L"isArray: Incorrect size of array");
    } else {
        // Если не целое, то возможна структура. Откат назад
        pos = ipos;
        symbol = isymbol;
        line = iline;
        column = icolumn;
    }
    return false;
_2:
    if(isSymbol(cd->symbol, L')')) {
        NextSym(cd);
        Ignore(cd);
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _3;
    }
    if(isSymbol(cd->symbol, L',')) {
        NextSym(cd);
        Ignore(cd);
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _1;
    }
    Err(cd, L"isArray: ')' have been expected");
    return false;
_3:
    if(isBaseType(pptv)) {
        Ignore(cd);
        goto _end;
    }
    // Остальные типы будут дописаны позднее
    Err(cd, L"isArray: vector type must have type of elements");
    return false;
_end:
    return true;
}

//--------------------------------------------------------------------------
// Определение именованной структуры.
_Bool Compiler::isStruct() {
    // Сохранение позиции для возможного отката назад
    auto ipos{pos};
    auto isymbol{symbol};
    auto iline{line};
    auto icolumn{column};
    auto erFlag = false;     // флаг для отката или ошибки
//_0:
    if(isSymbol(cd->symbol, L'(')) {
        NextSym(cd);
        Ignore(cd);
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _1;
    }
    return false;
_1:
    if(isSymbol(cd->symbol, L'{')) {
        NextSym(cd);
        Ignore(cd);
        erFlag = true;
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _2;
    }
    if(isId()) { //
        Ignore(cd);
        goto _4;
    }
    // Откат или сообщение об ошибке
    if(erFlag) {
        Err(cd, L"isStruct: Incorrect name list");
    } else {
        // Если не целое, то возможна структура. Откат назад
        pos = ipos;
        symbol = isymbol;
        line = iline;
        column = icolumn;
    }
    return false;
_2:
    if(isId()) { //
        Ignore(cd);
        goto _3;
    }
    Err(cd, L"isStruct: Expected name of field");
    return false;
_3:
    if(isSymbol(cd->symbol, L',')) {
        NextSym(cd);
        Ignore(cd);
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _2;
    }
    if(isSymbol(cd->symbol, L'}')) {
        NextSym(cd);
        Ignore(cd);
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _4;
    }
    Err(cd, L"isStruct: Expected '}' or','");
    return false;
_4:
    if(isSymbol(cd->symbol, L'@')) {
        NextSym(cd);
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        erFlag = true;
        goto _5;
    }
    // Откат или сообщение об ошибке
    if(erFlag) {
        Err(cd, L"isStruct: Expected '@'");
    } else {
        // Если не целое, то возможна структура. Откат назад
        pos = ipos;
        symbol = isymbol;
        line = iline;
        column = icolumn;
    }
    return false;
_5:
    if(isQualId()) { //
        Ignore(cd);
        goto _6;
    }
    if(isId()) { //
        Ignore(cd);
        goto _6;
    }
    Err(cd, L"isStruct: Expected type name");
    return false;
_6:
    if(isSymbol(cd->symbol, L',')) {
        NextSym(cd);
        Ignore(cd);
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _1;
    }
    if(isSymbol(cd->symbol, L')')) {
        NextSym(cd);
        Ignore(cd);
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _end;
    }
    Err(cd, L"isStruct: Expected ',' or')'");
    return false;
_end:
    return true;
}

//--------------------------------------------------------------------------
// Определение кортежа.
_Bool Compiler::isTuple(Type **pptv) {
    ////auto fieldCounter = 0;  // Счетчика повторяющихся полей
    // Вектор для накопления внутренних объявлений кортежа
    std::vector<DeclarationType*> tmpVec;
    // Сохранение позиции для возможного отката назад
    auto ipos{pos};
    auto isymbol{symbol};
    auto iline{line};
    auto icolumn{column};
    auto erFlag = false;     // флаг для отката или ошибки
//_0:
    if(isSymbol(cd->symbol, L'(')) {
        NextSym(cd);
        Ignore(cd);
        goto _1;
    }
    return false;
_1:
    if(isQualId()) { // Фиксирует как обычный ид, так и квалифицированный (по счетчику)
        // Проверка на то, что это объявление типа
        DeclarationType* pdt = sm.FindTypeDeclaration(lexValue);
        if(pdt != NULL) {
            // Тип добавляется к временному списку типов кортежа
            tmpVec.push_back(pdt);
        }
        Ignore(cd);
        erFlag = true;
        goto _5;
    }
    if(isId()) { // Локальный тип
        Ignore(cd);
        goto _5;
    }
    // Откат или сообщение об ошибке
    if(erFlag) {
        Err(cd, L"isTuple: typeName or expected '('");
    } else {
        // Если не целое, то возможна структура. Откат назад
        pos = ipos;
        symbol = isymbol;
        line = iline;
        column = icolumn;
    }
    return false;
_2:
    if(isInteger()) { // несколько однотипных полей
        Ignore(cd);
        goto _3;
    }
    Err(cd, L"isTuple: Expected integer (number of same fields)");
    return false;
_3:
    if(isSymbol(cd->symbol, L')')) {
        NextSym(cd);
        Ignore(cd);
        goto _4;
    }
    Err(cd, L"isTuple: Expected ')'");
    return false;
_4:
    if(isQualId()) { // Импортируемый тип
        Ignore(cd);
        goto _5;
    }
    if(isId()) { // Локальный тип
        Ignore(cd);
        goto _5;
    }
    // Откат или сообщение об ошибке
    if(erFlag) {
        Err(cd, L"isTuple: Expected type name");
    } else {
        // Если не целое, то возможна структура. Откат назад
        pos = ipos;
        symbol = isymbol;
        line = iline;
        column = icolumn;
    }
    return false;
_5:
    if(isSymbol(cd->symbol, L',')) {
        NextSym(cd);
        Ignore(cd);
        goto _1;
    }
    if(isSymbol(cd->symbol, L')')) {
        NextSym(cd);
        Ignore(cd);
        goto _end;
    }
    Err(cd, L"isStruct: Expected ',' or')'");
    return false;
_end:
    // Формирование кортежа
    TypeTuple* typeTuple = sm.NewTuple();
    // Заполнение полей кортежа
    for(auto it: tmpVec) {
        typeTuple->AddType(it);
    }
    *pptv = typeTuple;
    return true;
}

//--------------------------------------------------------------------------
// Определение роя.
_Bool Compiler::isSwarm(Type** pptv) {
//_0:
    if(isSymbol(cd->symbol, L'[')) {
        NextSym(cd);
        Ignore(cd);
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _1;
    }
    return false;
_1:
    if(isInteger()) { // размерность роя
        Ignore(cd);
        goto _2;
    }
    Err(cd, L"isSwarm: A size of swarm type have been expected");
    return false;
_2:
    if(isSymbol(cd->symbol, L']')) {
        NextSym(cd);
        Ignore(cd);
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _3;
    }
    Err(cd, L"isSwarm: swarm type must have symbol ']'");
    return false;
_3:
    if(isBaseType(pptv)) {
        Ignore(cd);
        goto _end;
    }
    // Остальные типы будут дописаны позднее
    Err(cd, L"isSwarm: swarm type must have type of elements");
    return false;
_end:
    return true;
}

//--------------------------------------------------------------------------
// Определение импорта.
_Bool Compiler::isImportDefinition() {

    return false;
}
*/
//--------------------------------------------------------------------------
// Определение константного выражения.
_Bool isConstExpression(CompilerData *cd) {
//_0:
    if(isConstTerm(cd)) {
        goto _1;
    }
    return false;
_1:
    if(isSymbol(cd->symbol, L':')) {
        NextSym(cd);
        Ignore(cd);
        goto _2;
    }
    if(isSymbol(cd->symbol, L'^')) {
        NextSym(cd);
        Ignore(cd);
        goto _3;
    }
    goto _end;
_2:
    if(isConstTerm(cd)) {
        goto _1;
    }
    Err(cd, L"ConstExpression: A constant term have been expected");
    return false;
_3:
    if(isConstExpression(cd)) {
        goto _end;
    }
    Err(cd, L"ConstExpression: A constant expression have been expected");
    return false;
_end:
    return true;
}

//--------------------------------------------------------------------------
// Константный терм, содержащий константы и константные операции
_Bool isConstTerm(CompilerData *cd) {
    ///fmt.Println("ConstTerm")
//_0:
    if(isAtom(cd)) {
        goto _end;
    }
    /// Остальные варианты термов будут доопределяться по ходу разработки
    /// ConstDataList
    /// ConstParList
    /// String
    return false;
_end:
    return true;
}

//--------------------------------------------------------------------------
// Атомарная константа
_Bool isAtom(CompilerData *cd) {
    // Const* pcv;
    ///fmt.Println("Atom")
    //ok := false
    //!!!! Возникла мысль о том, что не имеет смысла использовать сигналы
    //!!!! как именованные константы, так как они не имеют альтернативных
    //!!!! значений. Поэтому их можно вынести из константных величин
    if(isSymbol(cd->symbol, L'!')) { // Сигнальная константа
        NextSym(cd);
        Ignore(cd);
        // pcv = sm.NewSignalValue();
        goto _end;
    }
    if(isBoolean(cd)) {
        //! Формирование значения булевой константы в виде элемента семантической модели
        // _Bool bv = false;
        // if(lexValue == "true") {
        //     bv = true;
        // } else if(lexValue == "false") {
        //     bv = false;
        // }
        // Формирование значения для данной константы
        // pcv = sm.NewBoolValue(bv);
        goto _end;
    }
    // if(isFloat(cd)) {
        // double fv = std::stod(lexValue);
        // Формирование значения для данной константы
        // pcv = sm.NewFloatValue(fv);
        // Формирование значения действительной константы в виде элемента семантической модели
        //fv, _ := strconv.ParseFloat(Value, 64)
        // Формирование значения для данной константы
        //floatValue := smodel.NewFloatValue(fv)
        // Тестовый вывод константы как значения
        //fmt.Println("Float const floatValue (isAtom):", floatValue)
        //!* = floatValue
        // Тестовый вывод константы как значения
        //fmt.Println("Float const  (isAtom):", *)
        // goto _end;
    // }
    if(isInteger(cd)) {
        // int iv = std::stoi(lexValue);
        // Формирование значения для данной константы
        // pcv = sm.NewIntValue(iv);
        // Формирование значения целочисленной константы в виде элемента семантической модели
        //iv, _ := strconv.Atoi(Value)
        // Формирование значения для данной константы
        //intValue := smodel.NewIntValue(iv)
        // Тестовый вывод константы как значения
        //fmt.Println("Int const intValue (isAtom):", intValue)
        //!* = intValue
        // Тестовый вывод константы как значения
        //fmt.Println("Int const  (isAtom):", *)
        goto _end;
    }
    // if(isChar(cd)) {
        // Формирование значения целочисленной константы
        // в виде элемента семантической модели
        // char cv = lexValue[0];
        // pcv = sm.NewCharValue(cv);
        // goto _end;
    // }
    //fmt.Println("Symbol ==", string(Symbol))
    /// Остальные варианты атомов будут доопределяться по ходу разработки
    /// SpecSymbol
    /// NamedConst
    /// IntervalConst
    return false;
_end:
    // *ppcv = pcv;
    return true;
}

/*
//--------------------------------------------------------------------------
// Константный вектор (список данных)
_Bool Compiler::isConstDataList() {
    return false;
}

//--------------------------------------------------------------------------
// Константная группа (параллельный список)
_Bool Compiler::isConstParList() {
    return false;
}

//--------------------------------------------------------------------------
// Строка символов
//! Пока заморожена, так как символы не входят в основные типы данных.
_Bool Compiler::isString() {
    return false;
}
*/