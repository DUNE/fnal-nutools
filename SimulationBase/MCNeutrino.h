////////////////////////////////////////////////////////////////////////
/// \file  MCNeutrino.cxx
/// \brief Simple MC truth class, holds a vector of TParticles
///
/// \version $Id: MCNeutrino.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
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
    kQE,
    kRes,
    kDIS,
    kCoh
  };
  
  //......................................................................

  /// Event generator information
  class MCNeutrino {
  public:

    MCNeutrino();
    MCNeutrino(MCParticle &nu, MCParticle &lep, 
	       int CCNC, int mode, int target, int nucleon,
	       int quark, double w, double x, double y, double qsqr);
    ~MCNeutrino();

  public:

    const  MCParticle& Nu()       const { return fNu;      }
    const  MCParticle& Lepton()   const { return fLepton;  }
    int                CCNC()     const { return fCCNC;    }
    int                Mode()     const { return fMode;    }
    int                Target()   const { return fTarget;  }
    int                HitNuc()   const { return fHitNuc;  }
    int                HitQuark() const { return fHitQuark;}
    double             W()        const { return fW;       }
    double             X()        const { return fX;       }
    double             Y()        const { return fY;       }
    double             QSqr()     const { return fQSqr;    }
    double             Pt()       const; ///< transverse momentum of interaction
    double             Theta()    const; ///< angle between incoming and outgoing leptons
    friend std::ostream&  operator<< (std::ostream& output, const simb::MCNeutrino &mcnu);

  private:

    MCParticle fNu;       ///< the incoming neutrino
    MCParticle fLepton;   ///< the outgoing lepton
    int        fMode;     ///< Interaction mode (QE/1-pi/DIS...) see enum list
    int        fCCNC;     ///< CC or NC interaction? see enum list
    int        fTarget;   ///< Nuclear target
    int        fHitNuc;   ///< Hit nucleon (2212 (proton) or 2112 (neutron))
    int        fHitQuark; ///< For DIS events only
    double     fW;        ///< Hadronic invariant mass 
    double     fX;        ///< Bjorken x=Q^2/(2M*(E_neutrino-E_lepton))
    double     fY;        ///< Inelasticity y=1-(E_lepton/E_neutrino)
    double     fQSqr;     ///< Momentum transfer Q^2

    ///< units of the variables are:
    ///< fTarget, fHitNuc, fHitQuark - PDG code
    ///< fW    - GeV
    ///< fX    - unitless
    ///< fY    - unitless
    ///< fQSqr - GeV^2
    ///< Pt    - GeV/c
    ///< Theta - radians
 
  };
}

#endif //SIMB_MCNEUTRINO_H
////////////////////////////////////////////////////////////////////////
