cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project (NpolProcessEvents)
message("Configuring project NpolProcessEvents.")

set(CMAKE_CXX_FLAGS "-D_GLIBCXX_USE_CXX11_ABI=0 -Wall -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/include)
#include_directories(../npollib/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

# include ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
message("    -> ROOT Include directory is: ${ROOT_INCLUDE_DIR}")
message("    -> ROOT Library Directory is: ${ROOT_LIBRARY_DIR}")
include_directories(${ROOT_INCLUDE_DIR})

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/output)

add_executable(NpolProcessEvents ${sources})
target_link_libraries(NpolProcessEvents NpolClasses ${ROOT_LIBRARIES})
