#
# $Id: GNUmakefile,v 1.4 2011-01-22 19:30:30 p-nusoftart Exp $
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

override LOADLIBES += $(ROOTLIBS)

LIBLINK     := $(LOADLIBES) -L$(SRT_PRIVATE_CONTEXT)/lib/$(SRT_SUBDIR) -L$(SRT_PUBLIC_CONTEXT)/lib/$(SRT_SUBDIR) -l$(PACKAGE)

# SUBDIRS     := test

########################################################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_art.mk
include SoftRelTools/arch_spec_root.mk

override LIBLIBS += -L$(ROOTSYS)/lib -lGraf -lGX11 -lGX11TTF

