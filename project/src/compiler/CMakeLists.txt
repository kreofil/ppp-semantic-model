# set minimum cmake version
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_C_COMPILER ppclang)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type is Debug" FORCE)

# Установка переменной со списком исходников
set(SOURCE_LIB
    # headers
    #expcomp.h
    parser.h

    #sources
    #expcomp.cpp
    util.c
    tlt.c
    scanner.c
    parser.c
)

add_library(compiler STATIC ${SOURCE_LIB})   # Создание статической библиотеки

# Линковка библиотеки с библиотекой семантической модели
target_link_libraries(compiler
    -Wl,--whole-archive
        smodel
    -Wl,--no-whole-archive
)
