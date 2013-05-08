
cet_add_compiler_flags(CXX -DSETDIFFXSEC_1ARG )

art_make( LIBRARY_NAME NuReweightArt
          LIB_LIBRARIES SimulationBase
	                NuReweight 
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
                        ${ROOTSYS}/lib/libEGPythia6.so
                        ${ROOT_GUI}
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
 	  BASENAME_ONLY )

install_headers()
install_fhicl()
install_source()
