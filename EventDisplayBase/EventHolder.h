///
/// \file    EventHolder.h
/// \brief   Singleton to hold the current edm::Event for the event display
/// \author  brebel@fnal.gov
/// \version $Id: EventHolder.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
///
#ifndef EVDB_EVENTHOLDER_H
#define EVDB_EVENTHOLDER_H
#ifndef __CINT__

#include "FWCore/Framework/interface/Event.h"

namespace evdb {
  
  class EventHolder {
    
  public:
    static EventHolder* Instance();

    void SetEvent(edm::Event const* evt);
    const edm::Event* GetEvent() const;

  private:

    EventHolder();
    ~EventHolder();

    const edm::Event* fEvent; ///< the Event
  };

}
#endif // __CINT__
#endif // EVDB_EVENTHOLDER_H
