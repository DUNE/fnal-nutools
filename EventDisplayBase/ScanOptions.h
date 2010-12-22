////////////////////////////////////////////////////////////////////////
// $Id: ScanOptions.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
//
// Display parameters for scanning
//
// \author brebel@fnal.gov
////////////////////////////////////////////////////////////////////////
#ifndef EVDB_SCANOPTIONS_H
#define EVDB_SCANOPTIONS_H
#ifndef __CINT__
#include <string>
#include <vector>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

namespace evdb {
  class ScanOptions 
  {
  public:
    ScanOptions(edm::ParameterSet const& pset, edm::ActivityRegistry& reg);
    ~ScanOptions();
    
    bool        fIncludeMCInfo;           ///> true if MC information is to be included in scan output
    std::string fScanFileBase;            ///> base file name for scanning

    // separate fields that are grouped together by the "@" character as an entry in both vectors below

    std::vector<std::string>  fCategories;        ///> names of the various categories for the scan
    std::vector<unsigned int> fFieldsPerCategory; ///> number of fields in each category
    std::vector<std::string>  fFieldTypes;        ///> types of the fields, ie TextEntry, Buttons, NumberEntry, etc
    std::vector<std::string>  fFieldLabels;       ///> types of the fields, ie TextEntry, Buttons, NumberEntry, etc

  };
}//namespace
#endif // __CINT__
#endif

