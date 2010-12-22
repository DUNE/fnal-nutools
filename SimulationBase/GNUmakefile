#
# $Id: GNUmakefile,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
#
# messier@indiana.edu
#
include SoftRelTools/arch_spec_root.mk

PACKAGE     := SimulationBase
LIB_TYPE    := shared
LIB         := lib$(PACKAGE)
LIBCXXFILES := $(wildcard *.cxx)
JOBFILES    := $(wildcard *.py)

LIBLINK     := $(LOADLIBES) -L$(SRT_PRIVATE_CONTEXT)/lib/$(SRT_SUBDIR) -L$(SRT_PUBLIC_CONTEXT)/lib/$(SRT_SUBDIR) -l$(PACKAGE)

########################################################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_art.mk
