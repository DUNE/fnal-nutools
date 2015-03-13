
set( NURW_LIBS ${ART_FRAMEWORK_SERVICES_REGISTRY}
               ${ART_FRAMEWORK_SERVICES_OPTIONAL}
	       ${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
               ${ART_FRAMEWORK_CORE}
	       ${ART_PERSISTENCY_COMMON}
               ${ART_FRAMEWORK_IO_SOURCES}
               ${ART_FRAMEWORK_IO_ROOT}
               ${ART_FRAMEWORK_IO_ROOT_ROOTINPUT_SOURCE}
               ${ART_FRAMEWORK_PRINCIPAL}
               ${ART_PERSISTENCY_PROVENANCE}
               ${ART_UTILITIES}
               ${MF_MESSAGELOGGER}
               ${MF_UTILITIES}
               ${FHICLCPP}
               ${CETLIB}
	       ${LOG4CPP}
	       ${XML2}
	       ${PYTHIA6}
	       ${LHAPDF}
	       ${GALGORITHM}
	       ${GBARYONRESONANCE}
	       ${GBASE}
	       ${GBODEKYANG}
	       ${GCHARM}
	       ${GCOH}
	       ${GDFRC}
	       ${GCROSSSECTIONS}
	       ${GELAS}
	       ${GELFF}
	       ${GHEP}
	       ${GEVGCORE}
	       ${GEVGDRIVERS}
	       ${GGIBUU}
	       ${GHADRONTRANSP}
	       ${GINTERACTION}
	       ${GLLEWELLYNSMITH}
	       ${GMEC}
	       ${GMESSENGER}
	       ${GNUGAMMA}
	       ${GNUE}
	       ${GNTUPLE}
	       ${GNUCLEAR}
	       ${GQPM}
	       ${GPDG}
	       ${GPDF}
	       ${GQEL}
	       ${GREGISTRY}
	       ${GREINSEGHAL}
	       ${GGEO}
	       ${GFLUXDRIVERS}
	       ${GMUELOSS}
	       ${GREWEIGHT}
	       ${GNUCLEONDECAY}
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
               ${ROOT_THREAD}
               ${ROOT_EG}
               ${ROOT_TREEPLAYER} 
               ${ROOT_FFTW}
               ${ROOT_REFLEX}
               ${ROOT_GUI}
           )

add_library(NuReweightArt SHARED
	NuReweight.h
	NuReweight.cxx
	)

target_link_libraries(NuReweightArt 
     SimulationBase NuReweight ${NURW_LIBS} )


art_add_module(ReweightAna_module ReweightAna_module.cc)

target_link_libraries(ReweightAna_module 
     NuReweightArt SimulationBase NuReweight ${NURW_LIBS} )



install(TARGETS
     NuReweightArt
     ReweightAna_module
     EXPORT nutoolsLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime 
     )

install(FILES NuReweight.h DESTINATION
     ${CMAKE_INSTALL_INCLUDEDIR}/NuReweightArt COMPONENT Development)


