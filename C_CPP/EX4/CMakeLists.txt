cmake_minimum_required(VERSION 3.17)
project(EX4_5 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(EX4_5 main.c
        vector.c pair.c hashmap.c test_suite.h test_suite.c test_pairs.h)
