////////////////////////////////////////////////////////////////////////
/// \version $Id: ReweightAna.cxx,v 1.2 2012-09-19 17:15:48 nsmayer Exp $
// 
////////////////////////////////////////////////////////////////////////
#include "NuReweight/ReweightAna.h"
#include <vector>
#include <cmath>
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TDatabasePDG.h"
#include "TSystem.h"
#include "SimulationBase/MCTruth.h"
#include "SimulationBase/GTruth.h"
#include "SimulationBase/MCNeutrino.h"
#include "SimulationBase/MCFlux.h"
#include "Geometry/geo.h"

#include "art/Framework/Principal/Event.h" 
#include "art/Framework/Principal/SubRun.h" 
#include "fhiclcpp/ParameterSet.h" 
#include "art/Framework/Principal/Handle.h" 
#include "art/Persistency/Common/Ptr.h" 
#include "art/Persistency/Common/PtrVector.h" 
#include "art/Framework/Services/Registry/ServiceHandle.h" 
#include "art/Framework/Services/Optional/TFileService.h" 
#include "art/Framework/Services/Optional/TFileDirectory.h" 
#include "messagefacility/MessageLogger/MessageLogger.h" 
#include "Utilities/AssociationUtil.h"

//#include "EventGeneratorBase/evgenbase.h"
//#include "NuReweight/reweight.h"
#include "NuReweight/GENIEReweight.h"
//#include "ReWeight/GSystSet.h"
//#include "ReWeight/GSyst.h"
//#include "EventGeneratorBase/ReweightLabels.h"


namespace rwgt{

  //  static int msg1cnt = 0;
  static int cntEvent = 0; //event number
  //  static int cntFLSHit = 0; //FLS hit number

  //......................................................................
  
  
  ReweightAna::ReweightAna(fhicl::ParameterSet const& p)
  {
    this->reconfigure(p);
  }
  

  //......................................................................
  
  ReweightAna::~ReweightAna() { }
  
  //......................................................................
  void ReweightAna::beginJob()
  {
    art::ServiceHandle<art::TFileService> tfs;
    art::ServiceHandle<geo::Geometry> geo;
   
    std::cout << "make histograms" << std::endl;
    fEnergyNeutrino   = tfs->make<TH1F>("fEnergyneutrino", "Total number of events", 50, 0., 25); 
    fNeventsSubrun    = tfs->make<TH1F>("fNeventsSubrun", "Total number of events", 1, 0., 1.); 
    //fNeventsPOTSubrun = tfs->make<TH1F>("fNeventsPOTSubrun", "Total number of events", 1, 0., 1.); 
    //fPOTSubrun        = tfs->make<TH1F>("fPOTSubrun", "Total number of events", 1, 0., 1.); 
    //fPOTSpillSubrun   = tfs->make<TH1F>("fPOTSpillSubrun", "Total number of events", 1, 0., 1.); 
    char name[300];
    for(int i = 0; i < 3; i++) {
      sprintf(name, "fWgtQE_%dsigma", i+1);
      fWgtQE[i] = tfs->make<TH1F>(name, "Evt Wgts", 100, 0., 2.0); 
      sprintf(name, "fWgtRES_%dsigma", i+1);
      fWgtRES[i] = tfs->make<TH1F>(name, "Evt Wgts", 100, 0., 2.0); 
      sprintf(name, "fWgtDIS_%dsigma", i+1);
      fWgtDIS[i] = tfs->make<TH1F>(name, "Evt Wgts", 100, 0., 2.0); 

      double sigma = (double)(i+1);
      fGrwgt[i] = new rwgt::GENIEReweight();
      //fGrwgt[i]->AddReweightValue(genie::rew::kXSecTwkDial_NormCCQE, 4.);
      fGrwgt[i]->AddReweightValue(rwgt::fReweightMaCCQE, sigma);
      fGrwgt[i]->AddReweightValue(rwgt::fReweightMaCCRES, sigma);
      fGrwgt[i]->AddReweightValue(rwgt::fReweightMaNCRES, sigma);
      fGrwgt[i]->AddReweightValue(rwgt::fReweightRvpCC1pi, sigma);
      fGrwgt[i]->AddReweightValue(rwgt::fReweightRvnCC1pi, sigma);
      fGrwgt[i]->AddReweightValue(rwgt::fReweightRvpCC2pi, sigma);
      fGrwgt[i]->AddReweightValue(rwgt::fReweightRvnCC2pi, sigma);
      fGrwgt[i]->AddReweightValue(rwgt::fReweightRvpNC1pi, sigma);
      fGrwgt[i]->AddReweightValue(rwgt::fReweightRvnNC1pi, sigma);
      fGrwgt[i]->AddReweightValue(rwgt::fReweightRvpNC2pi, sigma);
      fGrwgt[i]->AddReweightValue(rwgt::fReweightRvnNC2pi, sigma);
      
      fGrwgt[i]->Configure();
    }
  }
  
