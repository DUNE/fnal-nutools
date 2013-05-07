# ======================================================================
#  nutools main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/nutools/ups/setup_for_development
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path]
#        -DCMAKE_BUILD_TYPE=$CETPKG_TYPE
#        $CETPKG_SOURCE
#        
#  make
#  make test
#  make install
#  make package (builds distribution tarfile)
# ======================================================================


# use cmake 2.8 or later
cmake_minimum_required (VERSION 2.8)

# cetbuildtools contains our cmake modules
SET ( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
IF (NOT CETBUILDTOOLS_VERSION)
    MESSAGE (FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules")
ENDIF()

# we need our own modules until we can use at least art v1_04_00
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Modules 
                      $ENV{ART_DIR}/Modules
		      $ENV{CETBUILDTOOLS_DIR}/Modules
		      ${CMAKE_MODULE_PATH})

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS CAUTIOUS EXTRA_FLAGS -pedantic -Wno-deprecated-declarations)
cet_have_qual( e2 )
if ( ${CET_HAVE_QUAL} )
  cet_add_compiler_flags(CXX -std=c++0x)
endif()
cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
find_ups_product( art v1_02_00 )
find_ups_product( genie v3665 )
find_ups_product( cetbuildtools v3_03_00 )

## these will be redundant once nutools migrates to art v1_03_08
find_ups_product( cppunit v1_12_1 )
find_ups_product( gccxml v0_9 )
find_ups_product( libsigcpp v2_2_10 )
find_ups_product( clhep v2_1_2_2 )
find_ups_product( cpp0x v1_03_00 )
find_ups_product( cetlib v1_03_13 )
find_ups_product( fhiclcpp v2_17_00 )
find_ups_product( messagefacility v1_10_00 )
find_ups_product( sqlite v3_07_08_00 )
find_ups_root( v5_34_00 )
find_ups_boost( v1_47_0 )

message(STATUS "ROOTSYS is ${ROOTSYS}")
set( ROOT_INC ${ROOTSYS}/include )
message(STATUS "ROOT_INC is ${ROOT_INC}")

cet_check_gcc()

