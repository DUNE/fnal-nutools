////////////////////////////////////////////////////////////////////////
/// \file  JobMenu.cxx
/// \brief The job pull down menu
///
/// \version $Id: JobMenu.cxx,v 1.3 2011-01-20 16:43:29 p-nusoftart Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#include "EventDisplayBase/JobMenu.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include "TROOT.h"
#include "TSystem.h"
#include "TGMsgBox.h"
#include "TGMenu.h"
#include "TGLayout.h"
#include "TGFileDialog.h"
#include "TTimer.h"
#include "EventDisplayBase/evdb.h"
#include "EventDisplayBase/EventDisplay.h"

using namespace evdb;

// Define ID codes for the actions on the file menu
enum {
  kM_JOB_OPENXML            = 99001,
  kM_JOB_EDITCONFIG         = 99002,
  kM_JOB_RESETJOB           = 99003,
  kM_JOB_AUTO_ADVANCE_START = 99004,
  kM_JOB_AUTO_ADVANCE_STOP  = 99005
};

//......................................................................

JobMenu::JobMenu(TGMenuBar* menubar, TGMainFrame* mf) :
  fMainFrame(mf)
{
//======================================================================
// Build the help menu
//======================================================================
  fJobMenu = new TGPopupMenu(gClient->GetRoot());
  fLayout  = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0);
  
  fConfigMenu = new TGPopupMenu(gClient->GetRoot());
  // this->BuildConfigMenu();
  
  // Create the list of functions. Associate each which a command code
  fJobMenu->AddEntry("&Load job",  kM_JOB_OPENXML);
  fJobMenu->AddEntry("&Reset Job", kM_JOB_RESETJOB);
  fJobMenu->AddSeparator();
  fJobMenu->AddPopup("&Configure Module", fConfigMenu);
  fJobMenu->AddSeparator();
  fJobMenu->AddEntry("Start Auto Advance", kM_JOB_AUTO_ADVANCE_START);
  fJobMenu->AddEntry("Stop Auto Advance",  kM_JOB_AUTO_ADVANCE_STOP);
  
  fJobMenu->Connect("Activated(Int_t)",
		    "evdb::JobMenu",
		    this,
		    "HandleMenu(int)");
  
  // Attach the menu to the menu bar
  menubar->AddPopup("&Job",fJobMenu,fLayout);
}

//......................................................................

JobMenu::~JobMenu() 
{
//======================================================================
// Delete the job menu
//======================================================================
  if (fLayout)  { delete fLayout;  fLayout  = 0; }
  if (fJobMenu) { delete fJobMenu; fJobMenu = 0; }
}

//......................................................................

void JobMenu::SetWorkers(const std::vector<std::string>& w)
{
  // Wipe out the existing menus and lists
  for (unsigned int i=0;;++i) {
    TGMenuEntry* m = fConfigMenu->GetEntry(i);
    if (m) fConfigMenu->DeleteEntry(i);
    else   break;
  }
  
  // Rebuild the list
  for (unsigned int i=0; i<w.size(); ++i) {
    fConfigMenu->AddEntry(w[i].c_str(), i);
  }
  fConfigMenu->Connect("Activated(Int_t)",
		       "evdb::JobMenu",
		       this,
		       "EditConfig(int)");
}

//......................................................................

void JobMenu::HandleMenu(int menu) 
{
//======================================================================
// Take care of menu events
//======================================================================
  switch(menu) {
  case kM_JOB_OPENXML: 
    this->OpenJobXML(); 
    break;
  case kM_JOB_RESETJOB: 
    this->ResetJob(); 
    break;
  case kM_JOB_AUTO_ADVANCE_START: 
    // IoModule::Instance()->StartAutoAdvance();
    break;
  case kM_JOB_AUTO_ADVANCE_STOP:
    // IoModule::Instance()->StopAutoAdvance();
    break;
  default:
    // This is bogus, but it works. TODO: Figure out why the fConfigMenu
    // signals are misrouted here
    this->EditConfig(menu);
    break;
  }
}

//......................................................................

void JobMenu::EditConfig(int i)
{  
  art::ServiceHandle<evdb::EventDisplay> evd;
  evd->EditWorkerParameterSet(i);
}

//......................................................................

int JobMenu::OpenJobXML() 
{
  static TString dir(getenv("SRT_PRIVATE_CONTEXT"));
  const char* filetypes[] = {
    "Python Files", "*.py",
    0,           0
  };
  
  TGFileInfo finfo;
  finfo.fIniDir    = StrDup(dir.Data());
  finfo.fFileTypes = filetypes;

  new TGFileDialog(gClient->GetRoot(), 
		   gClient->GetRoot(),
		   kFDOpen,
		   &finfo);
  if (finfo.fFilename == 0) return 0;

  // Add this directory to the top of the XML path. Helps get the
  // supporting configuration XML loaded
  // xmli::AddToPath(finfo.fIniDir,-1);

  // Set flags that allows all configurations to be editted
  // cfg::Config::SetEditAllOK();
  
  // Clear existing job and then load the job xml file
  // jobc::Stack::Instance().CleanUp();
  // xmli::ReadFile(finfo.fFilename);
  // jobc::Stack::Instance().PrintAll();

  // Rebuild the menus to relect any new entries
  // this->BuildConfigMenu();

  // Reload the current event to trigger processing
  // IoModule::Instance()->Reload();

  return 0;
}

//......................................................................

void JobMenu::ResetJob() 
{
  // jobc::Stack::Instance().CleanUp();
}

////////////////////////////////////////////////////////////////////////
