include(artTools)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories ( ${ROOT_INCLUDE_DIRS} )


set( SIMB_LIBS ${ART_FRAMEWORK_SERVICES_REGISTRY}
               ${ART_FRAMEWORK_CORE}
               ${ART_FRAMEWORK_IO_ROOT_ROOTINPUT_SOURCE}
	       ${ART_PERSISTENCY_COMMON}
               ${ART_FRAMEWORK_IO_SOURCES}
               ${ART_FRAMEWORK_PRINCIPAL}
               ${ART_PERSISTENCY_PROVENANCE}
               ${ART_UTILITIES}
               ${MF_MESSAGELOGGER}
               ${MF_UTILITIES}
               ${FHICLCPP}
               ${CETLIB}
	       ${ROOT_EG}
	       ${ROOT_CORE}
	       ${ROOT_CINT} 
	       ${ROOT_RIO}
	       ${ROOT_NET}
	       ${ROOT_HIST} 
	       ${ROOT_GRAF}
	       ${ROOT_GRAF3D}
	       ${ROOT_GPAD}
	       ${ROOT_TREE}
	       ${ROOT_RINT}
	       ${ROOT_POSTSCRIPT}
	       ${ROOT_MATRIX}
	       ${ROOT_PHYSICS}
	       ${ROOT_MATHCORE}
	       ${ROOT_THREAD} )


set(SimulationBase_HEADERS 
     GTruth.h
     MCFlux.h
     MCNeutrino.h
     MCParticle.h
     MCTrajectory.h
     MCTruth.h
     )

add_library(SimulationBase SHARED
     ${SimulationBase_HEADERS}
     GTruth.cxx
     MCFlux.cxx
     MCNeutrino.cxx
     MCParticle.cxx
     MCTrajectory.cxx
     MCTruth.cxx
     )

art_add_dictionary( DICTIONARY_LIBRARIES art::art_Framework_Core SimulationBase ${SIMB_LIBRARIES})

target_link_libraries(SimulationBase
     ${SIMB_LIBRARIES}
     )


install(TARGETS
     SimulationBase
     SimulationBase_map
     SimulationBase_dict
     EXPORT nutoolsLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime 
     )

install(FILES ${SimulationBase_HEADERS} DESTINATION
     ${CMAKE_INSTALL_INCLUDEDIR}/SimulationBase COMPONENT Development)

