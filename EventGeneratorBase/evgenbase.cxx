////////////////////////////////////////////////////////////////////////
/// \file  evgenbase.cxx
/// \brief useful tools for event generation
///
/// 
///
/// \version $Id: evgenbase.cxx,v 1.1 2011-06-29 17:13:40 brebel Exp $
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
    TRandom3 rand(0);
    return rand.Integer(900000000);
  }

}
