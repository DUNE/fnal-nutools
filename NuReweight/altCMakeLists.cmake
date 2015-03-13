
##cet_add_compiler_flags(CXX -DSETDIFFXSEC_1ARG )

set(
          LIB_LIBRARIES 
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
			)
 
include_directories (
     ${ROOT_INCLUDE_DIRS}
     ${CLHEP_INCLUDE_DIRS}
     ${GENIE_INC}/GENIE 
     ${LOG4CPP_INC} 
     ${LIBXML2_FQ_DIR}/include/libxml2
)


set(NuReweight_HEADERS
	GENIEReweight.h
	ReweightLabels.h
	)

add_library(NuReweight SHARED
	${NuReweight_HEADERS}
	GENIEReweight.cxx
	)

target_link_libraries( NuReweight ${LIB_LIBRARIES} )

install(TARGETS
     NuReweight
     EXPORT nutoolsLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime 
     )

install(FILES ${NuReweight_HEADERS} DESTINATION
     ${CMAKE_INSTALL_INCLUDEDIR}/NuReweight COMPONENT Development)



add_subdirectory(art)