  //......................................................................
  void ReweightAna::beginSubRun(art::SubRun const& sr) {
    
    std::cerr << "beginSubRun" << std::endl;
    //art::Handle< sumdata::POTSum > p;
    //sr.getByLabel(fPotLabel,p);

    std::cerr << "temp" << std::endl;

    //fPOTSum->totpot     = p->totpot;
    //fPOTSum->totgoodpot = p->totgoodpot;
    //fPOTSum->totspills  = p->totspills;
    std::cerr << "temp" << std::endl;
    //fPOTSum->goodspills = p->goodspills;
    //std::cerr << "end beginSubRun" << std::endl;

  }

  //......................................................................
  void ReweightAna::analyze(art::Event const& evt)
  { 

    // // Pull the MC generator information out of the event
    std::cout << "Start analyze" << std::endl;
    art::Handle< std::vector<simb::MCTruth> > mclist;
    evt.getByLabel(fMCTruthModuleLabel, mclist);
    if (mclist->empty()) {
      std::cerr << "Error retrieving MCTruth list" << std::endl;
       return;
     }
    
    //art::Handle< std::vector<simb::MCFlux> > fllist;
    //evt.getByLabel(fMCTruthModuleLabel, fllist);
    //if (fllist->empty()) {
    //if(msg1cnt < 5){
    //std::cerr << "ooops file made before MCFlux existed.  let it slide" 
    //<< std::endl;
    //++msg1cnt;
    //}
    
    art::Handle< std::vector<simb::GTruth> > gtlist;
    evt.getByLabel(fMCTruthModuleLabel, gtlist);
    if (gtlist->empty()) {
      std::cerr << "Error retrieving GTruth list" << std::endl;
      return;
    }
    
    std::cout<<"MC List sizes:" << mclist->size() << " " << gtlist->size() << "\n";
    
    // // Loop over neutrino interactions
    for(unsigned int i_intx = 0; i_intx < mclist->size(); ++i_intx){
      std::cout << "start loop" << std::endl;
      // Pointers to current MCTruth and MCFlux
      art::Ptr<simb::MCTruth> mc    (mclist,i_intx);
      std::cerr << "GTruth" << std::endl;
      art::Ptr<simb::GTruth> gt (gtlist, i_intx);
      //   art::Ptr<simb::MCFlux>  mcflux(fllist,i_intx);
      
      //   // Link to the MCNeutrino class.
      //   // The class contains information not only about
      //   // the incoming neutrino, but about the products of the decay
      const simb::MCNeutrino& mc_neutrino = mc->GetNeutrino();
      simb::MCTruth truth = *mc;
      simb::GTruth gtruth = *gt;
      fEnergyNeutrino->Fill(mc_neutrino.Nu().E());
      for(int i = 0; i < 3; i++) {
	double wgt = fGrwgt[i]->CalcWeight(truth, gtruth);
	//double wgt = 1.;
	if(mc_neutrino.Mode()==0 && mc_neutrino.CCNC()==0) {
	  fWgtQE[i]->Fill(wgt);
	}
	else if(mc_neutrino.Mode()==1 && mc_neutrino.CCNC()==0) {
	  fWgtRES[i]->Fill(wgt);
	}
	else if(mc_neutrino.Mode()==2 && mc_neutrino.CCNC()==0) {
	  fWgtDIS[i]->Fill(wgt);
	}
      }
      //   //mc->Print();
      std::cerr << "end loop" << std::endl;
    }//end loop over interactions

 
    
  }
  

  //......................................................................
  
  void ReweightAna::LoadMCInfo(art::Event const& evt)
  {


  }
    
  
  //......................................................................
  void ReweightAna::reconfigure(const fhicl::ParameterSet& p)
  {
    
    fMCTruthModuleLabel = p.get< std::string>("MCTruthModuleLabel");
    //fGTruthModuleLabel = p.get< std::string>("MCTruthModuleLabel");
    fPotLabel = p.get< std::string>("PotLabel");
  }
  
  //......................................................................
  void ReweightAna::endSubRun(art::SubRun const& sr)
  {
    /*
    mf::LogInfo ("ReweightAna") << "Event Number " << cntEvent << std::endl;
    mf::LogInfo ("ReweightAna") << "Subrun Number " << sr.subRun() << std::endl;
    mf::LogInfo ("ReweightAna") << "Event per POT " << cntEvent/fPOTSum->totpot << std::endl;

    fNeventsPOTSubrun->Fill(sr.subRun(), cntEvent/fPOTSum->totpot);
    fPOTSubrun->Fill(sr.subRun(), fPOTSum->totpot);
    */
    //fPOTSpillSubrun->Fill(sr.subRun(), fPOTSum->totpot/fPOTSum->totspills);
    fNeventsSubrun->Fill(sr.subRun(), cntEvent);
    cntEvent = 0;

  }

  //......................................................................
  void ReweightAna::endJob()
  {

  }
  
  
  ////////////////////////////////////////////////////////////////////////
}


