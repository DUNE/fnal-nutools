////////////////////////////////////////////////////////////////////////
/// \file EventDisplay_plugin.cc
///
/// \version $Id: EventDisplay_plugin.cc,v 1.2 2011-01-19 16:44:59 p-nusoftart Exp $
/// \author  jpaley@anl.gov

// Framework includes
#include "FWCore/Framework/interface/MakerMacros.h"
#include "art/MessageLogger/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"

/// NOvASoft includes

#include "EventDisplayBase/EventDisplay.h"

namespace evdb {

  DEFINE_FWK_SERVICE(EventDisplay);

} // namespace evdb
////////////////////////////////////////////////////////////////////////



