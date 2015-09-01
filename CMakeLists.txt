# (1)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project (Npolapp)
set(useROOT true)

if(useROOT)
  add_definitions(-DG4ANALYSIS_USE_ROOT=1)
endif(useROOT)

# (2)
set(CMAKE_MODULE_PATH /usr/local/geant4/geant4.10.00.p01/share/Geant4-10.0.1/bin)
option(WITH_GEANT4_UIVIS "Build project with Geant4 UI an Vis Drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED gdml ui_all vis_all)
else()
  find_package(Geant4 REQUIRED gdml)
endif()
message("    -> GEANT4 Include directory is: ${GEANT4_INCLUDE_DIR}")
message("    -> GEANT4 Library Directory is: ${GEANT4_LIBRARY_DIR}")

# (3)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# (3) Load ROOT and setup include directory
set(CMAKE_MODULE_PATH /usr/local/lib/Geant4-10.0.1/Modules)
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})

# (3) Set up compiler flags if useROOT is set
if(useROOT)
  EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
endif(useROOT) 

# (4)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# (5)
add_executable(Npolapp Npolapp.cc ${sources} ${headers})
target_link_libraries(Npolapp ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

# (6)
set(Npolapp_SCRIPTS
  npol.mac init.mac init_vis.mac vis.mac NpolVertex_hh.so include/NpolVertex.hh gdml/HBender.gdml gdml/BeamDump.gdml
)

foreach(_script ${Npolapp_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

# (extra)
#add_custom_target(Npolapp DEPENDS Npolapp)

# (7)
install(TARGETS Npolapp DESTINATION bin)
