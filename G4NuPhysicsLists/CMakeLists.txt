#

# setup UPS products for ROOT (and get Geant4 + gcc)
#
#  WHICH_G4=`ls -d ${GEANT4_FQ_DIR}/lib64/Geant4-*`
#  WHICH_CLHEP=`ls -d ${CLHEP_LIB_DIR}/CLHEP-*`
# 
#  cmake -DGeant4_DIR=${WHICH_G4} -DCLHEP_DIR=${WHICH_CLHEP}  ./
#
#
#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(G4NuPhysicsLists)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

find_package(CLHEP REQUIRED)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/G4NuPhysicsLists)
include_directories($ENV{XERCESCROOT}/include)
include_directories($ENV{ROOTSYS}/include)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories($ENV{GEANT4_FQ_DIR}/include)

link_directories($ENV{ROOTSYS}/lib)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_library(g4nupl SHARED ${sources})
target_link_libraries(g4nupl ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
#set(EXAMPLEG4SA_SCRIPTS
#  g4sa.mac
#  newgeom.mac
#  rndmSeed.mac
#  run_e.mac
#  )
#
#foreach(_script ${EXAMPLEG4SA_SCRIPTS})
#  configure_file(
#    ${PROJECT_SOURCE_DIR}/${_script}
#    ${PROJECT_BINARY_DIR}/${_script}
#    COPYONLY
#    )
#endforeach()
#
#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
#add_custom_target(G4SA DEPENDS g4sa)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
#install(TARGETS g4sa DESTINATION bin)


