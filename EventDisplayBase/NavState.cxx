///
/// \file    NavState.cxx
/// \brief   Holds information about what action to take next.
/// \version $Id: NavState.cxx,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
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
