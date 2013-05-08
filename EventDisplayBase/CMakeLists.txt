include_directories ( ${CMAKE_CURRENT_SOURCE_DIR} )

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

# header list for rootcint
set( EVD_HEADERS ButtonBar.h
		 Canvas.h
		 ColorScale.h
		 DisplayWindow.h
		 EditMenu.h
		 evdb.h
		 EventHolder.h
		 FileMenu.h
		 HelpMenu.h
		 JobMenu.h
		 ListWindow.h
		 MenuBar.h
		 NavState.h
		 ObjListCanvas.h
		 ParameterSetEdit.h
		 Printable.h
		 PrintDialog.h
		 RootEnv.h
		 ScanWindow.h
		 View2D.h
		 View3D.h
		 WindowMenu.h
		 LinkDef.h )
# for rootcint
set( EVD_CXX_MACROS -D_POSIX_SOURCE
		    -D_SVID_SOURCE
		    -D_BSD_SOURCE
		    -D_POSIX_C_SOURCE=2
		-D_XOPEN_SOURCE                    
		-DUNIX  
		-DLINUX  
		-D__UNIX__  
		-D__LINUX__  
		-DDATAREP_LITTLE_IEEE  
		-DDATAREP_LITTLE_ENDIAN  
		    -DDEFECT_NO_IOSTREAM_NAMESPACES
		    -DDEFECT_NO_JZEXT
		    -DDEFECT_NO_INTHEX
		    -DDEFECT_NO_INTHOLLERITH
		    -DDEFECT_NO_READONLY
		    -DDEFECT_NO_DIRECT_FIXED
		    -DDEFECT_NO_STRUCTURE )

find_program( ROOTCINT NAMES rootcint PATHS ${ROOTSYS}/bin )

message(STATUS "EventDisplayBase: running ${ROOTCINT} and using headers in ${ROOT_INC}")

add_custom_command(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/EventDisplayBaseCint.cc
          ${CMAKE_CURRENT_BINARY_DIR}/EventDisplayBaseCint.h
   COMMAND ${ROOTCINT} -f ${CMAKE_CURRENT_BINARY_DIR}/EventDisplayBaseCint.cc
                       -c -p ${EVD_CXX_MACROS}
		       -I.
		       -I${CMAKE_SOURCE_DIR}
		       -I${CMAKE_CURRENT_SOURCE_DIR}
		       -DUSE_ROOT -I${ROOT_INC}
		       ${EVD_HEADERS} || { rm -f ${CMAKE_CURRENT_BINARY_DIR}/EventDisplayBaseCint.cc\; /bin/false\; }
   DEPENDS ${EVD_HEADERS}
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# set variable for install_source
set(cet_generated_code ${CMAKE_CURRENT_BINARY_DIR}/EventDisplayBaseCint.cc
                       ${CMAKE_CURRENT_BINARY_DIR}/EventDisplayBaseCint.h )


art_make_library( LIBRARY_NAME EventDisplayBase
        	  SOURCE ${src_files} ${CMAKE_CURRENT_BINARY_DIR}/EventDisplayBaseCint.cc
        	  LIBRARIES ${EVD_LIBS} )

simple_plugin( EventDisplay service ${EVD_LIBS} BASENAME_ONLY )
simple_plugin( ScanOptions service ${EVD_LIBS} BASENAME_ONLY )

install_headers()
install_fhicl()
install_source()
