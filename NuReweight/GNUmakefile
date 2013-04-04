#
# $Id: GNUmakefile,v 1.2 2012-09-19 18:12:09 brebel Exp $
#
# messier@indiana.edu
#
#include SoftRelTools/arch_spec_root.mk
#include SoftRelTools/arch_spec_genie.mk
#include SoftRelTools/arch_spec_cern.mk
#include SoftRelTools/arch_spec_cry.mk
#include SoftRelTools/standard.mk
#include SoftRelTools/arch_spec_art.mk
#include SoftRelTools/arch_spec_root.mk
#include SoftRelTools/arch_spec_genie.mk

PACKAGE     := NuReweight
#SUBDIRS     := art
LIB_TYPE    := shared
LIB         := lib$(PACKAGE)
LIBCXXFILES := $(wildcard *.cxx)
JOBFILES    := $(wildcard *.fcl)
SUBDIRS     := art

ifneq ($(shell /bin/ls \${GENIE}/src/FluxDrivers/GFlavorMixerFactory.h 2> /dev/null ),)
  # genie has the new code (r3663)... we can use it
  override CPPFLAGS += -DFLAVORMIXERFACTORY
  #$(warning Hey we have GFlavorMixerFactory)
else
  # class isn't available ... don't try to build code that relies on it
  skipfiles = GFlavorOsc.cxx
  skiphead  = $(skipfiles:.cxx=.h)
  override LIBCXXFILES := $(filter-out $(skipfiles), $(LIBCXXFILES))
  #$(warning Drat we are missing GFlavorMixerFactory no oscillations for you)
endif

LIBLINK    := -L$(SRT_PRIVATE_CONTEXT)/lib/$(SRT_SUBDIR) -L$(SRT_PUBLIC_CONTEXT)/lib/$(SRT_SUBDIR) -l$(PACKAGE)


########################################################################
include SoftRelTools/standard.mk
#include SoftRelTools/arch_spec_art.mk
include SoftRelTools/arch_spec_root.mk
include SoftRelTools/arch_spec_genie.mk
#-lEventGeneratorBase
override LIBLIBS += $(LOADLIBES)  -L$(SRT_PRIVATE_CONTEXT)/lib/$(SRT_SUBDIR) -L$(SRT_PUBLIC_CONTEXT)/lib/$(SRT_SUBDIR) 

override CXXFLAGS := $(filter-out -Woverloaded-virtual, $(CXXFLAGS))