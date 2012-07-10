////////////////////////////////////////////////////////////////////////
/// \file  GTruth.h
/// \Class to hold the additional information needed to recreate a genie::EventRecord
/// \author  nathan.mayer@tufts.edu
////////////////////////////////////////////////////////////////////////

/// This class stores/retrieves the additional information needed (and not in MCTruth) to recreate a genie::EventRecord
/// for genie based event reweighting.

#include "SimulationBase/simbase.h"

#include <TLorentzVector.h>

#include "art/Persistency/Common/Ptr.h"

#include <vector>
#include <iostream>
#include <string>



namespace simb {

  GTruth::GTruth() {
    std::cout << "Creating GTruth" << std::endl;
    fGint = -1;
    fGscatter = -1;
    fResNum = -1;
    fProbePDG = -1;
    
    fNumPiPlus = -1;
    fNumPiMinus = -1;
    fNumPi0 = -1;
    fNumProton = -1;
    fNumNeutron = -1;
    
    fIsCharm = false;
    fIsSeaQuark = false;
    
    fweight = 0;
    fprobability = 0;
    fXsec = 0;
    fDiffXsec = 0;
    fgQ2 = 0;
    fgq2 = 0;
    fgW = 0;
    fgT = 0;
    fgX = 0;
    fgY = 0;

    fFShadSystP4.SetXYZT(0, 0, 0, 0);
    fHitNucP4.SetXYZT(0, 0, 0, 0);
    fProbeP4.SetXYZT(0, 0, 0, 0);
    fVertex.SetXYZT(0, 0, 0, 0);

    ftgtZ = 0;
    ftgtA = 0;
    
  }
  
  GTruth::~GTruth() {

  }

} // namespace simb
