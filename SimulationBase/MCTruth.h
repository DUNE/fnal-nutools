////////////////////////////////////////////////////////////////////////
/// \file  MCTruth.cxx
/// \brief Simple MC truth class, holds a vector of TParticles
///
/// \version $Id: MCTruth.h,v 1.4 2011-11-13 16:10:55 brebel Exp $
/// \author  jpaley@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef SIMB_MCTRUTH_H
#define SIMB_MCTRUTH_H

#include <vector>
#include "TParticle.h"
#include "SimulationBase/MCNeutrino.h"

namespace simb {

  class MCParticle;

  /// event origin types
  typedef enum _ev_origin{
    kUnknown,           ///< ???
    kBeamNeutrino,      ///< Beam neutrinos
    kCosmicRay,         ///< Cosmic rays
    kSuperNovaNeutrino, ///< Supernova neutrinos
    kSingleParticle     ///< single particles thrown at the detector
  } Origin_t;

  //......................................................................

  /// Event generator information
  class MCTruth {
  public:
    MCTruth();
    ~MCTruth();

  public:

    simb::Origin_t          Origin()            const { return fOrigin;               }
    int                     NParticles()        const { return (int)fPartList.size(); }
    const MCParticle&       GetParticle(int i)  const { return fPartList[i];          }
    const simb::MCNeutrino& GetNeutrino()       const { return fMCNeutrino;           }
    
    void             Add(MCParticle& part)            { fPartList.push_back(part);    }
    void             SetOrigin(simb::Origin_t origin) { fOrigin = origin;             }
    void             SetNeutrino(int CCNC, 
				 int mode, 
				 int interactionType,
				 int target, 
				 int nucleon,
				 int quark, 
				 double w, 
				 double x, 
				 double y, 
				 double qsqr);                      
    bool             NeutrinoSet()                      const { return fNeutrinoSet; }
 
    friend std::ostream&  operator<< (std::ostream& o, simb::MCTruth const& a);

  private:

    std::vector<MCParticle> fPartList;   ///< list of particles in this event
    simb::MCNeutrino        fMCNeutrino; ///< reference to neutrino info - null if not a neutrino
    simb::Origin_t          fOrigin;     ///< origin for this event
    bool                    fNeutrinoSet;///< flag for whether the neutrino information has been set

  };
}

#endif //SIMB_MCTRUTH_H
////////////////////////////////////////////////////////////////////////
