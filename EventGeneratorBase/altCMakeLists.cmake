
add_subdirectory(CRY)
add_subdirectory(GENIE)
add_subdirectory(test)

install(FILES evgenbase.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/EventGeneratorBase 
     COMPONENT Development )

