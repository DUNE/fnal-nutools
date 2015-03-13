include(CetRootCint)
include(CetParseArgs)

include_directories ( ${CMAKE_CURRENT_SOURCE_DIR} )

set( PACKAGE EventDisplayBase )
FILE( GLOB src_files *.cxx )
FILE( GLOB inc_files *.h )


set( EVD_LIBS ${ART_FRAMEWORK_SERVICES_REGISTRY}
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
	      ${ROOT_EVE}
 	      ${ROOT_EG}
	      ${ROOT_GX11}
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

cet_rootcint( ${PACKAGE} )
add_library(${PACKAGE} SHARED ${src_files} ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE}Cint.cc)

set( EVD_SERVICE_LIBS ${PACKAGE} ${EVD_LIBS} )

art_add_service(EventDisplay_service EventDisplay_service.cc )
art_add_service(ScanOptions_service ScanOptions_service.cc )
target_link_libraries(EventDisplay_service ${EVD_SERVICE_LIBS})
target_link_libraries(ScanOptions_service ${EVD_SERVICE_LIBS})

install(TARGETS
     ${PACKAGE}
     EventDisplay_service
     ScanOptions_service
     EXPORT nutoolsLibraries 
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )

install(FILES ${inc_files} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PACKAGE} COMPONENT development)
