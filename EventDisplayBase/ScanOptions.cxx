////////////////////////////////////////////////////////////////////////
// $Id: ScanOptions.cxx,v 1.2 2011-01-19 16:44:59 p-nusoftart Exp $
//
// Display parameters for the hand scan view
//
// \author brebel@fnal.gov
////////////////////////////////////////////////////////////////////////
#include "EventDisplayBase/ScanOptions.h"
#include <iostream>

namespace evdb {

  //......................................................................
  ScanOptions::ScanOptions(art::ParameterSet const& pset, art::ActivityRegistry& reg) 
  {
    fIncludeMCInfo     = pset.getParameter< bool                      >("IncludeMCInfo");
    fScanFileBase      = pset.getParameter< std::string               >("FileNameBase");
    fCategories        = pset.getParameter< std::vector<std::string>  >("Categories");
    fFieldLabels       = pset.getParameter< std::vector<std::string>  >("FieldLabels");
    fFieldTypes        = pset.getParameter< std::vector<std::string>  >("FieldTypes");
    fFieldsPerCategory = pset.getParameter< std::vector<unsigned int> >("FieldsPerCategory");
  }
  
  //......................................................................
  ScanOptions::~ScanOptions() 
  {
  }
  
}
////////////////////////////////////////////////////////////////////////
