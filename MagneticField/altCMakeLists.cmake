
include_directories(${Geant4_INCLUDE_DIR}/..)

art_add_service(MagneticField_service MagneticField_service.cc)

install(TARGETS MagneticField_service
     EXPORT nutoolsLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime 
     )

target_link_libraries(MagneticField_service 
     ${CLHEP}
     )

install(FILES MagneticField.h 
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MagneticField COMPONENT Development)
   

install(FILES magfield.fcl DESTINATION job COMPONENT Runtime)

