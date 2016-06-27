cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project (NpolAnalysis)
message("Configuring project NpolAnalysis.")

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

# include ROOT
find_package(ROOT REQUIRED)
message("    -> ROOT Include directory is: ${ROOT_INCLUDE_DIR}")
message("    -> ROOT Library Directory is: ${ROOT_LIBRARY_DIR}")
include_directories(${ROOT_INCLUDE_DIR})

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/output)

add_executable(NpolAnalysis ${sources})
target_link_libraries(NpolAnalysis NpolClasses ${ROOT_LIBRARIES})
