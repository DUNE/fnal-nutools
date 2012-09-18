////////////////////////////////////////////////////////////////////////
/// \file  GENIEReweight.cxx
/// \brief Wrapper for reweight neutrino interactions with GENIE
///
/// \author  nathan.mayer@tufts.edu
////////////////////////////////////////////////////////////////////////

// C/C++ includes
#include <math.h>
#include <map>
#include <cassert>
#include <fstream>

//ROOT includes
#include "TFile.h"
#include "TDirectory.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TCollection.h"
#include "TSystem.h"
#include "TString.h"
#include "TRandom.h" //needed for gRandom to be defined
#include "TRegexp.h"

//GENIE includes
#include "Conventions/Units.h"
#include "EVGCore/EventRecord.h"
//#include "EVGDrivers/GMCJDriver.h"
#include "GHEP/GHepUtils.h"

#include "ReWeight/GReWeightI.h"
#include "ReWeight/GSystSet.h"
#include "ReWeight/GSyst.h"
#include "ReWeight/GReWeight.h"
#include "ReWeight/GReWeightNuXSecNCEL.h"
#include "ReWeight/GReWeightNuXSecCCQE.h"
#include "ReWeight/GReWeightNuXSecCCRES.h"
#include "ReWeight/GReWeightNuXSecCOH.h"
#include "ReWeight/GReWeightNonResonanceBkg.h"
#include "ReWeight/GReWeightFGM.h"
#include "ReWeight/GReWeightDISNuclMod.h"
#include "ReWeight/GReWeightResonanceDecay.h"
#include "ReWeight/GReWeightFZone.h"
#include "ReWeight/GReWeightINuke.h"
#include "ReWeight/GReWeightAGKY.h"
#include "ReWeight/GReWeightNuXSecCCQEvec.h"
#include "ReWeight/GReWeightNuXSecNCRES.h" 
#include "ReWeight/GReWeightNuXSecDIS.h"   
#include "ReWeight/GReWeightNuXSecNC.h"  
#include "ReWeight/GSystUncertainty.h"
#include "ReWeight/GReWeightUtils.h" 

#include "Geo/ROOTGeomAnalyzer.h"
#include "Geo/GeomVolSelectorFiducial.h"
#include "Geo/GeomVolSelectorRockBox.h"
#include "Utils/StringUtils.h"
#include "Utils/XmlParserUtils.h"
#include "Interaction/InitialState.h"
#include "Interaction/Interaction.h"
#include "Interaction/Kinematics.h"
#include "Interaction/KPhaseSpace.h"
#include "Interaction/ProcessInfo.h"
#include "Interaction/XclsTag.h"
#include "GHEP/GHepParticle.h"
#include "PDG/PDGCodeList.h"
#include "Conventions/Constants.h" //for calculating event kinematics

//NuTools includes
#include "SimulationBase/simbase.h"
#include "NuReweight/GENIEReweight.h"

// Framework includes
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "cetlib/search_path.h"
#include "cetlib/getenv.h"
#include "cetlib/split_path.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"



namespace rwgt {
  ///<constructor
  GENIEReweight::GENIEReweight() :
    fReweightNCEL(false),
    fReweightQEMA(false),
    fReweightQEVec(false),
    fReweightCCRes(false),
    fReweightNCRes(false),
    fReweightResBkg(false),
    fReweightResDecay(false),
    fReweightNC(false),
    fReweightDIS(false),
    fReweightCoh(false),
    fReweightAGKY(false),
    fReweightDISNucMod(false),
    fReweightFGM(false),
    fReweightFZone(false),
    fReweightINuke(false), 
    fReweightMEC(false),
    fMaQEshape(false),
    fMaCCResShape(false),
    fMaNCResShape(false),
    fDISshape(false),
    fUseSigmaDef(true) {
    
    std::cout << "Create GENIEReweight object" << std::endl;
    
    fWcalc = new genie::rew::GReWeight();
    this->SetNominalValues();
  }
  
  ///<destructor
  GENIEReweight::~GENIEReweight() {
    delete fWcalc;
  }
  
