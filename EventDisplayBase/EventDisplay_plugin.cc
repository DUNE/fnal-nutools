////////////////////////////////////////////////////////////////////////
/// \file EventDisplay_plugin.cc
///
/// \version $Id: EventDisplay_plugin.cc,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  jpaley@anl.gov

// Framework includes
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"

/// NOvASoft includes

#include "EventDisplayBase/EventDisplay.h"

namespace evdb {

  DEFINE_FWK_SERVICE(EventDisplay);

} // namespace evdb
////////////////////////////////////////////////////////////////////////



