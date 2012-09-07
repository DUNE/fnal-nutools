#
# $Id: GNUmakefile,v 1.3 2012-09-07 19:27:45 rhatcher Exp $
#
# messier@indiana.edu
#
include SoftRelTools/arch_spec_root.mk
include SoftRelTools/arch_spec_genie.mk
include SoftRelTools/arch_spec_cern.mk

#VERBOSE     := true
LIB_TYPE    := shared
LIB         := lib$(PACKAGE)GENIE
LIBCXXFILES := $(wildcard *.cxx)
JOBFILES    := $(wildcard *.fcl)

#
# this makefile (and package code) assumes GENIE r3208 or later
#

# take advantage of new GENIE flux driver interfaces
ifneq ($(shell /bin/grep \<set\> \${GENIE}/src/FluxDrivers/GSimpleNtpFlux.h 2> /dev/null ),)
# genie is has the new code (r3669) ... can use the method
#  $(warning GENIE/ROOTGeomAnalyzer haves new LoadBeamSimData set/vector methods)
  override CPPFLAGS += -DGFLUX_TAKES_SETORVECTOR
endif

ifneq ($(shell /bin/ls \${GENIE}/src/FluxDrivers/GFlavorMixerFactory.h 2> /dev/null ),)
# genie has the new code (r3663)... we can use it
  override CPPFLAGS += -DFLAVORMIXERFACTORY
endif

# remain compatible w/ older GENIE version by using inefficient method
ifeq ($(shell /bin/grep GetMaxPathLength \${GENIE}/src/Geo/ROOTGeomAnalyzer.h 2> /dev/null ),)
# genie is missing the new code (r3318) ... can't use the method
#  $(warning GENIE/ROOTGeomAnalyzer does not have GetMaxPathLengths method)
  override CPPFLAGS += -DGENIE_MISSING_GETMAXPL
endif

ifeq ($(shell /bin/grep ppmedium \${GENIE}/src/FluxDrivers/GSimpleNtpFlux.h 2> /dev/null ),)
   override CPPFLAGS += -DGSIMPLE_NUMI_V1
#     vx, vy,vz, ndecay, ppmedium was added (r3068)
#     $(warning Hey we have GSimpleNtpFlux but no ppmedium)
endif


########################################################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_art.mk

override LIBLIBS += -L$(ROOTSYS)/lib -lGeom -lGeomPainter -L$(SRT_PRIVATE_CONTEXT)/lib/$(SRT_SUBDIR) -L$(SRT_PUBLIC_CONTEXT)/lib/$(SRT_SUBDIR) -lGeometry -lEventGeneratorBase

override CXXFLAGS := $(filter-out -Woverloaded-virtual, $(CXXFLAGS))

