cmake_minimum_required(VERSION 3.20)
project(c_parser)

set(CMAKE_CXX_STANDARD 14)

add_executable(c_parser
        Cparser.cpp
        globals.h
        parse.cpp
        parse.h
        scan.cpp
        scan.h
        util.cpp
        util.h)
