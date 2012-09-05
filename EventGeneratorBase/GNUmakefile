#
# $Id: GNUmakefile,v 1.10 2012-09-05 20:24:21 brebel Exp $
#
# messier@indiana.edu
#
include SoftRelTools/arch_spec_root.mk
include SoftRelTools/arch_spec_cern.mk

#VERBOSE     := true
PACKAGE     := EventGeneratorBase
SUBDIRS     := test GENIE CRY
LIB_TYPE    := shared
LIB         := lib$(PACKAGE)
LIBCXXFILES := $(wildcard *.cxx)
JOBFILES    := $(wildcard *.fcl)

########################################################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_art.mk

override LIBLIBS += -L$(ROOTSYS)/lib -lGeom -lGeomPainter -L$(SRT_PRIVATE_CONTEXT)/lib/$(SRT_SUBDIR) -L$(SRT_PUBLIC_CONTEXT)/lib/$(SRT_SUBDIR) -lGeometry

override CXXFLAGS := $(filter-out -Woverloaded-virtual, $(CXXFLAGS))

