////////////////////////////////////////////////////////////////////////
/// \file  EventDisplay.h
/// \brief The interactive event display
///
/// \version $Id: EventDisplay.h,v 1.6 2011-04-11 21:36:05 greenc Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVDB_EVENTDISPLAY_H
#define EVDB_EVENTDISPLAY_H
#ifndef __CINT__

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

namespace art{ 
  class RootInput;
  class Worker; 
}

namespace evdb {

  /// ART event display service
  class EventDisplay  {
  public:

    EventDisplay(fhicl::ParameterSet const& pset, art::ActivityRegistry& reg);
    ~EventDisplay();
    
    void EditWorkerParameterSet(int i);
    void EditDrawingOptionParameterSet(int i);
    
  private:

    void postBeginJob();
    void postBeginJobWorkers(art::InputSource* inputs,
			     std::vector<art::Worker*> const& workers);
    void preProcessEvent(art::EventID const&, art::Timestamp const&);
    void postProcessEvent(art::Event const&);

    void ReconfigureWorkers();
    void ReconfigureDrawingOptions();

  private:
    art::RootInput* fInputSource; ///< Input source of events
    
  public:
    unsigned int              fAutoAdvanceInterval; ///< Wait time in milliseconds
    std::vector<art::Worker*> fWorkers;             ///< needed for reconfiguration
    std::vector<std::string>  fParamSets;           ///< Reconfigure parameters
    std::vector<std::string>  fDrawingOptions;      ///< The drawing option services in use
    std::vector<std::string>  fDrawingParamSets;    ///< Reconfigure drawing parameters
  };
}
#endif // __CINT__
#endif // EVDB_EVENTDISPLAY_H

////////////////////////////////////////////////////////////////////////
