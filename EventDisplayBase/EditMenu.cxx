////////////////////////////////////////////////////////////////////////
/// \file  EditMenu.cxx
/// \brief The edit pull down menu
///
/// \version $Id: EditMenu.cxx,v 1.6 2011-04-05 22:26:55 messier Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#include "EventDisplayBase/EditMenu.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include "TGMsgBox.h"
#include "TGMenu.h"
#include "TGLayout.h"
#include "TThread.h"
#include "EventDisplayBase/evdb.h"
using namespace evdb;

// old code from FMWK - removed cfg namespace to make doxygen happy
// static Config* gsConfig[1024];

//......................................................................

EditMenu::EditMenu(TGMenuBar* menubar, TGMainFrame* mf) :
  fMainFrame(mf)
{
  fEditMenu = new TGPopupMenu(gClient->GetRoot());
  fLayout   = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0);
  
  fEditMenu->Connect("Activated(Int_t)",
		     "evdb::EditMenu",
		     this,
		     "HandleMenu(int)");
  
  // Attach the menu to the menu bar
  menubar->AddPopup("&Edit",fEditMenu,fLayout);
}

//......................................................................

EditMenu::~EditMenu() 
{
  if (fLayout)   { delete fLayout;   fLayout   = 0; }
  if (fEditMenu) { delete fEditMenu; fEditMenu = 0; }
}

//......................................................................

void EditMenu::SetWorkers(const std::vector<std::string>& w)
{
  // Wipe out the existing menus and lists
  for (unsigned int i=0;;++i) {
    TGMenuEntry* m = fEditMenu->GetEntry(i);
    if (m) fEditMenu->DeleteEntry(i);
    else   break;
  }
  
  // Rebuild the list
  for (unsigned int i=0; i<w.size(); ++i) {
    fEditMenu->AddEntry(w[i].c_str(), i);
  }
  fEditMenu->Connect("Activated(Int_t)",
		     "evdb::EditMenu",
		     this,
		     "EditDrawingOptions(int)");
}

//......................................................................

void EditMenu::HandleMenu(int i) 
{
  switch(i){
  default:
    this->EditDrawingOptions(i);
    break;
  }
}

//......................................................................
void EditMenu::EditDrawingOptions(int i)
{
  art::ServiceHandle<evdb::EventDisplay> evd;
  evd->EditDrawingOptionParameterSet(i);
}

//......................................................................

int EditMenu::Preferences() 
{
  this->NoImpl("Preferences");
  return 0;
}

//......................................................................

int EditMenu::NoImpl(const char* method) 
{
  std::string s;
  s = "Sorry action '"; s += method; s+= "' is not implemented.\n";
  // Why isn't this a memory leak? Dunno, but its seems the TG classes
  // are all managed by TGClient which takes care of deletion
  new TGMsgBox(evdb::TopWindow(), fMainFrame,
	       "No implementation",s.c_str(),kMBIconExclamation);
  return 0;
}

////////////////////////////////////////////////////////////////////////
