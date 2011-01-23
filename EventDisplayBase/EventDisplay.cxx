///
/// \file  EventDisplay.cxx
/// \brief The interactive event display
///
/// \version $Id: EventDisplay.cxx,v 1.5 2011-01-23 17:31:54 p-nusoftart Exp $
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
#include "fhiclcpp/parse.h"
#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/make_ParameterSet.h"
#include "fhiclcpp/ParameterSetRegistry.h"
#include "art/Framework/Core/InputSource.h"
#include "art/Persistency/Provenance/ModuleDescription.h"
#include "art/Framework/Core/Worker.h"

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

EventDisplay::EventDisplay(fhicl::ParameterSet const& pset,
			   art::ActivityRegistry& reg) :
  fAutoAdvanceInterval(pset.get<unsigned int>("AutoAdvanceInterval"))
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

void EventDisplay::postBeginJobWorkers(art::InputSource* input_source,
				       std::vector<art::Worker*> const& w) 
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
  fhicl::ParameterSet params;
  
  fhicl::ParameterSetRegistry::get(fWorkers[i]->description().parameterSetID(), params);
  
  std::string newpset;
  new ParameterSetEdit(0,
		       fWorkers[i]->description().moduleName(),
		       fWorkers[i]->description().moduleLabel(),
		       params.to_string(),
		       &fParamSets[i]);

}

//......................................................................

void EventDisplay::preProcessEvent(art::EventID const& evtid,
				   art::Timestamp const&) 
{
  evdb::DisplayWindow::SetRunEventAll(evtid.run(), evtid.event());
}

//......................................................................

void EventDisplay::postProcessEvent(art::Event const& evt )
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
      fhicl::ParameterSet pset;
      fhicl::intermediate_table itable;
      fhicl::parse_document(fParamSets[i], itable); //returns a bool for error checking
      fhicl::make_ParameterSet(itable, pset); //returns a bool for error checking
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
