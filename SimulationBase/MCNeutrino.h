////////////////////////////////////////////////////////////////////////
/// \file  MCNeutrino.cxx
/// \brief Simple MC truth class, holds a vector of TParticles
///
/// \version $Id: MCNeutrino.h,v 1.2 2011-03-29 19:31:22 brebel Exp $
/// \author  jpaley@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef SIMB_MCNEUTRINO_H
#define SIMB_MCNEUTRINO_H

#include "SimulationBase/MCParticle.h"

namespace simb {

  /// Neutrino interaction categories
  enum curr_type_{
    kCC,
    kNC
  };

  /// Neutrino interaction categories
  enum int_type_{
    kQE                       = 0,
    kRes                      = 1,
    kDIS		      = 2,
    kCoh		      = 3,
    kNuanceOffset             = 1000,                ///< offset to account for adding in Nuance codes to this enum
    kCCQE                     = kNuanceOffset + 1,   ///< charged current quasi-elastic	      	
    kNCQE                     = kNuanceOffset + 2,   ///< neutral current quasi-elastic	      	
    kCCDIS                    = kNuanceOffset + 91,  ///< charged current deep inelastic scatter
    kNCDIS                    = kNuanceOffset + 92,  ///< charged current deep inelastic scatter
    kCCCOH                    = kNuanceOffset + 97,  ///< charged current coherent pion         
    kNuElectronElastic        = kNuanceOffset + 98,  ///< neutrino electron elastic scatter	
    kInverseMuDecay           = kNuanceOffset + 99,  ///< inverse muon decay			  
    kResCCNuProtonPiPlus      = kNuanceOffset + 3,   ///< resonant charged current, nu p -> l- p pi+
    kResCCNuNeutronPi0        = kNuanceOffset + 4,   ///< resonant charged current, nu n -> l- p pi0
    kResCCNuNeutronPiPlus     = kNuanceOffset + 5,   ///< resonant charged current, nu n -> l- p pi+
    kResNCNuProtonPi0         = kNuanceOffset + 6,   ///< resonant neutral current, nu p -> nu p pi0
    kResNCNuProtonPiPlus      = kNuanceOffset + 7,   ///< resonant neutral current, nu p -> nu n pi+
    kResNCNuNeutronPi0        = kNuanceOffset + 8,   ///< resonant neutral current, nu n -> nu n pi0
    kResNCNuNeutronPiMinus    = kNuanceOffset + 9,   ///< resonant neutral current, nu n -> nu p pi-
    kResCCNuBarNeutronPiMinus = kNuanceOffset + 10,  ///< resonant charged current, nubar n -> l+ n pi-
    kResCCNuBarProtonPi0      = kNuanceOffset + 11,  ///< resonant charged current, nubar p -> l+ n pi0
    kResCCNuBarProtonPiMinus  = kNuanceOffset + 12,  ///< resonant charged current, nubar p -> l+ p pi-
    kResNCNuBarProtonPi0      = kNuanceOffset + 13,  ///< resonant charged current, nubar p -> nubar p pi0
    kResNCNuBarProtonPiPlus   = kNuanceOffset + 14,  ///< resonant charged current, nubar p -> nubar n pi+
    kResNCNuBarNeutronPi0     = kNuanceOffset + 15,  ///< resonant charged current, nubar n -> nubar n pi0
    kResNCNuBarNeutronPiMinus = kNuanceOffset + 16,  ///< resonant charged current, nubar n -> nubar p pi-  
  };
  
  //......................................................................

  /// Event generator information
  class MCNeutrino {
  public:

    MCNeutrino();
    MCNeutrino(MCParticle &nu, MCParticle &lep, 
	       int CCNC, int mode, int interactionType,
	       int target, int nucleon,
	       int quark, double w, double x, double y, double qsqr);
    ~MCNeutrino();

  public:

    const  MCParticle& Nu()              const { return fNu;              }
    const  MCParticle& Lepton()   	 const { return fLepton;  	  }
    int                CCNC()     	 const { return fCCNC;    	  }
    int                Mode()     	 const { return fMode;    	  }
    int                InteractionType() const { return fInteractionType; }
    int                Target()          const { return fTarget;          }
    int                HitNuc()   	 const { return fHitNuc;  	  }
    int                HitQuark() 	 const { return fHitQuark;	  }
    double             W()        	 const { return fW;       	  }
    double             X()        	 const { return fX;       	  }
    double             Y()        	 const { return fY;       	  }
    double             QSqr()     	 const { return fQSqr;    	  }
    double             Pt()              const; ///< transverse momentum of interaction, in GeV/c
    double             Theta()           const; ///< angle between incoming and outgoing leptons, in radians
    friend std::ostream&  operator<< (std::ostream& output, const simb::MCNeutrino &mcnu);

  private:
 
    MCParticle fNu;              ///< the incoming neutrino			      
    MCParticle fLepton;   	 ///< the outgoing lepton			      
    int        fMode;     	 ///< Interaction mode (QE/1-pi/DIS...) see enum list
    int        fInteractionType; ///< More detailed interaction type, see enum list below kNuanceOffset
    int        fCCNC;            ///< CC or NC interaction? see enum list	      
    int        fTarget;   	 ///< Nuclear target, as PDG code				      
    int        fHitNuc;   	 ///< Hit nucleon (2212 (proton) or 2112 (neutron))  
    int        fHitQuark; 	 ///< For DIS events only, as PDG code			      
    double     fW;        	 ///< Hadronic invariant mass, in GeV			      
    double     fX;        	 ///< Bjorken x=Q^2/(2M*(E_neutrino-E_lepton)), unitless	      
    double     fY;        	 ///< Inelasticity y=1-(E_lepton/E_neutrino), unitless	      
    double     fQSqr;     	 ///< Momentum transfer Q^2, in GeV^2                          

  };
}

#endif //SIMB_MCNEUTRINO_H
////////////////////////////////////////////////////////////////////////