## find_library directives
## these will be redundant once nutools migrates to art v1_03_08
cet_find_library( CETLIB NAMES cetlib PATHS ENV CETLIB_LIB )
cet_find_library( FHICLCPP NAMES fhiclcpp PATHS ENV FHICLCPP_LIB )
cet_find_library( MF_EXTMODULES NAMES MF_ExtModules PATHS ENV MESSAGEFACILITY_LIB )
cet_find_library( MF_MESSAGELOGGER NAMES MF_MessageLogger PATHS ENV MESSAGEFACILITY_LIB )
cet_find_library( MF_UTILITIES NAMES MF_Utilities PATHS ENV MESSAGEFACILITY_LIB )
cet_find_library( CLHEP NAMES CLHEP PATHS ENV CLHEP_LIB_DIR NO_DEFAULT_PATH )
cet_find_library( SIGC NAMES sigc-2.0 PATHS ENV LIBSIGCPP_LIB NO_DEFAULT_PATH )
cet_find_library( CPPUNIT NAMES cppunit PATHS ENV CPPUNIT_LIB NO_DEFAULT_PATH )
cet_find_library( SQLITE3 NAMES sqlite3 PATHS ENV SQLITE_LIB NO_DEFAULT_PATH )
cet_find_library( ART_VERSION NAMES art_Version PATHS ENV ART_LIB )
cet_find_library( ART_UTILITIES NAMES art_Utilities PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_ART NAMES art_Framework_Art PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_CORE NAMES art_Framework_Core PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_IO_CATALOG NAMES art_Framework_IO_Catalog PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_IO_PRODUCTMIX NAMES art_Framework_IO_ProductMix PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_IO_PRODUCTMIX_DICT NAMES art_Framework_IO_ProductMix_dict PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_IO_ROOTVERSION NAMES art_Framework_IO_RootVersion PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_IO_ROOT NAMES art_Framework_IO_Root PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_IO_ROOT_ROOTINPUT_SOURCE NAMES art_Framework_IO_Root_RootInput_source PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_IO_ROOT_ROOTOUTPUT_MODULE NAMES art_Framework_IO_Root_RootOutput_module PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_IO_SOURCES NAMES art_Framework_IO_Sources PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_MODULES_EMPTYEVENT_SOURCE NAMES art_Framework_Modules_EmptyEvent_source PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_MODULES_FILEDUMPEROUTPUT_MODULE NAMES art_Framework_Modules_FileDumperOutput_module PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_MODULES_PRESCALER_MODULE NAMES art_Framework_Modules_Prescaler_module PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_MODULES_PROVENANCECHECKEROUTPUT_MODULE NAMES art_Framework_Modules_ProvenanceCheckerOutput_module PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_MODULES_RANDOMNUMBERSAVER_MODULE NAMES art_Framework_Modules_RandomNumberSaver_module PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_MODULES_DICT NAMES art_Framework_Modules_dict PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_PRINCIPAL NAMES art_Framework_Principal PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_FILESERVICEINTERFACES NAMES art_Framework_Services_FileServiceInterfaces PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_OPTIONAL NAMES art_Framework_Services_Optional PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_OPTIONAL_RANDOMNUMBERGENERATOR_SERVICE NAMES art_Framework_Services_Optional_RandomNumberGenerator_service PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_OPTIONAL_SIMPLEINTERACTION_SERVICE NAMES art_Framework_Services_Optional_SimpleInteraction_service PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_OPTIONAL_SIMPLEMEMORYCHECK_SERVICE NAMES art_Framework_Services_Optional_SimpleMemoryCheck_service PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE NAMES art_Framework_Services_Optional_TFileService_service PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_OPTIONAL_TIMING_SERVICE NAMES art_Framework_Services_Optional_Timing_service PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_OPTIONAL_TRACER_SERVICE NAMES art_Framework_Services_Optional_Tracer_service PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_OPTIONAL_TRIVIALFILEDELIVERY_SERVICE NAMES art_Framework_Services_Optional_TrivialFileDelivery_service PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_OPTIONAL_TRIVIALFILETRANSFER_SERVICE NAMES art_Framework_Services_Optional_TrivialFileTransfer_service PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_REGISTRY NAMES art_Framework_Services_Registry PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_SYSTEM_CURRENTMODULE_SERVICE NAMES art_Framework_Services_System_CurrentModule_service PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_SYSTEM_FILECATALOGMETADATA_SERVICE NAMES art_Framework_Services_System_FileCatalogMetadata_service PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_SYSTEM_FLOATINGPOINTCONTROL_SERVICE NAMES art_Framework_Services_System_FloatingPointControl_service PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_SYSTEM_TRIGGERNAMESSERVICE_SERVICE NAMES art_Framework_Services_System_TriggerNamesService_service PATHS ENV ART_LIB )
cet_find_library( ART_FRAMEWORK_SERVICES_USERINTERACTION NAMES art_Framework_Services_UserInteraction PATHS ENV ART_LIB )
cet_find_library( ART_PERSISTENCY_CLHEPDICTIONARIES_DICT NAMES art_Persistency_CLHEPDictionaries_dict PATHS ENV ART_LIB )
cet_find_library( ART_PERSISTENCY_COMMON NAMES art_Persistency_Common PATHS ENV ART_LIB )
cet_find_library( ART_PERSISTENCY_COMMON_DICT NAMES art_Persistency_Common_dict PATHS ENV ART_LIB )
cet_find_library( ART_PERSISTENCY_PROVENANCE NAMES art_Persistency_Provenance PATHS ENV ART_LIB )
cet_find_library( ART_PERSISTENCY_PROVENANCE_DICT NAMES art_Persistency_Provenance_dict PATHS ENV ART_LIB )
cet_find_library( ART_PERSISTENCY_ROOTDB NAMES art_Persistency_RootDB PATHS ENV ART_LIB )
cet_find_library( ART_PERSISTENCY_STDDICTIONARIES_DICT NAMES art_Persistency_StdDictionaries_dict PATHS ENV ART_LIB )
cet_find_library( ART_PERSISTENCY_WRAPPEDSTDDICTIONARIES_DICT NAMES art_Persistency_WrappedStdDictionaries_dict PATHS ENV ART_LIB )
cet_find_library( ART_PERSISTENCY_CETLIBDICTIONARIES_DICT NAMES art_Persistency_CetlibDictionaries_dict PATHS ENV ART_LIB )
cet_find_library( ART_PERSISTENCY_FHICLCPPDICTIONARIES_DICT NAMES art_Persistency_FhiclCppDictionaries_dict PATHS ENV ART_LIB )
cet_find_library( TEST_FRAMEWORK_PRINCIPAL_DICT NAMES test_Framework_Principal_dict PATHS ENV ART_LIB )
cet_find_library( TECH-TESTBED NAMES tech-testbed PATHS ENV ART_LIB )

