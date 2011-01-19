////////////////////////////////////////////////////////////////////////
/// \file  G4Helper.h
/// \brief Use Geant4 to run the detector simulation
///
/// \version $Id: G4Helper.h,v 1.2 2011-01-19 16:45:41 p-nusoftart Exp $
/// \author  seligman@nevis.columbia.edu, brebel@fnal.gov
////////////////////////////////////////////////////////////////////////

/// This object has the following functions:
///
/// - Initialize Geant4 physics, detector geometry, and other
///   processing.
///
/// - Pass the primary particles to the Geant4 simulation to calculate
///   "truth" information for the detector response.
///

#ifndef G4BASE_G4HELPER_H
#define G4BASE_G4HELPER_H

// NOvA includes
#include "G4Base/ConvertMCTruthToG4.h"

#include <cstring>

// ART Includes
#include "art/Persistency/Common/Ptr.h"
#include "art/Persistency/Common/PtrVector.h"

// Forward declarations
class G4RunManager;
class G4UImanager;

namespace simb{ class MCTruth;      }
namespace sim { class ParticleList; }

namespace g4b {  

  // Forward declarations within namespace.
  class ParticleListAction;
  class ConvertPrimaryToGeant4;

  class G4Helper {

  public:

    /// Standard constructor and destructor for an FMWK module.
    G4Helper();
    G4Helper(std::string g4macropath);
    virtual ~G4Helper();

    /// Initialization for the Geant4 Monte Carlo, called before the
    /// first event is simulated.
    void InitMC();

    /// This is the method that actually passes the MCTruth objects to 
    /// G4 and gets back a list of particles
    bool G4Run(art::PtrVector<simb::MCTruth> &primaries);

  protected:
    // These variables are "protected" rather than private, because I
    // can forsee that it may be desirable to derive other simulation
    // routines from this one.


    std::string          fG4MacroPath;    ///> Full directory path for Geant4 macro file to be executed before main MC processing.

    G4RunManager*        fRunManager;     ///> Geant4's run manager.
    G4UImanager*         fUIManager;      ///> Geant4's user-interface manager.

    ConvertMCTruthToG4*  fConvertMCTruth; ///> Converts MCTruth objects; Geant4 event generator.
  };

} // namespace g4b

#endif // G4BASE_G4HELPER_H
