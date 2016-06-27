cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project (Npolapp)
message("Configuring project Npolapp")

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

# include Geant4
find_package(Geant4 REQUIRED gdml)
include(${Geant4_USE_FILE})
message("    -> GEANT4 Include directory is: ${Geant4_INCLUDE_DIR}")

# include ROOT
find_package(ROOT REQUIRED)
message("    -> ROOT Include directory is: ${ROOT_INCLUDE_DIR}")
message("    -> ROOT Library Directory is: ${ROOT_LIBRARY_DIR}")
include_directories(${ROOT_INCLUDE_DIR})

# set up directories for data files, etc.
file(COPY ${PROJECT_SOURCE_DIR}/gdml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/macros DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/scripts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/output)

add_executable(Npolapp ${sources})
target_link_libraries(Npolapp NpolClasses ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