  ///<Set the nominal values for the reweight parameters.
  void GENIEReweight::SetNominalValues() {
    //CCQE Nominal Values
    fNominalParameters[(int)rwgt::fReweightMaNCEL] = 0.99;

    fNominalParameters[(int)rwgt::fReweightEtaNCEL] = 0.12;
    
    //CCQE Nominal Values
    fNominalParameters[(int)rwgt::fReweightNormCCQE] = 1.0;
        
    fNominalParameters[(int)rwgt::fReweightNormCCQEenu] = 1.0;
        
    fNominalParameters[(int)rwgt::fReweightMaCCQEshape] = 0.99;
        
    fNominalParameters[(int)rwgt::fReweightMaCCQE] = 0.99;
    fNominalParameters[(int)rwgt::fReweightVecCCQEshape] = 0.84;
        
    //Resonance Nominal Values
    fNominalParameters[(int)rwgt::fReweightNormCCRES] = 1.0;
    fNominalParameters[(int)rwgt::fReweightMaCCRESshape] = 1.12;
    fNominalParameters[(int)rwgt::fReweightMvCCRESshape]  = 0.84;
    fNominalParameters[(int)rwgt::fReweightMaCCRES] = 1.12;
    fNominalParameters[(int)rwgt::fReweightMvCCRES] = 0.84;
    
    fNominalParameters[(int)rwgt::fReweightNormNCRES] = 1.0;
    fNominalParameters[(int)rwgt::fReweightMaNCRESshape] = 1.12;
    fNominalParameters[(int)rwgt::fReweightMvNCRESshape] = 0.84;
    fNominalParameters[(int)rwgt::fReweightMaNCRES] = 1.12;
    fNominalParameters[(int)rwgt::fReweightMvNCRES] = 0.84;
 
    //Coherent pion nominal values
    fNominalParameters[(int)rwgt::fReweightMaCHOpi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightR0COHpi] = 1.0;
    
    
    // Non-resonance background tweaking parameters:
    fNominalParameters[(int)rwgt::fReweightRvpCC1pi] = 1.0; 
    fNominalParameters[(int)rwgt::fReweightRvpCC2pi] = 1.0; 
    fNominalParameters[(int)rwgt::fReweightRvpNC1pi] = 1.0; 
    fNominalParameters[(int)rwgt::fReweightRvpNC2pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightRvnCC1pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightRvnCC2pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightRvnNC1pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightRvnNC2pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightRvbarpCC1pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightRvbarpCC2pi] = 1.0; 
    fNominalParameters[(int)rwgt::fReweightRvbarpNC1pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightRvbarpNC2pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightRvbarnCC1pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightRvbarnCC2pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightRvbarnNC1pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightRvbarnNC2pi] = 1.0;

    
    // DIS tweaking parameters - applied for DIS events with [Q2>Q2o, W>Wo], typically Q2okReweight =1GeV^2, WokReweight =1.7-2.0GeV
    fNominalParameters[(int)rwgt::fReweightAhtBY] = 0.538;
    fNominalParameters[(int)rwgt::fReweightBhtBY] = 0.305;
    fNominalParameters[(int)rwgt::fReweightCV1uBY] = 0.291;
    fNominalParameters[(int)rwgt::fReweightCV2uBY] = 0.189;
    
    fNominalParameters[(int)rwgt::fReweightAhtBYshape] = 0.538;
    fNominalParameters[(int)rwgt::fReweightBhtBYshape] = 0.305;
    fNominalParameters[(int)rwgt::fReweightCV1uBYshape] = 0.291;
    fNominalParameters[(int)rwgt::fReweightCV2uBYshape] = 0.189;
    
    fNominalParameters[(int)rwgt::fReweightNormDISCC] = 1.0;
    

    fNominalParameters[(int)rwgt::fReweightRnubarnuCC] = 0.0;//v to vbar ratio reweighting is not working in GENIE at the moment
    fNominalParameters[(int)rwgt::fReweightDISNuclMod] = 0.0;//The DIS nuclear model switch is not working in GENIE at the moment
    //

    fNominalParameters[(int)rwgt::fReweightNC] = 1.0;
        
    //
    // Hadronization [free nucleon target]
    // 
    fNominalParameters[(int)rwgt::fReweightAGKY_xF1pi] = 0.385; 
    fNominalParameters[(int)rwgt::fReweightAGKY_pT1pi] = 1./6.625;
        
    //
    // Medium-effects to hadronization
    // 
    fNominalParameters[(int)rwgt::fReweightFormZone] = 1.0;
        
    //
    // Intranuclear rescattering systematics.
    fNominalParameters[(int)rwgt::fReweightMFP_pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightMFP_N] = 1.0;
    fNominalParameters[(int)rwgt::fReweightFrCEx_pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightFrElas_pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightFrInel_pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightFrAbs_pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightFrPiProd_pi] = 1.0;
    fNominalParameters[(int)rwgt::fReweightFrCEx_N] = 1.0;
    fNominalParameters[(int)rwgt::fReweightFrElas_N] = 1.0;
    fNominalParameters[(int)rwgt::fReweightFrInel_N] = 1.0;
    fNominalParameters[(int)rwgt::fReweightFrAbs_N] = 1.0;
    fNominalParameters[(int)rwgt::fReweightFrPiProd_N] = 1.0;
    

    //
    //RFG Nuclear model
    // 
    fNominalParameters[(int)rwgt::fReweightCCQEPauliSupViaKF] = 1.0;
    fNominalParameters[(int)rwgt::fReweightCCQEMomDistroFGtoSF] = 0.0; //Continous "switch" at 0.0 full FG model.  At 1.0 full spectral function model.  From genie code it looks like weird stuff may happen for <0 and >1.
    //This parameter does not have an "uncertainty" value associated with it.  The tweaked dial value gets passed all the way through unchanged to the weight calculator
    
    //
    // Resonance decays
    // 
    fNominalParameters[(int)rwgt::fReweightBR1gamma] = 1.0;
    fNominalParameters[(int)rwgt::fReweightBR1eta] = 1.0;
    fNominalParameters[(int)rwgt::fReweightTheta_Delta2Npi] = 0.0; //Continous "switch" at 0.0 full isotropic pion angular distribution.  At 1.0 full R/S pion angular distribtuion.
    //This parameter does not have an "uncertainty" value associated with it.  The tweaked dial value gets passed all the way through unchanged to the weight calculator
  }

  ///<Return the nominal value for the given parameter.
  double GENIEReweight::NominalParameterValue(ReweightLabel_t rLabel) {
    double nominal_value;
    nominal_value = fNominalParameters[rLabel];
    return nominal_value;
  }

  ///<Return the configured value of the given parameter.
  double GENIEReweight::ReweightParameterValue(ReweightLabel_t rLabel) {
    int label = (int)rLabel;
    bool in_loop = false;
    bool found_par = false;
    double parameter = -10000;
    for(unsigned int i = 0; i < fReWgtParameterName.size(); i++) {
      in_loop = true;
      if(label == fReWgtParameterName[i]) {
	parameter = fReWgtParameterValue[i];
	found_par = true;
      }
    }
    if(in_loop) {
      if(found_par) return parameter;
      else {
	mf::LogWarning("GENIEReweight") << "Parameter " << label << " not set yet or does not exist";
	return parameter;
      }
    }
    else {
      mf::LogWarning("GENIEReweight") << "Vector of parameters has not been initialized yet (Size is 0).";
      return parameter;
    }
  }
  
