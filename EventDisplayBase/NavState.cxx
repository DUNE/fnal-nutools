///
/// \file    NavState.cxx
/// \brief   Holds information about what action to take next.
/// \version $Id: NavState.cxx,v 1.2 2011-08-31 20:19:58 brebel Exp $
/// \author  messier@indiana.edu
///
#include "EventDisplayBase/NavState.h"
#include "TROOT.h"
#include "TApplication.h"
using namespace evdb;

static int gsNavState    = 0;
static int gsTargetRun   = 0;
static int gsTargetEvent = 0;

//......................................................................

int NavState::Which() { return gsNavState; }

//......................................................................

void NavState::Set(int which)
{
  // only allow the state to change if it has not been previously set
  // to kSEQUENTIAL_ONLY, this should be made more intelligent, ie
  // if we can only do sequential access because we are using the 
  // socket input source, then we shouldn't be allowed to press the
  // other buttons
  if(gsNavState == kSEQUENTIAL_ONLY) return;

  gsNavState = which;
  gROOT->GetApplication()->Terminate();
}

//......................................................................

void NavState::SetTarget(int run, int event)
{
  gsTargetRun = run;
  gsTargetEvent = event;
}

//......................................................................

int NavState::TargetRun() { return gsTargetRun; }

//......................................................................

int NavState::TargetEvent() { return gsTargetEvent; }

////////////////////////////////////////////////////////////////////////
