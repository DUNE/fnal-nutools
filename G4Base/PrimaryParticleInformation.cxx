////////////////////////////////////////////////////////////////////////
/// \file PrimaryParticleInformation.cxx
//
/// \version $Id: PrimaryParticleInformation.cxx,v 1.3 2011-11-13 16:12:20 brebel Exp $
/// \author  seligman@nevis.columbia.edu, brebel@fnal.gov
////////////////////////////////////////////////////////////////////////
#include "G4Base/PrimaryParticleInformation.h"
#include "SimulationBase/simbase.h"

namespace g4b{

  //-------------------------------------------------
  G4Allocator<PrimaryParticleInformation> PrimaryParticleInformationAllocator;

  //-------------------------------------------------
  PrimaryParticleInformation::PrimaryParticleInformation()
  {
  }

  //-------------------------------------------------
  PrimaryParticleInformation::~PrimaryParticleInformation()
  {
  }

  //-------------------------------------------------
  void PrimaryParticleInformation::Print() const
  {
    if ( fMCTruth.isNonnull() )
      std::cout << *(fMCTruth.get());
  }

}// namespace