  ///<Add reweight parameters to the list
  void GENIEReweight::AddReweightValue(ReweightLabel_t rLabel, double value) {
    int label = (int)rLabel;   
    std::cout << "Adding parameter: " << label << ".  With value: " << value << std::endl;
    fReWgtParameterName.push_back(label);
    fReWgtParameterValue.push_back(value);
    
  }
  
  ///<Change a reweight parameter.  If it hasn't been added yet add it.
  void GENIEReweight::ChangeParameterValue(ReweightLabel_t rLabel, double value) {
    int label = (int)rLabel;
    bool found = false;
    for(unsigned int i = 0; i < fReWgtParameterName.size(); i++) {
      if(fReWgtParameterName[i] == label) {
	fReWgtParameterValue[i] = value;
	found = true;
      }
    }
    if(!found) {
      this->AddReweightValue(rLabel, value);
    }
  }
  
  ///<Configure the weight calculators.
  void GENIEReweight::Configure() {
    std::cout << "Configure weight calculator" << std::endl;
    
    for(unsigned int i = 0; i < fReWgtParameterName.size(); i++) {
      
      //NC Elastic parameters
      if( (fReWgtParameterName[i] == rwgt::fReweightMaNCEL) || 
	  (fReWgtParameterName[i] == rwgt::fReweightEtaNCEL) ) {
	fReweightNCEL = true;
      }
      //CC QE Axial parameters
      else if( (fReWgtParameterName[i]==rwgt::fReweightNormCCQE) ||
	       (fReWgtParameterName[i]==rwgt::fReweightNormCCQEenu) ||
	       (fReWgtParameterName[i]==rwgt::fReweightMaCCQEshape) ||
	       (fReWgtParameterName[i]==rwgt::fReweightMaCCQE) ) {
	fReweightQEMA = true;
      }
      //CC QE Vector parameters
      else if(fReWgtParameterName[i]==rwgt::fReweightVecCCQEshape) {
	fReweightQEVec = true;
      }
      //CC Resonance parameters
      else if( (fReWgtParameterName[i]==rwgt::fReweightNormCCRES) ||
	       (fReWgtParameterName[i]==rwgt::fReweightMaCCRESshape) ||
	       (fReWgtParameterName[i]==rwgt::fReweightMvCCRESshape) ||
	       (fReWgtParameterName[i]==rwgt::fReweightMaCCRES) ||
	       (fReWgtParameterName[i]==rwgt::fReweightMvCCRES) ) {
	fReweightCCRes = true;
      }
      //NC Resonance parameters
      else if( (fReWgtParameterName[i]==rwgt::fReweightNormNCRES) ||
	       (fReWgtParameterName[i]==rwgt::fReweightMaNCRESshape) ||
	       (fReWgtParameterName[i]==rwgt::fReweightMvNCRESshape) ||
	       (fReWgtParameterName[i]==rwgt::fReweightMaNCRES) ||
	       (fReWgtParameterName[i]==rwgt::fReweightMvNCRES) ) {
	fReweightNCRes = true;
      }
      //Coherant parameters
      else if( (fReWgtParameterName[i]==rwgt::fReweightMaCHOpi) || 
	       (fReWgtParameterName[i]==rwgt::fReweightR0COHpi) ) {
	fReweightCoh = true;
      }
      //Non-resonance background KNO parameters
      else if( (fReWgtParameterName[i]==rwgt::fReweightRvpCC1pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightRvpCC2pi) || 
	       (fReWgtParameterName[i]==rwgt::fReweightRvpNC1pi) || 
	       (fReWgtParameterName[i]==rwgt::fReweightRvpNC2pi) || 
	       (fReWgtParameterName[i]==rwgt::fReweightRvnCC1pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightRvnCC2pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightRvnNC1pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightRvnNC2pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightRvbarpCC1pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightRvbarpCC2pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightRvbarpNC1pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightRvbarpNC2pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightRvbarnCC1pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightRvbarnCC2pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightRvbarnNC1pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightRvbarnNC2pi) ) {
	fReweightResBkg = true;
      }
      //DIS parameters
      else if( (fReWgtParameterName[i]==rwgt::fReweightAhtBY) ||
	       (fReWgtParameterName[i]==rwgt::fReweightBhtBY) ||
	       (fReWgtParameterName[i]==rwgt::fReweightCV1uBY) || 
	       (fReWgtParameterName[i]==rwgt::fReweightCV2uBY) ||
	       (fReWgtParameterName[i]==rwgt::fReweightAhtBYshape) ||
	       (fReWgtParameterName[i]==rwgt::fReweightBhtBYshape) ||
	       (fReWgtParameterName[i]==rwgt::fReweightCV1uBYshape) ||
	       (fReWgtParameterName[i]==rwgt::fReweightCV2uBYshape) ||
	       (fReWgtParameterName[i]==rwgt::fReweightNormDISCC) ||
	       (fReWgtParameterName[i]==rwgt::fReweightRnubarnuCC) ) {
	fReweightDIS = true;
      }
      //DIS nuclear model parameters
      else if ( fReWgtParameterName[i]==rwgt::fReweightDISNuclMod ) {
	fReweightDISNucMod = true;
      }
      //NC cross section
      else if( fReWgtParameterName[i]==rwgt::fReweightNC) {
	fReweightNC = true;
      }
      //Hadronization parameters
      else if( (fReWgtParameterName[i]==rwgt::fReweightAGKY_xF1pi) || 
	       (fReWgtParameterName[i]==rwgt::fReweightAGKY_pT1pi) ) {
	fReweightAGKY = true;
      }
      //Elastic (and QE) nuclear model parameters
      else if( (fReWgtParameterName[i]==rwgt::fReweightCCQEPauliSupViaKF) || 
	       (fReWgtParameterName[i]==rwgt::fReweightCCQEMomDistroFGtoSF) ) {
	fReweightFGM = true;
      }
      //Formation Zone
      else if ( fReWgtParameterName[i]==rwgt::fReweightFormZone) {
	fReweightFZone = true;
      }
      //Intranuke Parameters
      else if( (fReWgtParameterName[i]==rwgt::fReweightMFP_pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightMFP_N) ||
	       (fReWgtParameterName[i]==rwgt::fReweightFrCEx_pi) || 
	       (fReWgtParameterName[i]==rwgt::fReweightFrElas_pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightFrInel_pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightFrAbs_pi) ||
	       (fReWgtParameterName[i]==rwgt::fReweightFrPiProd_pi) || 
	       (fReWgtParameterName[i]==rwgt::fReweightFrCEx_N) ||
	       (fReWgtParameterName[i]==rwgt::fReweightFrElas_N) ||
	       (fReWgtParameterName[i]==rwgt::fReweightFrInel_N ) ||
	       (fReWgtParameterName[i]==rwgt::fReweightFrAbs_N ) ||
	       (fReWgtParameterName[i]==rwgt::fReweightFrPiProd_N) ) {
	fReweightINuke = true;
      }
      //Resonance Decay parameters
      else if( (fReWgtParameterName[i]==rwgt::fReweightBR1gamma) ||
	       (fReWgtParameterName[i]==rwgt::fReweightBR1eta) ||
	       (fReWgtParameterName[i]==rwgt::fReweightTheta_Delta2Npi)){
	
	fReweightResDecay = true;
      }
    } //end for loop
    
    //configure the individual weight calculators
    if(fReweightNCEL) this->ConfigureNCEL();
    if(fReweightQEMA) this->ConfigureQEMA();
    if(fReweightQEVec) this->ConfigureQEVec();
    if(fReweightCCRes) this->ConfigureCCRes();
    if(fReweightNCRes) this->ConfigureNCRes();
    if(fReweightResBkg) this->ConfigureResBkg();
    if(fReweightResDecay) this->ConfgureResDecay();
    if(fReweightNC) this->ConfigureNC();
    if(fReweightDIS) this->ConfigureDIS();
    if(fReweightCoh) this->ConfigureCoh();
    if(fReweightAGKY) this->ConfigureAGKY();
    if(fReweightDISNucMod) this->ConfigureDISNucMod();
    if(fReweightFGM) this->ConfigureFZone();
    if(fReweightFZone) this->ConfigureFZone();
    if(fReweightINuke) this->ConfigureINuke();
    this->ConfigureParameters();
    
  }
  
