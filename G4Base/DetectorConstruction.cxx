////////////////////////////////////////////////////////////////////////
/// \file  DetectorConstruction.cxx
/// \brief Build Geant4 geometry from GDML
///
/// \version $Id: DetectorConstruction.cxx,v 1.5 2011-10-20 17:10:56 brebel Exp $
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////

#include "G4Base/DetectorConstruction.h"
#include "Geometry/geo.h"
#include "MagneticField/mag.h"

#include <G4VPhysicalVolume.hh>
#include <G4GDMLParser.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4Material.hh>
#include <G4UniformMagField.hh>
#include <G4FieldManager.hh>

namespace g4b{

  // Allocate static variables.
  G4VPhysicalVolume* DetectorConstruction::fWorld    = 0;
  G4FieldManager*    DetectorConstruction::fFieldMgr = 0;

  //---------------------------------------------------
  // Constructor
  DetectorConstruction::DetectorConstruction() 
  {
    // Get the path to the GDML file from the Geometry interface.
    art::ServiceHandle<geo::Geometry> geometry;
    const G4String GDMLfile = static_cast<const G4String>( geometry->GDMLFile() );

    // Use Geant4's GDML parser to convert the geometry to Geant4 format.
    G4GDMLParser parser;
    //parser.SetOverlapCheck(true);
    parser.Read( GDMLfile );

    // Fetch the world physical volume from the parser.  This contains
    // the entire detector, not just the outline of the experimental
    // hall.
    fWorld = parser.GetWorldVolume();
  }
  
  //---------------------------------------------------
  // Destructor.
  DetectorConstruction::~DetectorConstruction() 
  {
  }
  
  //---------------------------------------------------
  G4VPhysicalVolume* DetectorConstruction::Construct()
  {
    // Setup the magnetic field situation 
    art::ServiceHandle<mag::MagneticField> bField;
    if (bField->UseField()) {
      // Define the basic field
      G4UniformMagField* magField = new G4UniformMagField( bField->FieldAtPoint() * tesla );
      fFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
      fFieldMgr->SetDetectorField(magField);
      fFieldMgr->CreateChordFinder(magField);

      // Reset the chord finding accuracy
      // fFieldMgr->GetChordFinder()->SetDeltaChord(1.0 * cm);

      // Attach this to the magnetized volume only
      //
      // \todo This isn't the Right (tm) thing to do, but it will do
      // for now.
      // Get pointer to the logical volume store 
      G4LogicalVolumeStore *lvs  = G4LogicalVolumeStore::GetInstance();
      G4LogicalVolume      *bvol = lvs->GetVolume(bField->MagnetizedVolume());

      // the boolean tells the field manager to use local volume
      bvol->SetFieldManager(fFieldMgr,true);
    }

    return fWorld;
  }

}// namespace
