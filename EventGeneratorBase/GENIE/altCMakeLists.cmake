
set( LIB_LIBRARIES SimulationBase
	         ${ART_UTILITIES}
              ${MF_MESSAGELOGGER}
              ${MF_UTILITIES}
              ${FHICLCPP}
              ${CETLIB}
			${XML2}
			${LOG4CPP}
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
	          ${ROOT_GEOM}
	          ${ROOT_GEOMPAINTER}
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

include_directories (
     ${ROOT_INCLUDE_DIRS}
     ${CLHEP_INCLUDE_DIRS}
     ${GENIE_INC}/GENIE 
     ${LOG4CPP_INC} 
     ${LIBXML2_FQ_DIR}/include/libxml2
)



add_library(EventGeneratorBaseGENIE SHARED
	GENIEHelper.cxx
	GENIEHelper.h
	)

target_link_libraries(EventGeneratorBaseGENIE
     FNALCore::FNALCore
     ${LIB_LIBRARIES}
     )

target_include_directories(EventGeneratorBaseGENIE
     PUBLIC
     ${ROOT_INCLUDE_DIRS}
     ${CLHEP_INCLUDE_DIRS}
     )

install(FILES GENIEHelper.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/EventGeneratorBase/GENIE 
     COMPONENT Development )

install(TARGETS EventGeneratorBaseGENIE 
     EXPORT nutoolsLibraries 
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )

