////////////////////////////////////////////////////////////////////////
/// \file  MCTruth.cxx
/// \brief Simple MC truth class, holds a vector of TParticles
///
/// \version $Id: MCTruth.cxx,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  jpaley@indiana.edu
////////////////////////////////////////////////////////////////////////
#include "SimulationBase/simbase.h"

#include "TDatabasePDG.h"

#include <iostream>

namespace simb{

  //......................................................................
  MCTruth::MCTruth() :
    fOrigin(simb::kUnknown),
    fNeutrinoSet(false)
  { 
  }

  //......................................................................
  MCTruth::~MCTruth()
  {
  }

  //......................................................................
  void MCTruth::SetNeutrino(int CCNC, int mode, int target, int nucleon,
			    int quark, double w, double x, double y, double qsqr)
  {
    if( !fNeutrinoSet ){
      fNeutrinoSet = true;
      ///loop over the MCParticle list and get the outgoing lepton
      MCParticle nu  = fPartList[0];
      MCParticle lep = fPartList[1];
      ///start at i = 1 because i = 0 is the incoming neutrino
      for(unsigned int i = 1; i < fPartList.size(); ++i){
	if(CCNC == simb::kNC && fPartList[i].PdgCode() == nu.PdgCode() ){
	  lep = fPartList[i];
	  break;
	}
	else if(CCNC == simb::kCC 
		&& abs(fPartList[i].PdgCode()) == abs(nu.PdgCode())-1){
	  lep = fPartList[i];
	  break;
	}
      }//done looping over particles
    
      fMCNeutrino = simb::MCNeutrino(nu, lep, 
				     CCNC, mode, 
				     target, nucleon, quark, 
				     w, x, y, qsqr);
    }
    else
      std::cerr << "MCTruth - attempt to set neutrino when already set" << std::endl;

    return;
  }

  //......................................................................
  void MCTruth::Print() const
  {
    if(fOrigin == kCosmicRay) 
      std::cout << "This is a cosmic ray event" << std::endl;
    else if(fOrigin == kBeamNeutrino){
      std::cout << "This is a beam neutrino event" << std::endl;
      std::cout << fMCNeutrino;
    }
    else if(fOrigin == kSuperNovaNeutrino){ 
      std::cout << "This is a supernova neutrino event" << std::endl;
      std::cout << fMCNeutrino;
    }  

    const TDatabasePDG* databasePDG = TDatabasePDG::Instance();
  
    for (unsigned int i=0; i<fPartList.size(); ++i) {
      std::cout << i << ") " 
		<< databasePDG->GetParticle(fPartList[i].PdgCode())->GetName() << "\t"
		<< fPartList[i].PdgCode() << "\t"
		<< fPartList[i].Process() << "\t"
		<< "["
		<< fPartList[i].Vx() << ","
		<< fPartList[i].Vy() << ","
		<< fPartList[i].Vz() << ","
		<< fPartList[i].T() 
		<<  "] ["
		<< fPartList[i].Px() << ","
		<< fPartList[i].Py() << ","
		<< fPartList[i].Pz() 
		<< "] \t"
		<< fPartList[i].E()
		<< std::endl;
    }
  }
}
////////////////////////////////////////////////////////////////////////