  ///<Reconfigure the weight calculators
  void GENIEReweight::Reconfigure() {
    delete fWcalc;
    fWcalc = new genie::rew::GReWeight();
    this->Configure();
  }
  
  ///<Simple Configuration functions for configuring a single weight calculator
  
  ///<Simple Configuraiton of the NC elastic weight calculator
  void GENIEReweight::ReweightNCEL(double ma, double eta) {
    std::cout << "Configuring GENIEReweight for NC Elastic Reweighting" << std::endl;
    if(ma!=0.0) {
      this->AddReweightValue(rwgt::fReweightMaNCEL, ma);
    }
    if(eta!=0.0) {
      this->AddReweightValue(rwgt::fReweightEtaNCEL, eta);
    }
    this->Configure();
  }
  
  ///<Simple Configurtion of the CCQE axial weight calculator
  void GENIEReweight::ReweightQEMA(double ma) {
    std::cout << "Configuring GENIEReweight for QE Axial Mass Reweighting" << std::endl;
    fMaQEshape=false;
    this->AddReweightValue(rwgt::fReweightMaCCQE, ma);
    this->Configure();
  }

  ///<Simple Configuration of the CCQE vector weight calculator
  void GENIEReweight::ReweightQEVec(double mv) {
    std::cout << "Configuring GENIEReweight for QE Vector Mass Reweighting" << std::endl;
    this->AddReweightValue(rwgt::fReweightVecCCQEshape, mv);
    this->Configure();
  }
  
  ///<Simple Configuration of the CC Resonance weight calculator
  void GENIEReweight::ReweightCCRes(double ma, double mv) {
    std::cout << "Configuring GENIEReweight for CC Resonance Reweighting" << std::endl;
    fMaCCResShape=false;
    this->AddReweightValue(rwgt::fReweightMaCCRES, ma);
    if(mv!=0.0) {
      this->AddReweightValue(rwgt::fReweightMvCCRES, mv);
    }
    this->Configure();
  }
  
  ///<Simple Configurtion of the NC Resonance weight calculator
  void GENIEReweight::ReweightNCRes(double ma, double mv) {
    std::cout << "Configuring GENIEReweight for NC Resonance Reweighting" << std::endl;
    fMaNCResShape=false;
    this->AddReweightValue(rwgt::fReweightMaNCRES, ma);
    if(mv!=0.0) {
      this->AddReweightValue(rwgt::fReweightMvNCRES, mv);
    }
    this->Configure();
  }
  
  ///<Simple Configuration of the Coherant weight calculator
  void GENIEReweight::ReweightCoh(double ma, double r0) {
    std::cout << "Configuring GENIEReweight for Coherant Reweighting" << std::endl;
    this->AddReweightValue(rwgt::fReweightMaCHOpi, ma);
    this->AddReweightValue(rwgt::fReweightR0COHpi, r0);
    this->Configure();
  }
  
