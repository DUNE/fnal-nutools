////////////////////////////////////////////////////////////////////////
/// \file PrimaryParticleInformation.cxx
//
/// \version $Id: PrimaryParticleInformation.cxx,v 1.4 2012-08-06 23:07:19 brebel Exp $
/// \author  seligman@nevis.columbia.edu, brebel@fnal.gov
////////////////////////////////////////////////////////////////////////
#include "G4Base/PrimaryParticleInformation.h"
#include "SimulationBase/simbase.h"

namespace g4b{

  //-------------------------------------------------
  G4Allocator<PrimaryParticleInformation> PrimaryParticleInformationAllocator;

  //-------------------------------------------------
  PrimaryParticleInformation::PrimaryParticleInformation()
    : fMCTruth(0)
  {
  }

  //-------------------------------------------------
  PrimaryParticleInformation::~PrimaryParticleInformation()
  {
  }

  //-------------------------------------------------
  void PrimaryParticleInformation::Print() const
  {
    if ( fMCTruth )
      std::cout << *fMCTruth;
  }

}// namespace
