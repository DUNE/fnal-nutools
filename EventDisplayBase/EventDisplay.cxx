///
/// \file  EventDisplay.cxx
/// \brief The interactive event display
///
/// \version $Id: EventDisplay.cxx,v 1.11 2011-03-29 16:06:07 greenc Exp $
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
#include "art/Framework/Services/Registry/ServiceRegistry.h"

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

  reg.watchPostBeginJob       (this, &EventDisplay::postBeginJob);
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

void EventDisplay::postBeginJob() 
{
  std::vector< fhicl::ParameterSet > psets;
  art::ServiceRegistry& inst = art::ServiceRegistry::instance();
  inst.presentToken().getParameterSets(psets);

  // loop over the service parameter sets and get the names of those
  // that have DrawingOptions in their names

  fDrawingOptions.clear();
  for(size_t i = 0; i < psets.size(); ++i){

    std::string pset = psets[i].to_string();

    if(pset.find("DrawingOptions") != std::string::npos)      
      fDrawingOptions.push_back(psets[i].get<std::string>("service_type"));

  }

  fDrawingParamSets.resize(fDrawingOptions.size());
  evdb::DisplayWindow::SetDrawingOptionsAll(fDrawingOptions);

}

//......................................................................

void EventDisplay::EditWorkerParameterSet(int i) 
{
  fhicl::ParameterSet params;
  
  fhicl::ParameterSetRegistry::get(fWorkers[i]->description().parameterSetID(), params);
  
  new ParameterSetEdit(0,
		       fWorkers[i]->description().moduleName(),
		       fWorkers[i]->description().moduleLabel(),
		       params.to_string(),
		       &fParamSets[i]);

}

//......................................................................

void EventDisplay::EditDrawingOptionParameterSet(int i) 
{
  std::vector< fhicl::ParameterSet > psets;
  art::ServiceRegistry& inst = art::ServiceRegistry::instance();
  inst.presentToken().getParameterSets(psets);

  for(size_t ps = 0; ps < psets.size(); ++ps){
    if(psets[ps].get<std::string>("service_type", "none").compare(fDrawingOptions[i]) == 0){
      new ParameterSetEdit(0,
			   "Service",
			   fDrawingOptions[i],
			   psets[ps].to_string(),
			   &fDrawingParamSets[i]);
    }// end if the correct configuration
  }

}

//......................................................................

void EventDisplay::preProcessEvent(art::EventID const& evtid,
				   art::Timestamp const&) 
{
  evdb::DisplayWindow::SetRunEventAll(evtid.run(), evtid.event());
}

//......................................................................

void EventDisplay::ReconfigureWorkers()
{
  // Look to see if we have any new configurations to apply
  for (unsigned int i=0; i<fParamSets.size(); ++i) {
    try {
      if (!fParamSets[i].empty()) {
	fhicl::ParameterSet pset;
	fhicl::intermediate_table itable;
	fhicl::parse_document(fParamSets[i], itable); // May throw on error: should check.
	fhicl::make_ParameterSet(itable, pset); // May throw on error: should check.
	fParamSets[i] = "";
	fWorkers[i]->reconfigure(pset);
      }
    }
    catch (fhicl::exception& e) {
      std::cout << "Error parsing the new configuration:\n"
		<< e
		<< "\nRe-configuration has been ignored for module: "
		<< fWorkers[i]->label()
		<< std::endl;
    }
  }
}

//......................................................................

void EventDisplay::ReconfigureDrawingOptions()
{
  // Look to see if we have any new service configurations to apply
  art::ServiceRegistry& inst = art::ServiceRegistry::instance();
  std::vector< fhicl::ParameterSet > psets;
  inst.presentToken().getParameterSets(psets);
  for(size_t ps = 0; ps < psets.size(); ++ps){    
    for (unsigned int i=0; i<fDrawingParamSets.size(); ++i) {
      
      if(fDrawingOptions[i].compare(psets[ps].get<std::string>("service_type", "none")) == 0){
	try {
	  if (!fDrawingParamSets[i].empty()) {
	    fhicl::ParameterSet pset;
	    fhicl::intermediate_table itable;
	    // Each of the next 2 lines may throw on error: should check.
	    fhicl::parse_document(fDrawingParamSets[i], itable); 
	    fhicl::make_ParameterSet(itable, pset); 
	    fDrawingParamSets[i] = "";
	    psets[ps] = pset;
	  }
	}
	catch (fhicl::exception& e) {
	  std::cout << "Error parsing the new configuration:\n"
		    << e
		    << "\nRe-configuration has been ignored for service: "
		    << fDrawingOptions[i]
		    << std::endl;
	}
      }// end if this is the right service in the list
    }// end loop over drawing options
  }// end loop over service parameter sets
  inst.presentToken().putParameterSets(psets);
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

  this->ReconfigureWorkers();
  this->ReconfigureDrawingOptions();

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
     art::EventID id(art::SubRunID::invalidSubRun(art::RunID(NavState::TargetRun())), NavState::TargetEvent());
    fInputSource->readEvent(id);
    fInputSource->skipEvents(-1);
  }
  else abort();
}

////////////////////////////////////////////////////////////////////////
