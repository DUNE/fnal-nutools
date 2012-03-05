///
/// \file  EventDisplay.cxx
/// \brief The interactive event display
///
/// \version $Id: EventDisplay.cxx,v 1.24 2012-03-05 17:50:08 brebel Exp $
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
#include "art/Framework/IO/Root/RootInput.h"
#include "art/Persistency/Provenance/ModuleDescription.h"
#include "art/Framework/Principal/Worker.h"
#include "art/Framework/Services/Registry/ServiceRegistry.h"
#include "art/Persistency/Provenance/EventID.h"
#include "art/Utilities/Exception.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

namespace evdb{

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
      mf::LogWarning("EventDisplayBase") << buff;
      t->SetText(0.5,0.5,buff);
      t->Draw();
      ++count;
      fCanvas->Update();
    }
  };

  //......................................................................
  // Uncomment this static method for testing purposes
  // static evdb::Canvas* mk_canvas1(TGMainFrame* mf) {
  //   return new testCanvas1(mf);
  // }

  //......................................................................
  EventDisplay::EventDisplay(fhicl::ParameterSet const& pset,
			     art::ActivityRegistry& reg) 
    : fAutoPrintCount(0)
  {
    //   evdb::DisplayWindow::Register("Test1","Test display #1",600,900,mk_canvas1);
    //   evdb::DisplayWindow::OpenWindow(0);

    reg.watchPostBeginJob       (this, &EventDisplay::postBeginJob);
    reg.watchPostBeginJobWorkers(this, &EventDisplay::postBeginJobWorkers);
    reg.watchPreProcessEvent    (this, &EventDisplay::preProcessEvent);
    reg.watchPostProcessEvent   (this, &EventDisplay::postProcessEvent);

    this->reconfigure(pset);
  }

  //......................................................................
  void EventDisplay::reconfigure(fhicl::ParameterSet const& pset) 
  {
    fAutoAdvanceInterval = pset.get<unsigned int>("AutoAdvanceInterval");
    fAutoPrintMax        = pset.get<int         >("AutoPrintMax",     0);
    fAutoPrintPattern    = pset.get<std::string >("AutoPrintPattern"   );
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
    fServices.clear();
    for(size_t i = 0; i < psets.size(); ++i){

      std::string stype = psets[i].get<std::string>("service_type","none");

      if(stype.find("DrawingOptions") != std::string::npos)      
	fDrawingOptions.push_back(stype);
      else if(stype.find("Timing")                == std::string::npos &&
	      stype.find("TFileService")          == std::string::npos &&
	      stype.find("SimpleMemoryCheck")     == std::string::npos &&
	      stype.find("message")               == std::string::npos &&
	      stype.find("scheduler")             == std::string::npos &&
	      stype.find("RandomNumberGenerator") == std::string::npos &&
	      stype.find("none")                  == std::string::npos 
	      )
	fServices.push_back(stype);

    }
  
    fServiceParamSets.resize(fServices.size());
    fDrawingParamSets.resize(fDrawingOptions.size());
    evdb::DisplayWindow::SetDrawingOptionsAll(fDrawingOptions);
    evdb::DisplayWindow::SetServicesAll(fServices);

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
  void EventDisplay::EditServiceParameterSet(int i) 
  {
    std::vector< fhicl::ParameterSet > psets;
    art::ServiceRegistry& inst = art::ServiceRegistry::instance();
    inst.presentToken().getParameterSets(psets);

    for(size_t ps = 0; ps < psets.size(); ++ps){
      if(psets[ps].get<std::string>("service_type", "none").compare(fServices[i]) == 0){
	new ParameterSetEdit(0,
			     "Service",
			     fServices[i],
			     psets[ps].to_string(),
			     &fServiceParamSets[i]);
      }// end if the correct configuration
    }

  }

  //......................................................................
  void EventDisplay::preProcessEvent(art::Event const & evt) 
  {
    evdb::DisplayWindow::SetRunEventAll(evt.id().run(), evt.id().event());
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
	mf::LogError("EventDisplayBase") << "Error parsing the new configuration:\n"
					 << e
					 << "\nRe-configuration has been ignored for module: "
					 << fWorkers[i]->label();
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
	    mf::LogError("EventDisplayBase") << "Error parsing the new configuration:\n"
					     << e
					     << "\nRe-configuration has been ignored for service: "
					     << fDrawingOptions[i];
	  }
	}// end if this is the right service in the list
      }// end loop over drawing options
    }// end loop over service parameter sets
    inst.presentToken().putParameterSets(psets);
  }

  //......................................................................
  void EventDisplay::ReconfigureServices()
  {
    // Look to see if we have any new service configurations to apply
    art::ServiceRegistry& inst = art::ServiceRegistry::instance();
    std::vector< fhicl::ParameterSet > psets;
    inst.presentToken().getParameterSets(psets);
    for(size_t ps = 0; ps < psets.size(); ++ps){    
      for (unsigned int i=0; i<fServiceParamSets.size(); ++i) {
      
	if(fServices[i].compare(psets[ps].get<std::string>("service_type", "none")) == 0){
	  try {
	    if (!fServiceParamSets[i].empty()) {
	      fhicl::ParameterSet pset;
	      fhicl::intermediate_table itable;
	      // Each of the next 2 lines may throw on error: should check.
	      fhicl::parse_document(fServiceParamSets[i], itable); 
	      fhicl::make_ParameterSet(itable, pset); 
	      fServiceParamSets[i] = "";
	      psets[ps] = pset;
	    }
	  }
	  catch (fhicl::exception& e) {
	    mf::LogError("EventDisplayBase") << "Error parsing the new configuration:\n"
					     << e
					     << "\nRe-configuration has been ignored for service: "
					     << fServices[i];
	  }
	}// end if this is the right service in the list
      }// end loop over services
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

    if(fAutoPrintMax == 0){
      TApplication* app = gROOT->GetApplication();

      // Hold here for user input from the GUI...
      app->Run(kTRUE);
    }

    this->ReconfigureWorkers();
    this->ReconfigureDrawingOptions();
    this->ReconfigureServices();

    if(fAutoPrintMax > 0){
      ++fAutoPrintCount;
      std::map<std::string, Printable*>& ps = Printable::GetPrintables();
      for(std::map<std::string,Printable*>::iterator it = ps.begin(); it != ps.end(); ++it){
	Printable* p = it->second;
	// Ensure the format string is well-formed
	assert(fAutoPrintPattern.find("%s") != std::string::npos);
	assert(fAutoPrintPattern.find("%d") != std::string::npos);
	// png doesn't seem to work for some reason
	p->Print(TString::Format(fAutoPrintPattern.c_str(), p->PrintTag(), evt.event()));
      }
      if(fAutoPrintCount >= fAutoPrintMax) exit(0);
    }

    art::RootInput* rootInput = dynamic_cast<art::RootInput*>(fInputSource);

    if(!rootInput && NavState::Which() != kSEQUENTIAL_ONLY){
      NavState::Set(kSEQUENTIAL_ONLY);
      mf::LogWarning("EventDisplayBase")
	<< "Random access for the EventDisplay requires a RootInput source for proper operation.\n"
	<< "You do not have a RootInput source so only sequential access works.\n";
    }


    // Figure out where to go in the input stream from here
    switch (NavState::Which()) {
    case kSEQUENTIAL_ONLY: break;
    case kNEXT_EVENT: {
      // Contrary to appearances, this is *not* a NOP: it ensures run and
      // subRun are (re-)read as necessary if we've been doing random
      // access. Come the revolution ...
      //
      // 2011/04/10 CG.
      if(rootInput) rootInput->seekToEvent(0);
      break;
    }
    case kPREV_EVENT: {
      if(rootInput) rootInput->seekToEvent(-2);
      break;
    }
    case kRELOAD_EVENT: {
      if(rootInput) rootInput->seekToEvent(evt.id());
      break;
    }
    case kGOTO_EVENT: {
      art::EventID id(art::SubRunID::invalidSubRun(art::RunID(NavState::TargetRun())), NavState::TargetEvent());
      if(rootInput){
	if (!rootInput->seekToEvent(id)) { // Couldn't find event
	  mf::LogWarning("EventDisplayBase") << "Unable to find "
					     << id
					     << " -- reloading current event.";
	  // Reload current event.
	  rootInput->seekToEvent(evt.id());
	}
      }// end if not a RootInput
      break;
    }
    default: {
      throw art::Exception(art::errors::LogicError)
	<< "EvengtDisplay in unhandled state "
	<< NavState::Which()
	<< ".\n";
    }
    }

  }

}//namespace
////////////////////////////////////////////////////////////////////////
