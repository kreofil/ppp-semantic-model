# set minimum cmake version
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_C_COMPILER ppclang)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type is Debug" FORCE)

# Установка переменной со списком исходников
set(SOURCE_LIB
    # headers
    constant.h
    type.h
    variable.h
    operand.h
    instruction.h
    context.h
    name-table.h
    global.h

    #sources
    constant.c
    type.c
    variable.c
    operand.c
    instruction-dbg-out.c
    instruction-create-01.c
    instruction-create-02.c
    instruction-list.c
    context.c
    name-table.c
    global.c
)

add_library(smodel STATIC ${SOURCE_LIB})   # Создание статической библиотеки

