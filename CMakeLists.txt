# (1)
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)

project (Npolapp)
set(useROOT true)

# (2)
#set(CMAKE_MODULE_PATH /opt/geant4/geant4.10.02.p01/share/Geant4-10.2.1/bin)
option(WITH_GEANT4_UIVIS "Build project with Geant4 UI an Vis Drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED gdml ui_all vis_all)
else()
  find_package(Geant4 REQUIRED gdml)
endif()
include(${Geant4_USE_FILE})
message("    -> GEANT4 Include directory is: ${Geant4_INCLUDE_DIR}")
#message("    -> GEANT4 Library Directory is: ${Geant4_LIBRARY_DIR}")

if(useROOT)
  add_definitions(-DG4ANALYSIS_USE_ROOT=1)
  set(myROOTclasses ${PROJECT_SOURCE_DIR}/include/NpolStep.hh ${PROJECT_SOURCE_DIR}/include/NpolTagger.hh ${PROJECT_SOURCE_DIR}/include/NpolVertex.hh ${PROJECT_SOURCE_DIR}/include/NpolStatistics.hh)

  # Load ROOT and setup include directory
  #set(CMAKE_MODULE_PATH /usr/local/lib/Geant4-10.2.1/Modules)
  find_package(ROOT REQUIRED)
  message("    -> ROOT Include directory is: ${ROOT_INCLUDE_DIR}")
  message("    -> ROOT Library Directory is: ${ROOT_LIBRARY_DIR}")
  include_directories(${ROOT_INCLUDE_DIR})

  if(myROOTclasses)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/dict.cxx ${CMAKE_CURRENT_BINARY_DIR}/dict.h
      #COMMAND rootcint -f ${CMAKE_CURRENT_BINARY_DIR}/dict.cxx -c -p
       COMMAND rootcling -f ${CMAKE_CURRENT_BINARY_DIR}/dict.cxx -c -p
	      -I${Geant4_INCLUDE_DIR}
          ${myROOTclasses} ${PROJECT_SOURCE_DIR}/include/linkdef.hh
      DEPENDS ${PROJECT_SOURCE_DIR}/include/linkdef.hh ${myROOTclasses}
    )
    set(sources ${sources} ${CMAKE_CURRENT_BINARY_DIR}/dict.cxx)
  endif(myROOTclasses)
endif(useROOT)

# (3)
include_directories(${PROJECT_SOURCE_DIR}/include)

# (4)
# NB: headers only included here so some IDEs will pick them up. cmake already
#     knows about them and the associated dependencies
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB newsources ${PROJECT_SOURCE_DIR}/src/*.cc)
set(sources ${sources} ${newsources})

# (5)
add_executable(Npolapp Npolapp.cc ${sources} ${headers})
target_link_libraries(Npolapp ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

# (6)
set(Npolapp_FILES
  macros/npol.mac macros/init.mac macros/init_vis.mac macros/vis.mac 
  macros/ParticleFlux.mac macros/ElectronBeam.mac
)

set(Npolapp_DIRS
  gdml/
  root/
  output/
  scripts/
  macros/
)

foreach(_dir ${Npolapp_DIRS})
  file( MAKE_DIRECTORY ${_dir})
  file( COPY ${_dir} DESTINATION ${_dir})
endforeach()

foreach(_file ${Npolapp_FILES})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_file}
    ${PROJECT_BINARY_DIR}/${_file}
    COPYONLY
    )
endforeach()

# (extra)
#add_custom_target(Npolapp DEPENDS Npolapp)

# (7)
install(TARGETS Npolapp DESTINATION bin)
