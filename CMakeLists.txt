cmake_minimum_required(VERSION 3.20)
project(ProjectProg C)

set(CMAKE_C_STANDARD 99)

add_executable(ProjectProg main.c validation.c validation.h helpers.c helpers.h sort.c sort.h employee.h)
