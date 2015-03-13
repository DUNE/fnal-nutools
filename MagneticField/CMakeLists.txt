IF (ALT_CMAKE)
INCLUDE(altCMakeLists.cmake)
ELSE()


simple_plugin( MagneticField service
                        ${CLHEP}
                        BASENAME_ONLY )

install_headers()
install_fhicl()
install_source()

ENDIF()
