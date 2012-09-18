////////////////////////////////////////////////////////////////////////
/// \file ReweightAna.h
//
/// \version $Id: ReweightAna.h,v 1.1 2012-09-18 16:15:58 nsmayer Exp $
/// \author  
////////////////////////////////////////////////////////////////////////
#ifndef RWGT_REWEIGHT_H
#define RWGT_REWEIGHT_H

#include "art/Framework/Core/EDAnalyzer.h"

#include "RawData/DAQHeader.h"

namespace simb  { class MCTruth;      }
namespace simb  { class GTruth;       }
namespace rwgt  {class GENIEReweight;  }

namespace sumdata  { class POTSum; }
class TH1F;
class TH2F;

namespace rwgt {

  //class GENIEReweight;
  /// A module to check the results from the Monte Carlo generator
  
  class ReweightAna : public art::EDAnalyzer {
    
  public:
    explicit ReweightAna(fhicl::ParameterSet const& pset);
    virtual ~ReweightAna();

    void analyze(art::Event const& evt);
    void beginSubRun(art::SubRun const& sr);
    void beginJob();
    void endJob();
    void endSubRun(art::SubRun const& sr);
    void reconfigure(const fhicl::ParameterSet& p);

  private:
    void LoadMCInfo(art::Event const& evt);

  private:
    TH1F* fEnergyNeutrino;             //< Total number of events 
    TH1F* fNeventsSubrun;       ///< Total number of events per subrun
    TH1F* fNeventsPOTSubrun;    // Events per POT per subrun
    TH1F* fPOTSubrun;           // POT per subrun
    TH1F* fPOTSpillSubrun;      // POT per spill per subrun
    TH1F* fWgtQE[3];
    TH1F* fWgtRES[3];
    TH1F* fWgtDIS[3];
    rwgt::GENIEReweight* fGrwgt[3];  //X-sec weight calculator

    sumdata::POTSum* fPOTSum;
    
    std::string fMCTruthModuleLabel; ///< label for module producing mc truth information
    //std::string fGTruthModuleLabel;  ///< label for module producing generator truth info
    std::string fPotLabel;           ///< Module that produced the POTSum object

  };
};

#endif // MCCHK_POTANA_H
////////////////////////////////////////////////////////////////////////
