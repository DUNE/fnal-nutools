
set(
          LIB_LIBRARIES SimulationBase
	                MagneticField_service
	                ${G4FR}
	                ${G4GMOCREN}
	                ${G4RAYTRACER}
	                ${G4TREE}
	                ${G4VRML}
	                ${G4EVENT}
	                ${G4GEOMETRY}
	                ${G4GLOBAL}
	                ${G4INTERCOMS}
	                ${G4MATERIALS}
	                ${G4MODELING}
	                ${G4PARTICLES}
	                ${G4PERSISTENCY}
	                ${G4PHYSICSLISTS}
	                ${G4PROCESSES}
	                ${G4READOUT}
	                ${G4RUN}
	                ${G4TRACKING}
	                ${G4VISHEPREP}
	                ${G4VIS_MANAGEMENT}
			${XERCESC}
	                ${CLHEP}
			)

include_directories(
     ${Geant4_INCLUDE_DIR}/..
     ${ROOT_INCLUDE_DIRS}
     ${CLHEP_INCLUDE_DIRS}
     ${Geant4_INCLUDE_DIRS}
     ${Geant4_DIR}/../../include
     )

set(G4Base_HEADERS
     ConvertMCTruthToG4.h
     DetectorConstruction.h
     ExampleAction.h
     G4Helper.h
     PrimaryParticleInformation.h
     UserAction.h
     UserActionFactory.h
     UserActionManager.h
     G4PhysListFactory.hh
     G4PhysListFactorySingleton.hh
     G4PhysicsProcessFactorySingleton.hh
     )

add_library(G4Base SHARED
     ${G4Base_HEADERS}
     ConvertMCTruthToG4.cxx
     DetectorConstruction.cxx
     ExampleAction.cxx
     G4Helper.cxx
     PrimaryParticleInformation.cxx
     UserActionFactory.cxx
     UserActionManager.cxx
     G4PhysListFactory.cc
     G4PhysListFactorySingleton.cc
     G4PhysicsProcessFactorySingleton.cc
     )

target_link_libraries(G4Base
    ${LIB_LIBRARIES}
     )


install(TARGETS
     G4Base
     EXPORT nutoolsLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime 
     )

install(FILES ${G4Base_HEADERS} DESTINATION
     ${CMAKE_INSTALL_INCLUDEDIR}/G4Base COMPONENT Development)


