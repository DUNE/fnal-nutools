////////////////////////////////////////////////////////////////////////
/// \file  DetectorConstruction.h
/// \brief Build Geant4 geometry from GDML
///
/// \version $Id: DetectorConstruction.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////
#ifndef G4BASE_DetectorConstruction_h
#define G4BASE_DetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"

// forward declarations
class G4VPhysicalVolume;

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

  private:

  };

} // namespace g4b

#endif // G4BASE_DetectorConstruction_h
