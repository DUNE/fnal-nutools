////////////////////////////////////////////////////////////////////////
/// \file  AutoAdvance.h
/// \brief Class to allow automatic flashing of events
///
/// \version $Id: AutoAdvance.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVDB_AUTOADVANCE_H
#define EVDB_AUTOADVANCE_H
#include "TObject.h"
class TTimer;

namespace evdb {
  /// Automatically advance display to next event based on a timer
  class AutoAdvance : public TObject {
  public:
    AutoAdvance();
    ~AutoAdvance();
    Bool_t HandleTimer(TTimer *t);

  public:
    TTimer* fTimer;
  };
}

#endif
////////////////////////////////////////////////////////////////////////
