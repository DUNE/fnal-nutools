///
/// \file    EventHolder.h
/// \brief   Singleton to hold the current art::Event for the event display
/// \author  brebel@fnal.gov
/// \version $Id: EventHolder.h,v 1.2 2011-01-19 16:44:59 p-nusoftart Exp $
///
#ifndef EVDB_EVENTHOLDER_H
#define EVDB_EVENTHOLDER_H
#ifndef __CINT__

#include "art/Framework/Core/Event.h"

namespace evdb {
  
  class EventHolder {
    
  public:
    static EventHolder* Instance();

    void SetEvent(art::Event const* evt);
    const art::Event* GetEvent() const;

  private:

    EventHolder();
    ~EventHolder();

    const art::Event* fEvent; ///< the Event
  };

}
#endif // __CINT__
#endif // EVDB_EVENTHOLDER_H
