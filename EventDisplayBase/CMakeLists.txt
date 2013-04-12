
FILE( GLOB src_files *.cxx )

set( EVD_LIBS ${ART_FRAMEWORK_IO_ROOT_ROOTINPUT_SOURCE}
	      ${ROOT_EVE}
	      ${ROOT_EG}
	      ${ROOT_TREEPLAYER} 
	      ${ROOT_GEOM}
	      ${ROOT_GED} 
	      ${ROOT_RGL} 
	      ${ROOT_GUI}
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

art_make_library( LIBRARY_NAME EventDisplayBase
        	  SOURCE ${src_files}
        	  LIBRARIES ${EVD_LIBS} )

simple_plugin( EventDisplay service ${EVD_LIBS} BASENAME_ONLY )
simple_plugin( ScanOptions service ${EVD_LIBS} BASENAME_ONLY )

install_headers()
install_fhicl()
install_source()
