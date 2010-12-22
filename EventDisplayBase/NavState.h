///
/// \file    NavState.h
/// \brief   Holds information about what action to take next.
/// \version $Id: NavState.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  messier@indiana.edu
///
#ifndef EVDB_NAVSTATE_H
#define EVDB_NAVSTATE_H

namespace evdb {
  enum nav_states_ {
    kNEXT_EVENT,
    kPREV_EVENT,
    kRELOAD_EVENT,
    kGOTO_EVENT
  };
  
  ///
  /// Encapsulate some data about where the display should go next
  ///
  /// Since there can only be one state, these are implemented as a
  /// set of static methods
  ///
  class NavState {
  public:
    static int  Which();
    static void Set(int which);
    static void SetTarget(int run, int event);
    static int  TargetRun();
    static int  TargetEvent();
  private:
    NavState() { }
  };
}

#endif
////////////////////////////////////////////////////////////////////////
