cmake_minimum_required(VERSION 3.17)
project(Part1 C)

set(CMAKE_C_STANDARD 99)

add_executable(Part1 main.c functions.c functions.h dtd.c xml.c)