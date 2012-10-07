////////////////////////////////////////////////////////////////////////
/// \file  DetectorConstruction.cxx
/// \brief Build Geant4 geometry from GDML
///
/// \version $Id: DetectorConstruction.cxx,v 1.9 2012-10-07 05:05:32 rhatcher Exp $
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////

#include "G4Base/DetectorConstruction.h"
#include "MagneticField/mag.h"

#include "Geant4/G4VPhysicalVolume.hh"
#include "Geant4/G4GDMLParser.hh"
#include "Geant4/G4LogicalVolumeStore.hh"
#include "Geant4/G4Material.hh"
#include "Geant4/G4UniformMagField.hh"
#include "Geant4/G4FieldManager.hh"

#include "cetlib/exception.h"

namespace g4b{

  // Allocate static variables.
  G4VPhysicalVolume* DetectorConstruction::fWorld    = 0;
  G4FieldManager*    DetectorConstruction::fFieldMgr = 0;

  //---------------------------------------------------
  // Constructor
  DetectorConstruction::DetectorConstruction(std::string const& gdmlFile) 
  {
    if ( gdmlFile.empty() ) {
      throw cet::exception("DetectorConstruction") 
        << "Supplied GDML filename is empty\n"
        << __FILE__ << ":" << __LINE__ << "\n";
    }
    // Get the path to the GDML file from the Geometry interface.
    const G4String GDMLfile = static_cast<const G4String>( gdmlFile );

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
      /// \todo This isn't the Right (tm) thing to do, but it will do
      /// \todo for now.
      /// \todo Get pointer to the logical volume store 
      G4LogicalVolumeStore *lvs  = G4LogicalVolumeStore::GetInstance();
      G4LogicalVolume      *bvol = lvs->GetVolume(bField->MagnetizedVolume());

      // the boolean tells the field manager to use local volume
      bvol->SetFieldManager(fFieldMgr,true);
    }

    return fWorld;
  }

}// namespace
