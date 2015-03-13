
set(
	  LIB_LIBRARIES ${ROOT_CORE}
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

add_library(NuBeamWeights SHARED
     Conventions.h
     skzpReweight.h
     skzpReweight.cxx
     )

install(TARGETS
     NuBeamWeights
     EXPORT nutoolsLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime 
     )

target_link_libraries(NuBeamWeights ${LIB_LIBRARIES})

install(FILES Conventions.h skzpReweight.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NuBeamWeights COMPONENT Development)


