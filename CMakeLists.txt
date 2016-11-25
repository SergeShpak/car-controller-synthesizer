cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME pid)

project(${PROJECT_NAME})

execute_process(COMMAND pkg-config --cflags ibex
                OUTPUT_VARIABLE IBEX_FLAGS
                OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND pkg-config --libs ibex
                OUTPUT_VARIABLE IBEX_LIBS
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -pg -Wall -frounding-math -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${IBEX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${IBEX_LIBS}")
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${IBEX_LIBS}" )

message("++ ${CMAKE_EXE_LINKER_FLAGS}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

link_directories(/usr/local/lib/)
target_link_libraries(${PROJECT_NAME} ibex Clp CoinUtils z m prim)
