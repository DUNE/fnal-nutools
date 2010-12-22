////////////////////////////////////////////////////////////////////////
/// \file PrimaryParticleInformation.cxx
//
/// \version $Id: PrimaryParticleInformation.cxx,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  seligman@nevis.columbia.edu, brebel@fnal.gov
////////////////////////////////////////////////////////////////////////
#include "G4Base/PrimaryParticleInformation.h"
#include "SimulationBase/simbase.h"

using namespace g4b;

///-------------------------------------------------
G4Allocator<PrimaryParticleInformation> PrimaryParticleInformationAllocator;

///-------------------------------------------------
PrimaryParticleInformation::PrimaryParticleInformation()
{
}

///-------------------------------------------------
PrimaryParticleInformation::~PrimaryParticleInformation()
{
}

///-------------------------------------------------
void PrimaryParticleInformation::Print() const
{
  if ( m_MCTruth.isNonnull() )
    m_MCTruth->Print();
}

