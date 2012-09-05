#
# $Id: GNUmakefile,v 1.1 2012-09-05 15:59:10 brebel Exp $
#
# messier@indiana.edu
#
include SoftRelTools/arch_spec_root.mk
include SoftRelTools/arch_spec_cern.mk
include SoftRelTools/arch_spec_cry.mk

#VERBOSE     := true
SUBDIRS     := test
LIB_TYPE    := shared
LIB         := lib$(PACKAGE)CRY
LIBCXXFILES := $(wildcard *.cxx)
JOBFILES    := $(wildcard *.fcl)

########################################################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_art.mk

override LIBLIBS += -L$(ROOTSYS)/lib -lGeom -lGeomPainter -L$(SRT_PRIVATE_CONTEXT)/lib/$(SRT_SUBDIR) -L$(SRT_PUBLIC_CONTEXT)/lib/$(SRT_SUBDIR) -lGeometry

override CXXFLAGS := $(filter-out -Woverloaded-virtual, $(CXXFLAGS))

