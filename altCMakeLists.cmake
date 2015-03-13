cmake_minimum_required(VERSION 2.8.12)
if(POLICY CMP0025)
cmake_policy(SET CMP0025 OLD)
endif()
if(POLICY CMP0042)
cmake_policy(SET CMP0042 NEW)
endif()

project(nutools)
set(nutools_VERSION 1.07.00)


include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CheckCXXCompilerFlag)

set(BASE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/BuildProducts")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_LIBDIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -O3 -g -DNDEBUG -fno-omit-frame-pointer")

find_package(ROOT 5.34.20 REQUIRED)
set(ROOTCINT ${ROOT_rootcint_CMD})
set(ROOTSYS "${ROOT_DIR}/..")
find_package(FNALCore 0.1.0 REQUIRED)
find_package(art 1.11.3 REQUIRED)
include(artTools)
find_package(CLHEP 2.2.0.3 REQUIRED)
find_package(Geant4 9.6.3 REQUIRED)
find_package(Boost 1.55.0 REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(${art_INCLUDE_DIRS})
include_directories(${FNALCore_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${CLHEP_INCLUDE_DIRS})

message(STATUS "ROOTSYS is ${ROOTSYS}")
set( ROOT_INC ${ROOTSYS}/include )
message(STATUS "ROOT_INC is ${ROOT_INC}")

## find_library directives
# geant4 libraries
find_library( XERCESC NAMES xerces-c PATHS ${XERCESCROOT}/lib NO_DEFAULT_PATH  )
find_library( G4FR NAMES G4FR PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4GMOCREN NAMES G4GMocren PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4OPENGL NAMES G4OpenGL PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4RAYTRACER  NAMES G4RayTracer PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4TREE  NAMES G4Tree PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4VRML  NAMES G4VRML PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4ANALYSIS  NAMES G4analysis PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4DIGITS_HITS  NAMES G4digits_hits PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4ERROR_PROPAGATION  NAMES G4error_propagation PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4EVENT  NAMES G4event PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4GEOMETRY  NAMES G4geometry PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4GL2PS  NAMES G4gl2ps PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4GLOBAL  NAMES G4global PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4GRAPHICS_REPS  NAMES G4graphics_reps PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4INTERCOMS  NAMES G4intercoms PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4INTERFACES  NAMES G4interfaces PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4MATERIALS NAMES G4materials PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4MODELING  NAMES G4modeling PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4PARMODELS  NAMES G4parmodels PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4PARTICLES  NAMES G4particles PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4PERSISTENCY  NAMES G4persistency PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4PHYSICSLISTS  NAMES G4physicslists PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4PROCESSES  NAMES G4processes PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4READOUT  NAMES G4readout PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4RUN  NAMES G4run PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4TRACK  NAMES G4track PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4TRACKING  NAMES G4tracking PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4VISHEPREP NAMES G4visHepRep PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4VISXXX NAMES G4visXXX PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4VIS_MANAGEMENT NAMES G4vis_management PATHS  ${G4LIB} NO_DEFAULT_PATH )
find_library( G4ZLIB  NAMES G4zlib PATHS  ${G4LIB} NO_DEFAULT_PATH )
# other dependencies
find_library( PYTHIA6 NAMES Pythia6 PATHS  ${PYLIB} NO_DEFAULT_PATH )
find_library( LHAPDF NAMES LHAPDF PATHS ${LHAPDF_FQ_DIR}/lib NO_DEFAULT_PATH )
find_library( LOG4CPP NAMES log4cpp PATHS ${LOG4CPP_FQ_DIR}/lib NO_DEFAULT_PATH )
find_library( XML2 NAMES xml2 PATHS ${LIBXML2_FQ_DIR}/lib NO_DEFAULT_PATH )
find_library( CRY NAMES CRY PATHS ${CRYHOME}/lib NO_DEFAULT_PATH )
# genie
find_library( GALGORITHM NAMES GAlgorithm PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GBARYONRESONANCE NAMES GBaryonResonance PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GBASE NAMES GBase PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GBODEKYANG NAMES GBodekYang PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GCHARM NAMES GCharm PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GCOH NAMES GCoh PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GDFRC NAMES GDfrc PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GDIS NAMES GDIS PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GCROSSSECTIONS NAMES GCrossSections PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GDECAY NAMES GDecay PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GELAS NAMES GElas PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GELFF NAMES GElFF PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GHEP NAMES GHEP PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GEVGCORE NAMES GEVGCore  PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GEVGMODULES NAMES GEVGModules PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GEVGDRIVERS NAMES GEVGDrivers PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GGIBUU NAMES GGiBUU PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GHADRONTRANSP NAMES GHadronTransp PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GFRAGMENTATION NAMES GFragmentation PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GINTERACTION NAMES GInteraction PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GLLEWELLYNSMITH NAMES GLlewellynSmith  PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GMEC NAMES GMEC PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GMESSENGER NAMES GMessenger PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GNUGAMMA NAMES GNuGamma PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GNUE NAMES GNuE PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GNTUPLE NAMES GNtuple PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GNUCLEAR NAMES GNuclear PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GNUMERICAL NAMES GNumerical PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GQPM NAMES GQPM PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GPDG NAMES GPDG PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GPDF NAMES GPDF PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GQEL NAMES GQEL PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GRES NAMES GRES PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GREGISTRY NAMES GRegistry PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GREINSEGHAL NAMES GReinSeghal PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GUTILS NAMES GUtils PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GGEO NAMES GGeo PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GFLUXDRIVERS NAMES GFluxDrivers PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GMUELOSS NAMES GMuELoss PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GREWEIGHT NAMES GReWeight PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )
find_library( GNUCLEONDECAY NAMES GNucleonDecay PATHS  ${GENIE_LIB} NO_DEFAULT_PATH )

# include search path
include_directories ( ${CMAKE_SOURCE_DIR} )
include_directories ( ${GENIE_INC}/GENIE )
include_directories ( ${LIBXML2_FQ_DIR}/include/libxml2 )
include_directories ( ${GEANT4_FQ_DIR}/include )
include_directories ( ${XERCES_C_INC} )
include_directories ( ${CRYHOME}/src )
include_directories ( ${LOG4CPP_INC} )
# included for Mac OSX using XQuartz for X11, ignored on other systems
include_directories ( /opt/X11/include )

# source
add_subdirectory (EventDisplayBase)
add_subdirectory (EventGeneratorBase)
add_subdirectory (G4Base)
add_subdirectory (MagneticField)
add_subdirectory (NuBeamWeights)
add_subdirectory (NuReweight)
add_subdirectory (SimulationBase)
#add_subdirectory (dk2nu)

# ups - table and config files
add_subdirectory(ups)
# packaging utility
#include(UseCPack)

configure_package_config_file(
  Modules/nutoolsConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/nutoolsConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nutools-${nutools_VERSION}
  PATH_VARS
    CMAKE_INSTALL_INCLUDEDIR
    CMAKE_INSTALL_LIBDIR
  )

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/nutoolsConfigVersion.cmake
  VERSION ${nutools_VERSION}
  COMPATIBILITY AnyNewerVersion
  )

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/nutoolsConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/nutoolsConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nutools-${nutools_VERSION}
  COMPONENT Development
  )

install(EXPORT nutoolsLibraries
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nutools-${nutools_VERSION}
  NAMESPACE nutools::
  COMPONENT Development
  )

include(ArtCPack)
