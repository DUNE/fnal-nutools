////////////////////////////////////////////////////////////////////////
/// \file  EditMenu.cxx
/// \brief The edit pull down menu
///
/// \version $Id: EditMenu.cxx,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
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

// static cfg::Config* gsConfig[1024];

//......................................................................

EditMenu::EditMenu(TGMenuBar* menubar, TGMainFrame* mf) :
  fMainFrame(mf)
{
  fEditMenu = new TGPopupMenu(gClient->GetRoot());
  fLayout   = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0);
  
  // Create the list of functions. Associate each which a command code
  
  int icfg = 0;
  /*
  cfg::Table::ConfigList& cfglist = cfg::Table::Instance().GetList();
  cfg::Table::ConfigList::iterator itr   (cfglist.begin());
  cfg::Table::ConfigList::iterator itrEnd(cfglist.end());
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
