////////////////////////////////////////////////////////////////////////
/// \file  G4Helper.h
/// \brief Use Geant4 to run the detector simulation
///
/// \version $Id: G4Helper.h,v 1.9 2012-05-09 18:26:22 brebel Exp $
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

// nutools includes
#include "G4Base/ConvertMCTruthToG4.h"

#include <cstring>

// ART Includes
#include "art/Persistency/Common/Ptr.h"
#include "art/Persistency/Common/PtrVector.h"

#include <G4RunManager.hh>
#include <G4VUserParallelWorld.hh>

// Forward declarations
class G4UImanager;

namespace simb{ class MCTruth;      }
namespace sim { class ParticleList; }

///basic interface to Geant4 for ART-based software
namespace g4b {  

  // Forward declarations within namespace.
  class ParticleListAction;
  class ConvertPrimaryToGeant4;
  class DetectorConstruction;

  class G4Helper {

  public:

    /// Standard constructor and destructor for an FMWK module.
    G4Helper();
    G4Helper(std::string g4macropath, std::string g4physicslist = "QGSP_BERT");
    virtual ~G4Helper();

    // have to call this before InitMC if you want to load in 
    // parallel worlds.  G4Helper takes over ownership
    void SetParallelWorlds(std::vector<G4VUserParallelWorld*> pworlds);

    // have to call this before InitMC if you want to control
    // when the detector is constructed, useful if you need to 
    // muck with G4LogicalVolumes
    // if the fDetector pointer is null when InitMC is called
    // it will just construct the fDetector
    void ConstructDetector();

    // Initialization for the Geant4 Monte Carlo, called before the
    // first event is simulated.
    void InitMC();

    // This is the method that actually passes the MCTruth objects to 
    // G4 and gets back a list of particles
    bool G4Run(art::PtrVector<simb::MCTruth> &primaries);

    // Pass a single MCTruth object to G4
    bool G4Run(art::Ptr<simb::MCTruth>& primary);

    G4RunManager* GetRunManager() { return fRunManager; }

  protected:

    void SetPhysicsList(std::string physicsList);

    // These variables are "protected" rather than private, because I
    // can forsee that it may be desirable to derive other simulation
    // routines from this one.
    std::string                        fG4MacroPath;    ///< Full directory path for Geant4 macro file 	 
                          	                        ///< to be executed before main MC processing.	 
    std::string           	       fG4PhysListName; ///< Name of physics list to use			 
				                                                                              
    G4RunManager*         	       fRunManager;     ///< Geant4's run manager.				 
    G4UImanager*          	       fUIManager;      ///< Geant4's user-interface manager.		 
				                                                                              
    ConvertMCTruthToG4*   	       fConvertMCTruth; ///< Converts MCTruth objects; Geant4 event generator.
    DetectorConstruction* 	       fDetector;       ///< DetectorConstruction object                      
    std::vector<G4VUserParallelWorld*> fParallelWorlds; ///< list of parallel worlds
  };

} // namespace g4b

#endif // G4BASE_G4HELPER_H
