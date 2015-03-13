
set( LIB_LIBRARIES SimulationBase
	          ${CRY}
	          ${CETLIB}
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


include_directories(
     ${ROOT_INCLUDE_DIRS}
     ${CLHEP_INCLUDE_DIRS}
     ${CRYHOME}/src 
     )

add_library(EventGeneratorBaseCRY SHARED
	CRYHelper.cxx
	CRYHelper.h
	)

target_link_libraries(EventGeneratorBaseCRY
     ${LIB_LIBRARIES}
     FNALCore::FNALCore
     )

target_include_directories(EventGeneratorBaseCRY
     PUBLIC
     ${ROOT_INCLUDE_DIRS}
     ${CLHEP_INCLUDE_DIRS}
     ${CRYHOME}/src
     )


install(FILES CRYHelper.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/EventGeneratorBase/CRY 
     COMPONENT Development )

install(TARGETS EventGeneratorBaseCRY 
     EXPORT nutoolsLibraries 
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )

