////////////////////////////////////////////////////////////////////////
/// \file RawDrawingOption_plugin.cc
///
/// \version $Id: ScanOptions_plugin.cc,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  brebel@fnal.gov

// Framework includes
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"

/// LArSoft includes

#include "EventDisplayBase/ScanOptions.h"

namespace evdb {

  DEFINE_FWK_SERVICE(ScanOptions);

} // namespace evd
////////////////////////////////////////////////////////////////////////
