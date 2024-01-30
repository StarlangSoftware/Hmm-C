cmake_minimum_required(VERSION 3.23)
project(Hmm)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3")

find_package(math_c REQUIRED)

add_library(Hmm HmmState.c HmmState.h Hmm1.c Hmm1.h Hmm.c Hmm.h Hmm2.c Hmm2.h)
target_link_libraries(Hmm math_c::math_c)
