////////////////////////////////////////////////////////////////////////
/// \file DisplayWindow.h
/// \brief A window containing a display of the detector or one of its
/// components
///
/// \version $Id: DisplayWindow.cxx,v 1.3 2011-05-12 15:22:06 brebel Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#include "EventDisplayBase/DisplayWindow.h"
#include <cassert>
#include <iostream>
#include <vector>
#include "TROOT.h"
#include "TGClient.h"
#include "TGWindow.h"
#include "TGFrame.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TH1.h"
#include "EventDisplayBase/evdb.h"
#include "EventDisplayBase/MenuBar.h"
#include "EventDisplayBase/JobMenu.h"
#include "EventDisplayBase/ButtonBar.h"
#include "EventDisplayBase/Canvas.h"
#include "EventDisplayBase/StatusBar.h"

using namespace evdb;

///
/// The collection of open windows
///
static std::vector<DisplayWindow*> gsWindows(64);

//......................................................................

///
/// Tables of information for display windows
///
static std::vector<std::string>     gsName;
static std::vector<std::string>     gsDescription;
static std::vector<unsigned int>    gsHeight;
static std::vector<unsigned int>    gsWidth;
static std::vector<CanvasCreator_t> gsCanvasCreator;

void DisplayWindow::SetRunEventAll(int run, int event) 
{
  for (size_t i=0; i<gsWindows.size(); ++i) {
    if (gsWindows[i]!=0) gsWindows[i]->SetRunEvent(run, event);
  }
}

//......................................................................

void DisplayWindow::SetWorkersAll(const std::vector<std::string>& w)
{
  for (size_t i=0; i<gsWindows.size(); ++i) {
    if (gsWindows[i]!=0) gsWindows[i]->SetWorkers(w);
  }
}


//......................................................................

void DisplayWindow::SetDrawingOptionsAll(const std::vector<std::string>& dopt)
{
  for (size_t i=0; i<gsWindows.size(); ++i) {
    if (gsWindows[i]!=0) gsWindows[i]->SetDrawingOptions(dopt);
  }
}

//......................................................................

void DisplayWindow::DrawAll(const char* opt)
{
  for (size_t i=0; i<gsWindows.size(); ++i) {
    if (gsWindows[i]!=0) gsWindows[i]->Draw(opt);
  }
}

//......................................................................

void DisplayWindow::SetRunEvent(int run, int event) 
{
  fButtonBar->SetRunEvent(run, event);
}

//......................................................................

void DisplayWindow::SetWorkers(const std::vector<std::string>& w) 
{
  fMenuBar->fJobMenu->SetWorkers(w);
}

//......................................................................

void DisplayWindow::SetDrawingOptions(const std::vector<std::string>& dopt) 
{
  fMenuBar->fEditMenu->SetWorkers(dopt);
}

//......................................................................

const std::vector<std::string>& DisplayWindow::Names() { return gsName; }

//......................................................................

///
/// Register a display canvas for use in creating windows
///
void DisplayWindow::Register(const char* name,
			     const char* description,
			     unsigned int h,
			     unsigned int w,
			     CanvasCreator_t creator)
{
  gsName.push_back(std::string(name));
  gsDescription.push_back(std::string(description));
  gsHeight.push_back(h);
  gsWidth.push_back(w);
  gsCanvasCreator.push_back(creator);
  
  if (gsName.size()>gsWindows.size()) gsWindows.resize(gsName.size());
}

//......................................................................

///
/// Create a window given a system-assigned ID number
///
int DisplayWindow::OpenWindow(int type) 
{
  unsigned id = 0;
  if (type>0) id = type;
  if (id>=gsName.size()) return 0;

  DisplayWindow* w = gsWindows[id];
  if (w==0) {
    w = gsWindows[id] = new DisplayWindow(id);
  }
  if (w==0) return 0;
  w->Raise();
  w->Draw();
  
  return 1;
}


//......................................................................

DisplayWindow::DisplayWindow(int id) 
{
  if (gROOT->IsBatch()) assert(0);
  assert(gClient);
  const TGWindow* tgw = gClient->GetRoot();
  assert(tgw);

  // Create the main application window. I need a resize to get the
  // window to draw the first time, so create the window slightly
  // smaller than the intended size. Bogus, but so it goes...
  unsigned int w = gsWidth[id];
  unsigned int h = gsHeight[id];
  fMain = new TGMainFrame(tgw, w-1, h-1);

  // Add items to the main window
  fMenuBar   = new MenuBar(fMain);
  fButtonBar = new ButtonBar(fMain);
  fDisplay   = (*gsCanvasCreator[id])(fMain);
  fStatusBar = new StatusBar(fMain);
  
  fMain->SetWindowName(gsName[id].c_str());
  
  // Now that all the subwindows are attached, do the final layout
  fMain->MapSubwindows();
  fMain->MapWindow();
  
  // Don't understand this, but I need a resize to get things to draw
  // the first time...
  fMain->Resize(w,h);

  // Plug the display into its signal/slots
  fDisplay->Connect();

  // fMain->Connect("CloseWindow()","evdb::DisplayWindow",this,"CloseWindow()");

  // Add to list of windows open
  gsWindows[id] = this;
}

//......................................................................

void DisplayWindow::Draw(const char* opt) { fDisplay->Draw(opt); }

//......................................................................

void DisplayWindow::CloseWindow() { delete this; }

//......................................................................

DisplayWindow::~DisplayWindow()
{
  if (fDisplay)   { delete fDisplay;   fDisplay   = 0; } 
  if (fStatusBar) { delete fStatusBar; fStatusBar = 0; }
  if (fButtonBar) { delete fButtonBar; fButtonBar = 0; }
  if (fMenuBar)   { delete fMenuBar;   fMenuBar   = 0; }
  if (fMain)      { delete fMain;      fMain      = 0; }
  for (unsigned int i=0; i<gsWindows.size(); ++i) {
    if (gsWindows[i] == this) gsWindows[i] = 0;
  }
}

//......................................................................

void DisplayWindow::Raise() { fMain->RaiseWindow(); }

////////////////////////////////////////////////////////////////////////
