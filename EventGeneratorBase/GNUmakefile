#
# $Id: GNUmakefile,v 1.8 2011-11-28 20:33:38 rhatcher Exp $
#
# messier@indiana.edu
#
include SoftRelTools/arch_spec_root.mk
include SoftRelTools/arch_spec_genie.mk
include SoftRelTools/arch_spec_cern.mk
include SoftRelTools/arch_spec_cry.mk

#VERBOSE     := true
PACKAGE     := EventGeneratorBase
SUBDIRS     := test
LIB_TYPE    := shared
LIB         := lib$(PACKAGE)
LIBCXXFILES := $(wildcard *.cxx)
JOBFILES    := $(wildcard *.fcl)

# hack until users all have GENIE R-2_6_0
ifeq ($(shell /bin/ls \${GENIE}/src/FluxDrivers/GSimpleNtpFlux.h 2> /dev/null ),)
  $(warning HEY we are MISSING GSimpleNtpFlux, needs GENIE R-2_6_0 or later)
  override CPPFLAGS += -DMISSING_GSIMPLENTPFLUX
else
#  $(warning Hey we have GSimpleNtpFlux)
  ifeq ($(shell /bin/grep ppmedium \${GENIE}/src/FluxDrivers/GSimpleNtpFlux.h 2> /dev/null ),)
     override CPPFLAGS += -DGSIMPLE_NUMI_V1
#     $(warning Hey we have GSimpleNtpFlux but no ppmedium)
  else
#     $(warning Hey we have GSimpleNtpFlux including ppmedium)
  endif
endif

ifneq ($(shell /bin/ls \${GENIE}/src/FluxDrivers/GFluxBlender.h 2> /dev/null ),)
# genie has new code ... don't use local version of these files
  skipfiles = GFluxBlender.cxx GFlavorMixerI.cxx GFlavorMap.cxx
  skiphead  = $(skipfiles:.cxx=.h)
  override LIBCXXFILES := $(filter-out $(skipfiles), $(LIBCXXFILES))
  override CPPFLAGS += -DBLENDER_IN_GENIE
#  $(warning GENIE has GFluxBlender.h, ignore local files: $(skipfiles))
endif

# remain compatible w/ older GENIE version by using inefficient method
ifeq ($(shell /bin/grep GetMaxPathLength \${GENIE}/src/Geo/ROOTGeomAnalyzer.h 2> /dev/null ),)
#  $(warning GENIE/ROOTGeomAnalyzer does not have GetMaxPathLengths method)
  override CPPFLAGS += -DGENIE_MISSING_GETMAXPL
endif

########################################################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_art.mk

override LIBLIBS += -L$(ROOTSYS)/lib -lGeom -lGeomPainter -L$(SRT_PRIVATE_CONTEXT)/lib/$(SRT_SUBDIR) -L$(SRT_PUBLIC_CONTEXT)/lib/$(SRT_SUBDIR) -lGeometry

override CXXFLAGS := $(filter-out -Woverloaded-virtual, $(CXXFLAGS))

