
set(MODULE_LIBRARIES SimulationBase  
               EventGeneratorBaseCRY 
 		     EventGeneratorBaseGENIE
	          ${ART_FRAMEWORK_SERVICES_OPTIONAL_RANDOMNUMBERGENERATOR_SERVICE}
        		${MF_MESSAGELOGGER}
        		${MF_UTILITIES}
        		${FHICLCPP}
        		${CETLIB}
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
                        ${GCROSSSECTIONS}
                        ${GELAS}
                        ${GELFF}
                        ${GHEP}
                        ${GEVGCORE}
                        ${GEVGDRIVERS}
                        ${GGIBUU}
                        ${GHADRONTRANSP}
                        ${GINTERACTION}
                        ${GLLEWELLYNSMITH}
                        ${GMEC}
                        ${GMESSENGER}
                        ${GNUGAMMA}
                        ${GNUE}
                        ${GNTUPLE}
                        ${GNUCLEAR}
                        ${GQPM}
                        ${GPDG}
                        ${GPDF}
                        ${GQEL}
                        ${GREGISTRY}
                        ${GREINSEGHAL}
                        ${GGEO}
                        ${GFLUXDRIVERS}
                        ${GMUELOSS}
                        ${GREWEIGHT}
                        ${GNUCLEONDECAY}
			         ${CRY}
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
                        ${ROOT_GUI}
 	                )


include_directories(
     ${ROOT_INCLUDE_DIRS}
     ${CLHEP_INCLUDE_DIRS}
     ${GENIE_INC}/GENIE 
     ${LOG4CPP_INC} 
     ${LIBXML2_FQ_DIR}/include/libxml2

)

art_add_module(EventGeneratorBase_test_EventGeneratorTest_module EventGeneratorTest_module.cc)

target_link_libraries(EventGeneratorBase_test_EventGeneratorTest_module 
     ${MODULE_LIBRARIES} )

install(TARGETS  
     EventGeneratorBase_test_EventGeneratorTest_module 
     EXPORT nutoolsLibraries 
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )

install(FILES evgentest.fcl DESTINATION job COMPONENT Runtime)

install(FILES README DESTINATION source/EventGeneratorBase/test )
