cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project (NpolLib)
message("Configuring project NpolLib")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

# include ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
message("    -> ROOT Include directory is: ${ROOT_INCLUDE_DIR}")
message("    -> ROOT Library Directory is: ${ROOT_LIBRARY_DIR}")
include_directories(${ROOT_INCLUDE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

ROOT_GENERATE_DICTIONARY(NpolClassDict ${headers} LINKDEF ${PROJECT_SOURCE_DIR}/linkdef.hh)

add_library(NpolClasses SHARED NpolClassDict.cxx ${sources})
target_link_libraries(NpolClasses ${ROOT_LIBRARIES})
target_include_directories(NpolClasses PUBLIC include)
