#
# $Id: GNUmakefile,v 1.2 2011-01-21 16:30:32 p-nusoftart Exp $
#
# messier@indiana.edu
#
include SoftRelTools/arch_spec_root.mk

PACKAGE     := SimulationBase
LIB_TYPE    := shared
LIB         := lib$(PACKAGE)
LIBCXXFILES := $(wildcard *.cxx)
JOBFILES    := $(wildcard *.fcl)

LIBLINK     := $(LOADLIBES) -L$(SRT_PRIVATE_CONTEXT)/lib/$(SRT_SUBDIR) -L$(SRT_PUBLIC_CONTEXT)/lib/$(SRT_SUBDIR) -l$(PACKAGE)

########################################################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_art.mk
