/// \file MagField.h
/// \brief Describe the magnetic field structure of a detector
/// 
/// \version $Id: MagneticField.h,v 1.1.1.1 2011-10-20 17:21:50 brebel Exp $
/// \author dmckee@phys.ksu.edu
//////////////////////////////////////////////////////////////////////////
/// \namespace mag
/// A namespace for simulated magnetic fields
//////////////////////////////////////////////////////////////////////////
#ifndef MAG_MAGNETICFIELD_H
#define MAG_MAGNETICFIELD_H

#include <string>

// Geant4 includes
#include <G4ThreeVector.hh>

// Framework includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

namespace mag {

  // Specifies the magnetic field over all space
  //
  // The default implementation, however, uses a nearly trivial,
  // non-physical hack.
  class MagneticField {
  public:
    MagneticField(fhicl::ParameterSet const& pset, art::ActivityRegistry& reg);
    ~MagneticField(){};

    void reconfigure(fhicl::ParameterSet const& pset);

    bool UseField() const { return fUseField; }

    // return the field at a particular point
    G4ThreeVector FieldAtPoint(G4ThreeVector p=G4ThreeVector(0)) const;

    // return the outermost affected volume
    std::string MagnetizedVolume() const { return fVolume; }

  private:
    // The simplest implmentation has a constant field inside a named
    // detector volume
    bool fUseField;       ///< is a field to be used
    G4ThreeVector fField; ///< the three vector of the field
    G4String fVolume;     ///< the volume of the field

    //\todo Need to add ability to read in a field from a database
  };

}

#endif // MAG_MAGNETICFIELD_H