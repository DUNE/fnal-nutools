///
/// \file  EventDisplay.cxx
/// \brief The interactive event display
///
/// \version $Id: EventDisplay.cxx,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  messier@indiana.edu
///
#include "EventDisplayBase/EventDisplay.h"
#include "EventDisplayBase/DisplayWindow.h"
#include "EventDisplayBase/Canvas.h"
#include "EventDisplayBase/RootEnv.h"
#include "EventDisplayBase/EventHolder.h"
#include "EventDisplayBase/ParameterSetEdit.h"
#include "EventDisplayBase/NavState.h"
#include <iostream>
#include <fstream>
#include "TROOT.h"
#include "TApplication.h"
#include "TText.h"
#include "TCanvas.h"
// ART includes
#include "FWCore/ParameterSet/interface/Registry.h"
#include "FWCore/Framework/interface/InputSource.h"
#include "DataFormats/Provenance/interface/ModuleDescription.h"
#include "FWCore/Framework/src/Worker.h"

using namespace evdb;

//
// Build this as soon as the library is loaded to ensure that our
// interactive session is started before other services that might use
// ROOT get a chance to make their own.
//
static evdb::RootEnv gsRootEnv(0,0);

//......................................................................

class testCanvas1 : public evdb::Canvas
{
public:
  testCanvas1(TGMainFrame* mf) : evdb::Canvas(mf) {
    evdb::Printable::AddToListOfPrintables(this->PrintTag(),this);
  }
  ~testCanvas1() {
    evdb::Printable::RemoveFromListOfPrintables(this);
  }
  const char* Description() const { return "Test Canvas 1"; }
  const char* PrintTag()    const { return "Test1"; }
  void Draw(const char* /* opt */) { 
    static TText* t = new TText(0.5,0.5,"-");
    static int count = 0;
    char buff[256];
    sprintf(buff,"%d",count);
    std::cout << buff << std::endl;
    t->SetText(0.5,0.5,buff);
    t->Draw();
    ++count;
    fCanvas->Update();
  }
};

//......................................................................

static evdb::Canvas* mk_canvas1(TGMainFrame* mf) {
  return new testCanvas1(mf);
}

//......................................................................

EventDisplay::EventDisplay(edm::ParameterSet const& pset,
			   edm::ActivityRegistry& reg) :
  fAutoAdvanceInterval(pset.getParameter<unsigned int>("AutoAdvanceInterval"))
{
//   evdb::DisplayWindow::Register("Test1","Test display #1",600,900,mk_canvas1);
//   evdb::DisplayWindow::OpenWindow(0);

  reg.watchPostBeginJobWorkers(this, &EventDisplay::postBeginJobWorkers);
  reg.watchPreProcessEvent    (this, &EventDisplay::preProcessEvent);
  reg.watchPostProcessEvent   (this, &EventDisplay::postProcessEvent);
}

//......................................................................

EventDisplay::~EventDisplay() { }

//......................................................................

void EventDisplay::postBeginJobWorkers(edm::InputSource* input_source,
				       std::vector<edm::Worker*> const& w) 
{
  fInputSource = input_source;

  std::vector<std::string> lbl;
  fWorkers.clear();
  for(unsigned int i=0; i<w.size(); ++i) {
    std::string s;
    s += w[i]->description().moduleName();
    s += " - ";
    s += w[i]->description().moduleLabel();
    lbl.push_back(s);
    
    fWorkers.push_back(w[i]);
  }
  fParamSets.resize(fWorkers.size());

  evdb::DisplayWindow::SetWorkersAll(lbl);
}

//......................................................................

void EventDisplay::EditWorkerParameterSet(int i) 
{
  edm::ParameterSet params;
  
  edm::pset::Registry::instance()->
    getMapped(fWorkers[i]->description().parameterSetID(), params);
  
  std::string newpset;
  new ParameterSetEdit(0,
		       fWorkers[i]->description().moduleName(),
		       fWorkers[i]->description().moduleLabel(),
		       params.toString(),
		       &fParamSets[i]);
}

//......................................................................

void EventDisplay::preProcessEvent(edm::EventID const& evtid,
				   edm::Timestamp const&) 
{
  evdb::DisplayWindow::SetRunEventAll(evtid.run(), evtid.event());
}

//......................................................................

void EventDisplay::postProcessEvent(edm::Event const& evt,
				    edm::EventSetup const& )
{
  // stuff the event into the holder
  evdb::EventHolder *holder = evdb::EventHolder::Instance();
  holder->SetEvent(&evt);

  evdb::DisplayWindow::DrawAll();

  TApplication* app = gROOT->GetApplication();

  // Hold here for user input from the GUI...
  app->Run(kTRUE);

  // Look to see if we have any new configurations to apply
  for (unsigned int i=0; i<fParamSets.size(); ++i) {
    if (fParamSets[i]!="") {
      edm::ParameterSet pset;
      pset.fromString(fParamSets[i]);
      fParamSets[i] = "";
      fWorkers[i]->reconfigure(std::cin, std::cout, pset);
    }
  }

  // Figure out where to go in the input stream from here
  if (NavState::Which() == kNEXT_EVENT) {
    // This happens by default -- nothing to do...
    // noop
  }
  else if (NavState::Which() == kPREV_EVENT) {
    fInputSource->skipEvents(-2); 
  }
  else if (NavState::Which() == kRELOAD_EVENT) {
    fInputSource->skipEvents(-1); 
  }
  else if (NavState::Which() == kGOTO_EVENT) {
    std::cout << "GOTO EVENT run=" << fInputSource->run() << std::endl;
  }
  else abort();
}

////////////////////////////////////////////////////////////////////////
