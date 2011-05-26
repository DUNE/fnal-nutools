#
# $Id: GNUmakefile,v 1.9 2011-05-26 13:30:34 brebel Exp $
#
# messier@indiana.edu
#

ROOTCINT    := YES
PACKAGE     := EventDisplayBase
LIB_TYPE    := shared
LIB         := lib$(PACKAGE)
LIBCXXFILES := $(wildcard *.cxx)
JOBFILES    := $(wildcard *.fcl)

ROOTLIBS  = $(shell root-config --libs) -lEG -lTreePlayer -lGeom -lFFTW -lReflex -lEGPythia6 -lGui

override CPPFLAGS += $(shell root-config --cflags) 
override LDFLAGS += $(shell root-config --libs) 

override LOADLIBES += -L$(ART_LIB) -lart_Framework_IO_Root_RootInput_source $(ROOTLIBS) $(ROOTGLIBS)

LIBLINK     := -L$(SRT_PRIVATE_CONTEXT)/lib/$(SRT_SUBDIR) -L$(SRT_PUBLIC_CONTEXT)/lib/$(SRT_SUBDIR) -l$(PACKAGE)

# SUBDIRS     := test

########################################################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_art.mk
include SoftRelTools/arch_spec_root.mk

override LIBLIBS += $(LOADLIBES) 
