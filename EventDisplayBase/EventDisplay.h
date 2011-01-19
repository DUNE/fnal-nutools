////////////////////////////////////////////////////////////////////////
/// \file  EventDisplay.h
/// \brief The interactive event display
///
/// \version $Id: EventDisplay.h,v 1.2 2011-01-19 16:44:59 p-nusoftart Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVDB_EVENTDISPLAY_H
#define EVDB_EVENTDISPLAY_H
#ifndef __CINT__

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/Service.h"
#include "art/ParameterSet/FileInPath.h"
#include "FWCore/Services/interface/UserInteraction.h"

namespace art{ class Worker; }

namespace evdb {

  /// ART event display service
  class EventDisplay  {
  public:

    EventDisplay(art::ParameterSet const& pset, art::ActivityRegistry& reg);
    ~EventDisplay();
    
    void EditWorkerParameterSet(int i);
    
  private:
    void postBeginJobWorkers(art::InputSource* inputs,
			     std::vector<art::Worker*> const& workers);
    void preProcessEvent(art::EventID const&, art::Timestamp const&);
    void postProcessEvent(art::Event const&,  art::EventSetup const& );

  private:
    art::InputSource* fInputSource; ///< Input source of events
    
  public:
    unsigned int              fAutoAdvanceInterval; ///< Wait time in milliseconds
    std::vector<art::Worker*> fWorkers;             ///< needed for reconfiguration
    std::vector<std::string>  fParamSets;           ///< Reconfigure parameters
  };
}
#endif // __CINT__
#endif // EVDB_EVENTDISPLAY_H

////////////////////////////////////////////////////////////////////////
