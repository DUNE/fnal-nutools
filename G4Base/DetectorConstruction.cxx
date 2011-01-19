////////////////////////////////////////////////////////////////////////
/// \file  DetectorConstruction.cxx
/// \brief Build Geant4 geometry from GDML
///
/// \version $Id: DetectorConstruction.cxx,v 1.2 2011-01-19 16:45:41 p-nusoftart Exp $
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////

#include "G4Base/DetectorConstruction.h"
#include "Geometry/geo.h"

#include <G4VPhysicalVolume.hh>
#include <G4GDMLParser.hh>
#include <G4Material.hh>

using namespace g4b;

//---------------------------------------------------
// Constructor
DetectorConstruction::DetectorConstruction() 
{
}

//---------------------------------------------------
// Destructor.
DetectorConstruction::~DetectorConstruction() 
{
}

//---------------------------------------------------
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  ///get a parser for the gdml file - the gdml file name
  ///must be set before this method is called.

  // Get the path to the GDML file from the Geometry interface.
  art::Service<geo::Geometry> geometry;
  const G4String GDMLfile = static_cast<const G4String>( geometry->GDMLFile() );

  G4GDMLParser pars;
  pars.Read(GDMLfile);
  std::cout << *(G4Material::GetMaterialTable()) << std::endl;
  return pars.GetWorldVolume();
}
