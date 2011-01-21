#
# $Id: GNUmakefile,v 1.2 2011-01-21 16:30:32 p-nusoftart Exp $
#
# messier@indiana.edu
#
include SoftRelTools/arch_spec_root.mk
include SoftRelTools/arch_spec_genie.mk
include SoftRelTools/arch_spec_cern.mk
include SoftRelTools/arch_spec_cry.mk

#VERBOSE     := true
PACKAGE     := EventGeneratorBase
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
  skipfiles = GFluxBlender.cxx GFlavorMixerI.cxx GFlavorSwap.cxx
  skiphead  = $(skipfiles:.cxx=.h)
  override LIBCXXFILES := $(filter-out $(skipfiles), $(LIBCXXFILES))
  override CPPFLAGS += -DBLENDER_IN_GENIE
  $(warning GENIE has GFluxBlender.h, ignore local files: $(skipfiles))
endif

########################################################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_art.mk