  ///<Simple Configuration of the Non-Resonance Background weight calculator.  Here it is being configured for v+p and vbar + n (1 pi) type interactions
  void GENIEReweight::ReweightNonResRvp1pi(double sigma) {
    std::cout << "Configuring GENIEReweight for  Non-Resonance Background Reweighting (Neutrino Single Pion)" << std::endl;
    this->AddReweightValue(rwgt::fReweightRvpCC1pi, sigma);
    this->AddReweightValue(rwgt::fReweightRvbarnCC1pi, sigma);
    this->AddReweightValue(rwgt::fReweightRvpNC1pi, sigma);
    this->AddReweightValue(rwgt::fReweightRvbarnNC1pi, sigma);
    this->Configure();
  }
  
  ///<Simple Configuration of the Non-Resonance Background weight calculator.  Here it is being configured for v+n and vbar + p (1 pi) type interactions
  void GENIEReweight::ReweightNonResRvbarp1pi(double sigma) {
    std::cout << "Configuring GENIEReweight for  Non-Resonance Background Reweighting (Anti-Neutrino Single Pion)" << std::endl;
    this->AddReweightValue(rwgt::fReweightRvnCC1pi, sigma);
    this->AddReweightValue(rwgt::fReweightRvbarpCC1pi, sigma);
    this->AddReweightValue(rwgt::fReweightRvnNC1pi, sigma);
    this->AddReweightValue(rwgt::fReweightRvbarpNC1pi, sigma);
    this->Configure();
  }
  
  ///<Simple Configuration of the Non-Resonance Background weight calculator.  Here it is being configured for v+p and vbar + n (2 pi) type interactions
  void GENIEReweight::ReweightNonResRvp2pi(double sigma) {
    std::cout << "Configuring GENIEReweight for  Non-Resonance Background Reweighting (Neutrino Two Pion)" << std::endl;
    this->AddReweightValue(rwgt::fReweightRvpCC2pi, sigma);
    this->AddReweightValue(rwgt::fReweightRvbarnCC2pi, sigma);
    this->AddReweightValue(rwgt::fReweightRvpNC2pi, sigma);
    this->AddReweightValue(rwgt::fReweightRvbarnNC2pi, sigma);
    this->Configure();
  }
  
  ///<Simple Configuration of the Non-Resonance Background weight calculator.  Here it is being configured for v+n and vbar + p (2 pi) type interactions
  void GENIEReweight::ReweightNonResRvbarp2pi(double sigma) {
    std::cout << "Configuring GENIEReweight for  Non-Resonance Background Reweighting (Anti-Neutrino Two Pion)" << std::endl;
    this->AddReweightValue(rwgt::fReweightRvnCC2pi, sigma);
    this->AddReweightValue(rwgt::fReweightRvbarpCC2pi, sigma);
    this->AddReweightValue(rwgt::fReweightRvnNC2pi, sigma);
    this->AddReweightValue(rwgt::fReweightRvbarpNC2pi, sigma);
    this->Configure();
  }
  
  ///<Simple Configuration of the Resonance decay model weight calculator
  void GENIEReweight::ReweightResDecay(double gamma, double eta, double theta) {
    std::cout << "Configuring GENIEReweight for Resoncance Decay Parameters" << std::endl;
    if(gamma!=0.0) {
      this->AddReweightValue(rwgt::fReweightBR1gamma, gamma);
    }
    if(eta!=0.0) {
      this->AddReweightValue(rwgt::fReweightBR1eta, eta);
    }
    if(theta!=0.0) {
      this->AddReweightValue(rwgt::fReweightTheta_Delta2Npi, theta);
    }
    this->Configure();
  }
  
  ///<Simple Configuration of the Total NC cross section
  void GENIEReweight::ReweightNC(double norm) {
    std::cout << "Configuring GENIEReweight for NC Cross Section Scale" << std::endl;
    this->AddReweightValue(rwgt::fReweightNC, norm);
    this->Configure();
  }

  ///<Simple Configuration of the DIS FF model weight calculator
  void GENIEReweight::ReweightDIS(double aht, double bht, double cv1u, double cv2u) {
    std::cout << "Configuring GENIEReweight for DIS Form Factor Model Reweighting" << std::endl;
    fDISshape = false;
    if(aht==0.0) {
      this->AddReweightValue(rwgt::fReweightAhtBY, aht);
    }
    if(bht==0.0) {
      this->AddReweightValue(rwgt::fReweightBhtBY, bht);
    }
    if(cv1u==0.0) {
      this->AddReweightValue(rwgt:: fReweightCV1uBY, cv1u);
    }
    if(cv2u==0.0) {
      this->AddReweightValue(rwgt::fReweightCV2uBY, cv2u);
    }
    this->Configure();
  }
  
  ///<Simple Configuration of the DIS nuclear model
  void GENIEReweight::ReweightDISnucl(bool mode) {
    std::cout << "Configuring GENIEReweight for DIS Nuclear Model" << std::endl;
    this->AddReweightValue(rwgt::fReweightDISNuclMod, mode);
    this->Configure();
  }
  
  ///<Simple Configuration of the DIS AGKY hadronization model
  void GENIEReweight::ReweightAGKY(double xF, double pT) {
    std::cout << "Configuring GENIEReweight for DIS AGKY Hadronization Model Reweighting" << std::endl;
    if(xF==0.0) {
      this->AddReweightValue(rwgt::fReweightAGKY_xF1pi, xF);
    }
    if(pT==0.0) {
      this->AddReweightValue(rwgt::fReweightAGKY_pT1pi, pT);
    }
    this->Configure();
  }
  
