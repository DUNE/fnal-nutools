////////////////////////////////////////////////////////////////////////
/// \file  evgenbase.cxx
/// \brief useful tools for event generation
///
/// 
///
/// \version $Id: evgenbase.cxx,v 1.2 2011-07-18 15:28:44 brebel Exp $
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////

#include "EventGeneratorBase/evgenbase.h"

#include "TRandom3.h"

namespace evgb{

  unsigned int GetRandomNumberSeed()
  {

    // the maximum allowed seed for the art::RandomNumberGenerator
    // is 900000000. Use the system random number generator to get a pseudo-random
    // number for the seed value, and take the modulus of the maximum allowed 
    // seed to ensure we don't ever go over that maximum

    // Set gRandom to be a TRandom3 based on this state in case we need to pull
    // random values from histograms, etc
    TRandom3 *rand = new TRandom3(0);
    gRandom = rand;
    return rand->Integer(900000000);
  }

}
