////////////////////////////////////////////////////////////////////////
/// \file  WindowMenu.cxx
/// \brief Implement the pull down window menu
///
/// \version $Id: WindowMenu.cxx,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#include "EventDisplayBase/WindowMenu.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include "TGMsgBox.h"
#include "TGMenu.h"
#include "TGLayout.h"
#include "EventDisplayBase/evdb.h"
#include "EventDisplayBase/DisplayWindow.h"
// #include "EventDisplayBase/ScanCat.h"
using namespace evdb;

//......................................................................

WindowMenu::WindowMenu(TGMenuBar* menubar, TGMainFrame* /*mf*/)
{
  fWindowMenu = new TGPopupMenu(gClient->GetRoot());
  fLayout     = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0);

  // Create the list of functions. Associate each which a command code
  unsigned int i=0;
  const std::vector<std::string>& names = DisplayWindow::Names();
  for (; i<names.size(); ++i) {
    fWindowMenu->AddEntry(names[i].c_str(), i);
  }
  fWindowMenu->AddSeparator();

  // Create the list of functions. Associate each which a command code
  const std::vector<std::string>& lnames = ListWindow::Names();
  for (unsigned int j=0; j<lnames.size(); ++j) {
    fWindowMenu->AddEntry(lnames[j].c_str(), i+j);
  }
  fWindowMenu->AddSeparator();
  /*
  fWindowMenu->AddEntry("&Scan Categories",  -1);
  fWindowMenu->AddSeparator();
  fWindowMenu->AddEntry("&MC Information",   -2);
  */
  fWindowMenu->AddEntry("&ROOT Browser",    -3);
  fWindowMenu->Connect("Activated(Int_t)",
		       "evdb::WindowMenu",this,"HandleMenu(int)");
  
  // Attach the menu to the menu bar
  menubar->AddPopup("&Window",fWindowMenu,fLayout);
}

//......................................................................

WindowMenu::~WindowMenu() 
{
  if (fLayout)     { delete fLayout;     fLayout     = 0; }
  if (fWindowMenu) { delete fWindowMenu; fWindowMenu = 0; }
}

//......................................................................

void WindowMenu::HandleMenu(int menu) 
{
}

//......................................................................

int WindowMenu::NoImpl(const char* method) 
{
  std::string s;
  s = "Sorry action '"; s += method; s+= "' is not implemented.\n";
  // Why isn't this a memory leak? Dunno, but its seems the TG classes
  // are all managed by TGClient which takes care of deletion
  new TGMsgBox(evdb::TopWindow(),
	       evdb::TopWindow(),
	       "No implementation",s.c_str(),kMBIconExclamation);
  return 0;
}

//......................................................................

int WindowMenu::No3DViewer()
{
  new TGMsgBox(evdb::TopWindow(), 
	       evdb::TopWindow(),
	       "Not for this view",
	       "This display does not implement a 3D viewer",
	       kMBIconExclamation);
  return 0;
}

////////////////////////////////////////////////////////////////////////