  ///<Simple Configuration of the Intranuke Nuclear model
  void GENIEReweight::ReweightIntraNuke(ReweightLabel_t name, double sigma) {
    std::cout << "Configuring GENIEReweight for Intranuke Model Reweighting" << std::endl;
    if ( (name==rwgt::fReweightMFP_pi) ||
	 (name==rwgt::fReweightMFP_N) ||
	 (name==rwgt::fReweightFrCEx_pi) || 
	 (name==rwgt::fReweightFrElas_pi) ||
	 (name==rwgt::fReweightFrInel_pi) ||
	 (name==rwgt::fReweightFrAbs_pi) ||
	 (name==rwgt::fReweightFrPiProd_pi) || 
	 (name==rwgt::fReweightFrCEx_N) ||
	 (name==rwgt::fReweightFrElas_N) ||
	 (name==rwgt::fReweightFrInel_N ) ||
	 (name==rwgt::fReweightFrAbs_N ) ||
	 (name==rwgt::fReweightFrPiProd_N) ) {
      this->AddReweightValue(name, sigma);
    }
    else {
      mf::LogWarning("GENIEReweight") << "That is not a valid Intranuke parameter Intranuke not configured";
    }
    this->Configure();
  }

  ///<Simple Configuration of the Formation Zone reweight calculator
  void GENIEReweight::ReweightFormZone(double sigma) {
    std::cout << "Configuring GENIEReweight for Formation Zone Reweighting" << std::endl;
    this->AddReweightValue(rwgt::fReweightFormZone, sigma);
    this->Configure();
  }
  
  ///<Simple Configuration of the Fermigas model reweight calculator
  void GENIEReweight::ReweightFGM(double kF, double sf) {
    std::cout << "Configuruing GENIEReweight for Fermi Gas Model Reweighting" << std::endl;
    this->AddReweightValue(rwgt::fReweightCCQEPauliSupViaKF, kF);
    this->AddReweightValue(rwgt::fReweightCCQEMomDistroFGtoSF, sf);
    this->Configure();
  }

  ///<End of Simple Reweight Configurations.
  
  ///<Private Member functions to configure individual weight calculators.
  ///<Configure the NCEL weight calculator
  void GENIEReweight::ConfigureNCEL() {
    std::cout << "Adding NC elastic weight calculator" << std::endl;
    fWcalc->AdoptWghtCalc( "xsec_ncel",       new GReWeightNuXSecNCEL      );
  }
  
  ///<Configure the MaQE weight calculator
  void GENIEReweight::ConfigureQEMA() {
    std::cout << "Adding CCQE axial FF weight calculator ";
    fWcalc->AdoptWghtCalc( "xsec_ccqe",       new GReWeightNuXSecCCQE      );
    if(!fMaQEshape) {
      std::cout << "in axial mass (QE) rate+shape mode" << std::endl;
      GReWeightNuXSecCCQE *rwccqe = dynamic_cast <GReWeightNuXSecCCQE*> (fWcalc->WghtCalc("xsec_ccqe"));
      rwccqe->SetMode(GReWeightNuXSecCCQE::kModeMa);
    }
    else {
      std::cout << "in axial mass (QE) shape only mode" << std::endl; 
    }
  }

  ///<Configure the QE vector FF weightcalculator
  void GENIEReweight::ConfigureQEVec() {
    std::cout << "Adding CCQE vector FF weight calculator" << std::endl;
    fWcalc->AdoptWghtCalc( "xsec_ccqe_vec",   new GReWeightNuXSecCCQEvec   );
  }

  ///<Configure the CCRES calculator
  void GENIEReweight::ConfigureCCRes() {
    std::cout << "Adding CC resonance weight calculator ";
    fWcalc->AdoptWghtCalc( "xsec_ccres",      new GReWeightNuXSecCCRES     );
    if(!fMaCCResShape) {
      std::cout << "in axial mass (Res) rate+shape mode" << std::endl;
      GReWeightNuXSecCCRES * rwccres = dynamic_cast<GReWeightNuXSecCCRES *> (fWcalc->WghtCalc("xsec_ccres")); 
      rwccres->SetMode(GReWeightNuXSecCCRES::kModeMaMv);
    }
    else {
      std::cout << "in axial mass (Res) shape only mode" << std::endl; 
    }
  }

  ///<Configure the NCRES calculator
  void GENIEReweight::ConfigureNCRes() {
    std::cout << "Adding NC resonance weight calculator ";
    fWcalc->AdoptWghtCalc( "xsec_ncres",      new GReWeightNuXSecNCRES     );
    if(!fMaNCResShape) {
      std::cout << "in axial mass (Res) rate+shape mode" << std::endl;
      GReWeightNuXSecNCRES * rwncres = dynamic_cast<GReWeightNuXSecNCRES *> (fWcalc->WghtCalc("xsec_ncres")); 
      rwncres->SetMode(GReWeightNuXSecNCRES::kModeMaMv);
    }
    else {
      std::cout << "in axial mass (Res) shape only mode" << std::endl; 
    }
  }

  ///<Configure the ResBkg (kno) weight calculator
  void GENIEReweight::ConfigureResBkg() {
    std::cout << "Adding low Q^2 DIS (KNO) weight calculator" << std::endl;
    fWcalc->AdoptWghtCalc( "xsec_nonresbkg",  new GReWeightNonResonanceBkg );
  }

  ///<Configure the ResDecay weight calculator
  void GENIEReweight::ConfgureResDecay() {
    std::cout << "Adding resonance decay weight calculator" << std::endl;
    fWcalc->AdoptWghtCalc( "hadro_res_decay", new GReWeightResonanceDecay  );
  }

  ///<Configure the NC weight calculator
  void GENIEReweight::ConfigureNC() {
    std::cout << "Adding NC total cross section weight calculator" << std::endl;
    fWcalc->AdoptWghtCalc( "xsec_nc", new GReWeightNuXSecNC );
  }

