////////////////////////////////////////////////////////////////////////
/// \file  MCTruth.cxx
/// \brief Simple MC truth class, holds a vector of TParticles
///
/// \version $Id: MCTruth.cxx,v 1.7 2012-09-24 15:20:02 brebel Exp $
/// \author  jpaley@indiana.edu
////////////////////////////////////////////////////////////////////////
#include "SimulationBase/MCTruth.h"
#include "SimulationBase/MCParticle.h"
#include "SimulationBase/MCNeutrino.h"

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
  void MCTruth::SetNeutrino(int CCNC, 
			    int mode, 
			    int interactionType,
			    int target, 
			    int nucleon,
			    int quark, 
			    double w, 
			    double x, 
			    double y, 
			    double qsqr)
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
				     CCNC, mode, interactionType,
				     target, nucleon, quark, 
				     w, x, y, qsqr);
    }
    else
      std::cerr << "MCTruth - attempt to set neutrino when already set" << std::endl;

    return;
  }

  //......................................................................
  std::ostream& operator<< (std::ostream& o, simb::MCTruth const& a)
  {
    if(a.Origin() == kCosmicRay) 
      o << "This is a cosmic ray event" << std::endl;
    else if(a.Origin() == kBeamNeutrino){
      o << "This is a beam neutrino event" << std::endl;
      o << a.GetNeutrino();
    }
    else if(a.Origin() == kSuperNovaNeutrino){ 
      o << "This is a supernova neutrino event" << std::endl;
      o << a.GetNeutrino();
    }  

    for (int i = 0; i < a.NParticles(); ++i)
      o << i << " " << a.GetParticle(i) << std::endl;

    return o;
  }
}
////////////////////////////////////////////////////////////////////////
