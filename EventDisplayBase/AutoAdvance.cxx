////////////////////////////////////////////////////////////////////////
/// \file  AutoAdvance.cxx
/// \brief Advance to next event based on a timer
///
/// \version $Id: AutoAdvance.cxx,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#include "EventDisplayBase/AutoAdvance.h"
#include <iostream>
#include <unistd.h>
#include "TTimer.h"
// #include "IoModules/ReadWriteModule.h"
// #include "EventDisplayBase/IoModule.h"
using namespace evdb;

AutoAdvance::AutoAdvance() : fTimer(0) { 
  fTimer = new TTimer;
  fTimer->SetObject(this);
  fTimer->Start(1000);
}

//......................................................................

AutoAdvance::~AutoAdvance() 
{
  fTimer->Stop();
  delete fTimer;
}

//......................................................................

Bool_t AutoAdvance::HandleTimer(TTimer *t) 
{
  t->Stop();
  
  // Stuff related to advancing in the file...
  
  t->Start(1000);
  return true;
}

////////////////////////////////////////////////////////////////////////
