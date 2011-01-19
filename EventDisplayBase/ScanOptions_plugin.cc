////////////////////////////////////////////////////////////////////////
/// \file RawDrawingOption_plugin.cc
///
/// \version $Id: ScanOptions_plugin.cc,v 1.2 2011-01-19 16:44:59 p-nusoftart Exp $
/// \author  brebel@fnal.gov

// Framework includes
#include "FWCore/Framework/interface/MakerMacros.h"
#include "art/MessageLogger/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"

/// LArSoft includes

#include "EventDisplayBase/ScanOptions.h"

namespace evdb {

  DEFINE_FWK_SERVICE(ScanOptions);

} // namespace evd
////////////////////////////////////////////////////////////////////////
