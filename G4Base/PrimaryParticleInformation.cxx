////////////////////////////////////////////////////////////////////////
/// \file PrimaryParticleInformation.cxx
//
/// \version $Id: PrimaryParticleInformation.cxx,v 1.2 2011-10-20 17:10:56 brebel Exp $
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
      fMCTruth->Print();
  }

}// namespace