  ///<Configure the DIS (Bodek-Yang) weight calculator
  void GENIEReweight::ConfigureDIS() {
    std::cout << "Adding DIS (Bodek-Yang) weight calculator ";
    fWcalc->AdoptWghtCalc( "xsec_dis",        new GReWeightNuXSecDIS       );
    if(!fDISshape) {
      std::cout << "in shape+rate mode" << std::endl;
      GReWeightNuXSecDIS * rwdis = dynamic_cast<GReWeightNuXSecDIS *> (fWcalc->WghtCalc("xsec_dis"));
      rwdis->SetMode(GReWeightNuXSecDIS::kModeABCV12u);
    }
    else {
      std::cout << "in shape only mode" << std::endl;
    }
  }

  ///<Configure the Coherant model weight calculator
  void GENIEReweight::ConfigureCoh() {
    std::cout << "Adding coherant interaction model weight calculator" << std::endl;
    fWcalc->AdoptWghtCalc( "xsec_coh",        new GReWeightNuXSecCOH       );
  }

  ///<Configure the hadronization (AGKY) weight calculator
  void GENIEReweight::ConfigureAGKY() {
    std::cout << "Adding hadronization (AGKY) model weight calculator" << std::endl;
    fWcalc->AdoptWghtCalc( "hadro_agky",      new GReWeightAGKY            );
  }

  ///<Configure the DIS nuclear model weight calculator
  void GENIEReweight::ConfigureDISNucMod() {
    std::cout << "Adding DIS nuclear model weight calculator" << std::endl;
    fWcalc->AdoptWghtCalc( "nuclear_dis",     new GReWeightDISNuclMod      );
  }

  ///<Configure the FG model weight calculator
  void GENIEReweight::ConfigureFGM() {
    std::cout << "Adding Fermi Gas Model (FGM) weight calculator" << std::endl;
    fWcalc->AdoptWghtCalc( "nuclear_qe",      new GReWeightFGM             );
  }

  ///<Configure the Formation Zone weight calculator
  void GENIEReweight::ConfigureFZone() {
    std::cout << "Adding Formation Zone weight calculator" << std::endl;
    fWcalc->AdoptWghtCalc( "hadro_fzone",     new GReWeightFZone           );
  }

  ///<Configure the intranuke weight calculator
  void GENIEReweight::ConfigureINuke() {
    std::cout << "Adding the Intra-Nuke weight calculator" << std::endl;
    fWcalc->AdoptWghtCalc( "hadro_intranuke", new GReWeightINuke           );
  }

  ///<configure the weight parameters being used
  void GENIEReweight::ConfigureParameters() {
    GSystSet & syst = fWcalc->Systematics();
    for(unsigned int i = 0; i < fReWgtParameterName.size(); i++) {
      std::cout << "Configuring GENIEReweight parameter: " << fReWgtParameterName[i] << " with value: " << fReWgtParameterValue[i] << std::endl;
      if(fUseSigmaDef) {
	syst.Set( (GSyst_t)fReWgtParameterName[i], fReWgtParameterValue[i]);
      }
      else {
	double parameter = this->CalculateSigma((ReweightLabel_t)fReWgtParameterName[i], fReWgtParameterValue[i]);
	syst.Set( (GSyst_t)fReWgtParameterName[i], parameter);
      }
    }
    fWcalc->Reconfigure();
  }

  ///Used in parameter value mode (instead of parameter sigma mode) Given a user passed parameter value calculate the corresponding sigma value 
  ///that needs to be passed to genie to give the same weight.
  double GENIEReweight::CalculateSigma(ReweightLabel_t label, double value) {
  //double GENIEReweight::CalculateSigma(int label, double value) {
    int iLabel = (int) label;
    double nominal_def;
    double frac_err;
    double sigma;
    int sign;
    GSystUncertainty * gsysterr = GSystUncertainty::Instance();
    if(label==rwgt::fReweightCCQEMomDistroFGtoSF || 
       label==rwgt::fReweightTheta_Delta2Npi || 
       label==rwgt::fReweightDISNuclMod) {
      //These parameters don't use the sigma definition just pass them through the function unchanged
      sigma = value;
    }
    else {
      nominal_def = this->NominalParameterValue(label);
      sign = genie::utils::rew::Sign(value-nominal_def);
      frac_err = gsysterr->OneSigmaErr( (GSyst_t)iLabel, sign);
      sigma = (value - nominal_def)/(frac_err*nominal_def);
    }
    return sigma;
  }

  ///<Calculate the weights
  double GENIEReweight::CalcWeight(simb::MCTruth truth, simb::GTruth gtruth) {
    genie::EventRecord evr = this->RetrieveGHEP(truth, gtruth);
    double wgt = fWcalc->CalcWeight(evr);
    std::cout << "New Event Weight is: " << wgt << std::endl;
    return wgt;
  }

