#
# $Id: GNUmakefile,v 1.3 2011-03-04 19:43:18 brebel Exp $
#
# messier@indiana.edu
#
include SoftRelTools/arch_spec_root.mk
include SoftRelTools/arch_spec_CLHEP.mk
include SoftRelTools/arch_spec_xercesc.mk
include SoftRelTools/arch_spec_geant4.mk

PACKAGE     := G4Base
LIB_TYPE    := shared
LIB         := lib$(PACKAGE)
LIBCXXFILES := $(wildcard *.cxx)

CPPFLAGS     += -I$(G4INSTALL)/include

LIBLINK    := -L$(SRT_PRIVATE_CONTEXT)/lib/$(SRT_SUBDIR) -L$(SRT_PUBLIC_CONTEXT)/lib/$(SRT_SUBDIR) -l$(PACKAGE)


########################################################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_art.mk
########################################################################

override LIBLIBS += $(LOADLIBES) -L$(SRT_PRIVATE_CONTEXT)/lib/$(SRT_SUBDIR) -L$(SRT_PUBLIC_CONTEXT)/lib/$(SRT_SUBDIR) -lGeometry