# geant4 libraries
find_library( XERCESC NAMES xerces-c PATHS $ENV{XERCESCROOT}/lib  )
cet_find_library( G4FR NAMES G4FR PATHS ENV G4LIB )
cet_find_library( G4GMOCREN NAMES G4GMocren PATHS ENV G4LIB )
cet_find_library( G4OPENGL NAMES G4OpenGL PATHS ENV G4LIB )
cet_find_library( G4RAYTRACER  NAMES G4RayTracer PATHS ENV G4LIB )
cet_find_library( G4TREE  NAMES G4Tree PATHS ENV G4LIB )
cet_find_library( G4VRML  NAMES G4VRML PATHS ENV G4LIB )
cet_find_library( G4ANALYSIS  NAMES G4analysis PATHS ENV G4LIB )
cet_find_library( G4DIGITS_HITS  NAMES G4digits_hits PATHS ENV G4LIB )
cet_find_library( G4ERROR_PROPAGATION  NAMES G4error_propagation PATHS ENV G4LIB )
cet_find_library( G4EVENT  NAMES G4event PATHS ENV G4LIB )
cet_find_library( G4GEOMETRY  NAMES G4geometry PATHS ENV G4LIB )
cet_find_library( G4GL2PS  NAMES G4gl2ps PATHS ENV G4LIB )
cet_find_library( G4GLOBAL  NAMES G4global PATHS ENV G4LIB )
cet_find_library( G4GRAPHICS_REPS  NAMES G4graphics_reps PATHS ENV G4LIB )
cet_find_library( G4INTERCOMS  NAMES G4intercoms PATHS ENV G4LIB )
cet_find_library( G4INTERFACES  NAMES G4interfaces PATHS ENV G4LIB )
cet_find_library( G4MATERIALS NAMES G4materials PATHS ENV G4LIB )
cet_find_library( G4MODELING  NAMES G4modeling PATHS ENV G4LIB )
cet_find_library( G4PARMODELS  NAMES G4parmodels PATHS ENV G4LIB )
cet_find_library( G4PARTICLES  NAMES G4particles PATHS ENV G4LIB )
cet_find_library( G4PERSISTENCY  NAMES G4persistency PATHS ENV G4LIB )
cet_find_library( G4PHYSICSLISTS  NAMES G4physicslists PATHS ENV G4LIB )
cet_find_library( G4PROCESSES  NAMES G4processes PATHS ENV G4LIB )
cet_find_library( G4READOUT  NAMES G4readout PATHS ENV G4LIB )
cet_find_library( G4RUN  NAMES G4run PATHS ENV G4LIB )
cet_find_library( G4TRACK  NAMES G4track PATHS ENV G4LIB )
cet_find_library( G4TRACKING  NAMES G4tracking PATHS ENV G4LIB )
cet_find_library( G4VISHEPREP NAMES G4visHepRep PATHS ENV G4LIB )
cet_find_library( G4VISXXX NAMES G4visXXX PATHS ENV G4LIB )
cet_find_library( G4VIS_MANAGEMENT NAMES G4vis_management PATHS ENV G4LIB )
cet_find_library( G4ZLIB  NAMES G4zlib PATHS ENV G4LIB )

