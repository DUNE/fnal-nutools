############################################################
# here are the contents of the original SRT GNUmakefile
# all lines start with "#SRT "
#SRT #
#SRT # $Id: GNUmakefile,v 1.4 2012/09/07 21:35:26 brebel Exp $
#SRT #
#SRT # messier@indiana.edu
#SRT #
#SRT include SoftRelTools/arch_spec_root.mk
#SRT include SoftRelTools/arch_spec_genie.mk
#SRT include SoftRelTools/arch_spec_cern.mk
#SRT 
#SRT #VERBOSE     := true
#SRT LIB_TYPE    := shared
 
art_make( LIBRARY_NAME EventGeneratorBaseGENIE
          LIB_LIBRARIES ${ROOT_GEOM}
	                ${ROOT_GEOMPAINTER} )

install_headers()
install_fhicl()
install_source()
