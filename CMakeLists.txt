cmake_minimum_required(VERSION 3.3)
project(mist)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        builtin_func.cpp
        builtin_func.h
    Env.cpp
    Env.h
    eval.cpp
    eval.h
    LObj.cpp
    LObj.h
    main.cpp
    print.cpp
    print.h
    read.cpp
    read.h
    TypeDef.h builtin_symbols.cpp builtin_symbols.h)

add_executable(mist ${SOURCE_FILES})