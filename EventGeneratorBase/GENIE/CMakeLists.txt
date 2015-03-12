if (CMAKE_SYSTEM_NAME MATCHES Darwin)
  # ugh, no "ifdh" library at this time
  SET(IFDH "")
  SET(CMAKE_CXX_FLAGS "-DNO_IFDH_LIB=1 ${CMAKE_CXX_FLAGS}")
else (CMAKE_SYSTEM_NAME MATCHES Darwin)
  # we can rely on the existence of the "ifdh" library
  include_directories( $ENV{IFDHC_FQ_DIR}/inc )
  SET(ENV{IFHDC_LIB}   "$ENV{IFDHC_FQ_DIR}/lib")
  cet_find_library( IFDH NAMES ifdh PATHS ENV IFDHC_LIB NO_DEFAULT_PATH )
endif (CMAKE_SYSTEM_NAME MATCHES Darwin)

art_make( LIBRARY_NAME EventGeneratorBaseGENIE
          LIB_LIBRARIES SimulationBase
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
			${GDIS}
			${GCROSSSECTIONS}
			${GDECAY}
			${GELAS}
			${GELFF}
			${GHEP}
			${GEVGCORE}
			${GEVGMODULES}
			${GEVGDRIVERS}
			${GGIBUU}
			${GHADRONTRANSP}
			${GFRAGMENTATION}
			${GINTERACTION}
			${GLLEWELLYNSMITH}
			${GMEC}
			${GMESSENGER}
			${GNUGAMMA}
			${GNUE}
			${GNTUPLE}
			${GNUCLEAR}
			${GNUMERICAL}
			${GQPM}
			${GPDG}
			${GPDF}
			${GQEL}
			${GRES}
			${GREGISTRY}
			${GREINSEGHAL}
			${GUTILS}
			${GGEO}
			${GFLUXDRIVERS}
			${GMUELOSS}
			${GREWEIGHT}
			${GNUCLEONDECAY}
                        ${IFDH}
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

install_headers()
install_fhicl()
install_source()