  ///< Recreate the a genie::EventRecord from the MCTruth and GTruth objects.
  genie::EventRecord GENIEReweight::RetrieveGHEP(simb::MCTruth truth, simb::GTruth gtruth) {
    
    genie::EventRecord newEvent;
    newEvent.SetWeight(gtruth.fweight);
    newEvent.SetProbability(gtruth.fprobability);
    newEvent.SetXSec(gtruth.fXsec);
    newEvent.SetDiffXSec(gtruth.fDiffXsec);
    TLorentzVector vtx = gtruth.fVertex;
    newEvent.SetVertex(vtx);

    for(int i = 0; i < truth.NParticles(); i++) {
      simb::MCParticle mcpart = truth.GetParticle(i);
      
      int gmid = mcpart.PdgCode();
      genie::GHepStatus_t gmst = (genie::GHepStatus_t)mcpart.StatusCode();
      int gmmo = mcpart.Mother();
      int ndaughters = mcpart.NumberDaughters();
      //find the track ID of the first and last daughter particles
      int fdtrkid = 0;
      int ldtrkid = 0;
      if(ndaughters !=0) {
	fdtrkid = mcpart.Daughter(0);
	if(ndaughters == 1) {
	  ldtrkid = 1;
	}
	else if(ndaughters >1) {
	  fdtrkid = mcpart.Daughter(ndaughters-1);
	}
      }      
      int gmfd = -1;
      int gmld = -1;
      //Genie uses the index in the particle array to reference the daughter particles.
      //MCTruth keeps the particles in the same order so use the track ID to find the proper index.
      for(int j = 0; j < truth.NParticles(); j++) {
	simb::MCParticle temp = truth.GetParticle(i);
	if(temp.TrackId() == fdtrkid) {
	  gmfd = j;
	}
	if(temp.TrackId() == ldtrkid) {
	  gmld = j;
	}
      }
    
      double gmpx = mcpart.Px(0);
      double gmpy = mcpart.Py(0);
      double gmpz = mcpart.Pz(0);
      double gme = mcpart.E(0);
      double gmvx = mcpart.Gvx();
      double gmvy = mcpart.Gvy();
      double gmvz = mcpart.Gvz();
      double gmvt = mcpart.Gvt();
      int gmri = mcpart.Rescatter();
      
      genie::GHepParticle gpart(gmid, gmst, gmmo, -1, gmfd, gmld, gmpx, gmpy, gmpz, gme, gmvx, gmvy, gmvz, gmvt);
      gpart.SetRescatterCode(gmri);
      TVector3 polz = mcpart.Polarization();
      if(polz.x() !=0 || polz.y() !=0 || polz.z() !=0) {
	gpart.SetPolarization(polz);
      }
      newEvent.AddParticle(gpart);

    }

    genie::ProcessInfo proc_info;
    genie::ScatteringType_t gscty = (genie::ScatteringType_t)gtruth.fGscatter;
    genie::InteractionType_t ginty = (genie::InteractionType_t)gtruth.fGint;

    proc_info.Set(gscty,ginty);

    genie::XclsTag gxt;
    
    //Set Exclusive Final State particle numbers
    genie::Resonance_t gres = (genie::Resonance_t)gtruth.fResNum;
    gxt.SetResonance(gres);
    gxt.SetNPions(gtruth.fNumPiPlus, gtruth.fNumPi0, gtruth.fNumPiMinus);
    gxt.SetNNucleons(gtruth.fNumProton, gtruth.fNumNeutron);
    
    if(gtruth.fIsCharm) {
      gxt.SetCharm(0);
    }
     else {
       gxt.UnsetCharm();
     }
    
    //Set the GENIE kinematic info
    genie::Kinematics gkin;
    gkin.Setx(gtruth.fgX, true);
    gkin.Sety(gtruth.fgY, true);
    gkin.Sett(gtruth.fgT, true);
    gkin.SetW(gtruth.fgW, true);
    gkin.SetQ2(gtruth.fgQ2, true);
    gkin.Setq2(gtruth.fgq2, true);
    simb::MCNeutrino nu = truth.GetNeutrino();
    simb::MCParticle lep = nu.Lepton();
    gkin.SetFSLeptonP4(lep.Px(), lep.Py(), lep.Pz(), lep.E());
    gkin.SetHadSystP4(gtruth.fFShadSystP4.Px(), gtruth.fFShadSystP4.Py(), gtruth.fFShadSystP4.Pz(), gtruth.fFShadSystP4.E());
    
    //Set the GENIE final state interaction info
    genie::Interaction * p_gint = new genie::Interaction;
    genie::InitialState * p_ginstate = p_gint->InitStatePtr();
    //int Z = gtruth.ftgtZ;
    //int A = gtruth.ftgtA;
    int targetNucleon = nu.HitNuc();
    int struckQuark = nu.HitQuark(); 
    int incoming = gtruth.fProbePDG;
    p_ginstate->SetProbePdg(incoming);
    
    genie::Target* target123 = p_ginstate->TgtPtr();
    target123->SetId(gtruth.ftgtPDG);
    //target123->SetId(Z,A);
    
    //int pdg_code = pdg::IonPdgCode(A, Z);
    //TParticlePDG * p = PDGLibrary::Instance()->Find(pdg_code);
    
    //std::cerr << "Setting Target Z and A" << std::endl;
    //std::cerr << "Saved PDG: " << gtruth.ftgtPDG << std::endl;
    //std::cerr << "Target PDG: " << target123->Pdg() << std::endl;
    target123->SetHitNucPdg(targetNucleon);
    target123->SetHitQrkPdg(struckQuark);
    target123->SetHitSeaQrk(gtruth.fIsSeaQuark);
    
    if(newEvent.HitNucleonPosition()> 0) {
      genie::GHepParticle * hitnucleon = newEvent.HitNucleon();
      std::auto_ptr<TLorentzVector> p4hitnucleon(hitnucleon->GetP4());
      target123->SetHitNucP4(*p4hitnucleon);
    }  
    else {
      TLorentzVector dummy(0.,0.,0.,0.);
      target123->SetHitNucP4(dummy);
    }
   
    genie::GHepParticle * probe = newEvent.Probe();
    std::auto_ptr<TLorentzVector> p4probe(probe->GetP4());
    p_ginstate->SetProbeP4(*p4probe);
    if(newEvent.TargetNucleusPosition()> 0) {
      genie::GHepParticle * target = newEvent.TargetNucleus();
      std::auto_ptr<TLorentzVector> p4target(target->GetP4());
      p_ginstate->SetTgtP4(*p4target);
    }  else {
      TLorentzVector dummy(0.,0.,0.,0.);
      p_ginstate->SetTgtP4(dummy);
    }
    p_gint->SetProcInfo(proc_info);
    p_gint->SetKine(gkin);
    p_gint->SetExclTag(gxt);
    newEvent.AttachSummary(p_gint);

    return newEvent;
 
  }


  
}
