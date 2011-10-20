////////////////////////////////////////////////////////////////////////
/// \file  DetectorConstruction.h
/// \brief Build Geant4 geometry from GDML
///
/// \version $Id: DetectorConstruction.h,v 1.2 2011-10-20 17:10:56 brebel Exp $
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////
#ifndef G4BASE_DetectorConstruction_h
#define G4BASE_DetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"

// forward declarations
class G4VPhysicalVolume;
class G4FieldManager;

namespace g4b {

  class DetectorConstruction : public G4VUserDetectorConstruction {

  public:
    /// Standard constructor and destructor.
    DetectorConstruction();
    virtual ~DetectorConstruction();

    /// The key method in this class; returns the Geant4 version of
    /// the detector geometry.  Required of any class that inherits
    /// from G4VUserDetectorConstruction.
    G4VPhysicalVolume* Construct();

    /// Return a pointer to the world volume.  This returns the same
    /// pointer as Construct(), but Construct() also does other
    /// actions such as setting up sensitive detectors.  World() just
    /// returns the pointer to the top physical volume.
    static G4VPhysicalVolume* GetWorld() { return fWorld;    }

    /// Magnetic field
    static G4FieldManager* GetFieldMgr() { return fFieldMgr; }

  private:
    static G4VPhysicalVolume* fWorld;    ///< pointer to the world volume
    static G4FieldManager*    fFieldMgr; ///< pointer to the field manager

  };

} // namespace g4b

#endif // G4BASE_DetectorConstruction_h
