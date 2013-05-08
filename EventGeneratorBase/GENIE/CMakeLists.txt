
cet_add_compiler_flags(CXX -DGENIE_USE_ENVVAR -DGFLUX_MISSING_SETORVECTOR )

art_make( LIBRARY_NAME EventGeneratorBaseGENIE
          LIB_LIBRARIES ${ROOT_GEOM}
	                ${ROOT_GEOMPAINTER} )

install_headers()
install_fhicl()
install_source()
