////////////////////////////////////////////////////////////////////////
/// \file  EditMenu.cxx
/// \brief The edit pull down menu
///
/// \version $Id: EditMenu.cxx,v 1.3 2011-01-29 20:15:25 p-nusoftart Exp $
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
  
  // Create the list of functions. Associate each which a command code
  
  int icfg = 0;
  /* Table lives in the old FMKW cfg namespace - removing that to make doxygen output better
  Table::ConfigList& cfglist = Table::Instance().GetList();
  Table::ConfigList::iterator itr   (cfglist.begin());
  Table::ConfigList::iterator itrEnd(cfglist.end());
  for (; itr!=itrEnd; ++itr) {
    fEditMenu->AddEntry((*itr)->GetName(), icfg);
    gsConfig[icfg] = (*itr);
    ++icfg;
  }
  */
  
  // fEditMenu->Connect("Activated(Int_t)",
  // "evdb::EditMenu",this,"HandleMenu(int)");
  
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

void EditMenu::HandleMenu(int i) 
{
  // new CfgEdit(fMainFrame); 
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
