cmake_minimum_required(VERSION 3.23)
project(Hmm C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-O3")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(Hmm src/HmmState.c src/HmmState.h src/Hmm1.c src/Hmm1.h src/Hmm.c src/Hmm.h src/Hmm2.c src/Hmm2.h)
target_link_libraries(Hmm ${CONAN_LIBS})
