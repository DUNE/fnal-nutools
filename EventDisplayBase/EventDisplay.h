////////////////////////////////////////////////////////////////////////
/// \file  EventDisplay.h
/// \brief The interactive event display
///
/// \version $Id: EventDisplay.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVDB_EVENTDISPLAY_H
#define EVDB_EVENTDISPLAY_H
#ifndef __CINT__

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Services/interface/UserInteraction.h"

namespace edm{ class Worker; }

namespace evdb {

  /// ART event display service
  class EventDisplay  {
  public:

    EventDisplay(edm::ParameterSet const& pset, edm::ActivityRegistry& reg);
    ~EventDisplay();
    
    void EditWorkerParameterSet(int i);
    
  private:
    void postBeginJobWorkers(edm::InputSource* inputs,
			     std::vector<edm::Worker*> const& workers);
    void preProcessEvent(edm::EventID const&, edm::Timestamp const&);
    void postProcessEvent(edm::Event const&,  edm::EventSetup const& );

  private:
    edm::InputSource* fInputSource; ///< Input source of events
    
  public:
    unsigned int              fAutoAdvanceInterval; ///< Wait time in milliseconds
    std::vector<edm::Worker*> fWorkers;             ///< needed for reconfiguration
    std::vector<std::string>  fParamSets;           ///< Reconfigure parameters
  };
}
#endif // __CINT__
#endif // EVDB_EVENTDISPLAY_H

////////////////////////////////////////////////////////////////////////