# other dependencies
cet_find_library( PYTHIA6 NAMES Pythia6 PATHS ENV PYLIB )
find_library( LHAPDF NAMES LHAPDF PATHS $ENV{LHAPDF_FQ_DIR}/lib )
find_library( LOG4CPP NAMES log4cpp PATHS $ENV{LOG4CPP_FQ_DIR}/lib )
find_library( XML2 NAMES xml2 PATHS $ENV{LIBXML2_FQ_DIR}/lib )
find_library( CRY NAMES CRY PATHS $ENV{CRYHOME}/lib )
# genie
cet_find_library( GALGORITHM NAMES GAlgorithm PATHS ENV GENIE_LIB )
cet_find_library( GBARYONRESONANCE NAMES GBaryonResonance PATHS ENV GENIE_LIB )
cet_find_library( GBASE NAMES GBase PATHS ENV GENIE_LIB )
cet_find_library( GBODEKYANG NAMES GBodekYang PATHS ENV GENIE_LIB )
cet_find_library( GCHARM NAMES GCharm PATHS ENV GENIE_LIB )
cet_find_library( GCOH NAMES GCoh PATHS ENV GENIE_LIB )
cet_find_library( GDFRC NAMES GDfrc PATHS ENV GENIE_LIB )
cet_find_library( GDIS NAMES GDIS PATHS ENV GENIE_LIB )
cet_find_library( GCROSSSECTIONS NAMES GCrossSections PATHS ENV GENIE_LIB )
cet_find_library( GDECAY NAMES GDecay PATHS ENV GENIE_LIB )
cet_find_library( GELAS NAMES GElas PATHS ENV GENIE_LIB )
cet_find_library( GELFF NAMES GElFF PATHS ENV GENIE_LIB )
cet_find_library( GHEP NAMES GHEP PATHS ENV GENIE_LIB )
cet_find_library( GEVGCORE NAMES GEVGCore  PATHS ENV GENIE_LIB )
cet_find_library( GEVGMODULES NAMES GEVGModules PATHS ENV GENIE_LIB )
cet_find_library( GEVGDRIVERS NAMES GEVGDrivers PATHS ENV GENIE_LIB )
cet_find_library( GGIBUU NAMES GGiBUU PATHS ENV GENIE_LIB )
cet_find_library( GHADRONTRANSP NAMES GHadronTransp PATHS ENV GENIE_LIB )
cet_find_library( GFRAGMENTATION NAMES GFragmentation PATHS ENV GENIE_LIB )
cet_find_library( GINTERACTION NAMES GInteraction PATHS ENV GENIE_LIB )
cet_find_library( GLLEWELLYNSMITH NAMES GLlewellynSmith  PATHS ENV GENIE_LIB )
cet_find_library( GMEC NAMES GMEC PATHS ENV GENIE_LIB )
cet_find_library( GMESSENGER NAMES GMessenger PATHS ENV GENIE_LIB )
cet_find_library( GNUGAMMA NAMES GNuGamma PATHS ENV GENIE_LIB )
cet_find_library( GNUE NAMES GNuE PATHS ENV GENIE_LIB )
cet_find_library( GNTUPLE NAMES GNtuple PATHS ENV GENIE_LIB )
cet_find_library( GNUCLEAR NAMES GNuclear PATHS ENV GENIE_LIB )
cet_find_library( GNUMERICAL NAMES GNumerical PATHS ENV GENIE_LIB )
cet_find_library( GQPM NAMES GQPM PATHS ENV GENIE_LIB )
cet_find_library( GPDG NAMES GPDG PATHS ENV GENIE_LIB )
cet_find_library( GPDF NAMES GPDF PATHS ENV GENIE_LIB )
cet_find_library( GQEL NAMES GQEL PATHS ENV GENIE_LIB )
cet_find_library( GRES NAMES GRES PATHS ENV GENIE_LIB )
cet_find_library( GREGISTRY NAMES GRegistry PATHS ENV GENIE_LIB )
cet_find_library( GREINSEGHAL NAMES GReinSeghal PATHS ENV GENIE_LIB )
cet_find_library( GUTILS NAMES GUtils PATHS ENV GENIE_LIB )
cet_find_library( GGEO NAMES GGeo PATHS ENV GENIE_LIB )
cet_find_library( GFLUXDRIVERS NAMES GFluxDrivers PATHS ENV GENIE_LIB )
cet_find_library( GMUELOSS NAMES GMuELoss PATHS ENV GENIE_LIB )
cet_find_library( GREWEIGHT NAMES GReWeight PATHS ENV GENIE_LIB )
cet_find_library( GNUCLEONDECAY NAMES GNucleonDecay PATHS ENV GENIE_LIB )

# macros 
include(ArtMake)
include(CetMake)
include(ArtDictionary)
include(BuildPlugins)

# need to add -I.../nutools to the header list
include_directories ( $ENV{LIBSIGCPP_INC}/sigc++-2.0 )
include_directories ( $ENV{LIBSIGCPP_LIB}/sigc++-2.0/include )
include_directories ( ${CMAKE_SOURCE_DIR}/nutools )
include_directories ( $ENV{GENIE_INC}/GENIE )
include_directories ( $ENV{LIBXML2_FQ_DIR}/include/libxml2 )
include_directories ( $ENV{GEANT4_FQ_DIR}/include )
include_directories ( $ENV{XERCES_C_INC} )
include_directories ( $ENV{CRYHOME}/src )

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
include(UseCPack)
