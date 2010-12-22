////////////////////////////////////////////////////////////////////////
/// \file  ConvertMCTruthToG4.h
/// \brief Convert MCTruth to G4Event; Geant4 event generator
///
/// \version $Id: ConvertMCTruthToG4.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////
///
/// Every Geant4 simulation has to have some event generator object,
/// even if it's the default ParticleGun.  This class is the event
/// generator for G4Base.  It accepts a set of simb::MCTruth objects
/// associated with one event, converts them to Geant4's G4Event
/// format, and makes that information available to the G4 simulation
/// when it requests it.

#ifndef G4BASE_CONVERTMCTRUTHTOG4_H
#define G4BASE_CONVERTMCTRUTHTOG4_H

// G4 Includes
#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleTable.hh>
#include <globals.hh>

// ART Includes
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/PtrVector.h"

// Forward declarations
class G4Event;
namespace simb { 
  class MCTruth; 
}

namespace g4b {

  class ConvertMCTruthToG4 : public G4VUserPrimaryGeneratorAction {

  public:
    /// Standard constructor and destructor.
    ConvertMCTruthToG4();
    virtual ~ConvertMCTruthToG4();

    /// Get ready to convert a new set of MCTruth objects.
    void Reset();

    /// Add a new MCTruth object to the list of primary particles to
    /// be appended to the Geant4 event.
    void Append( edm::Ptr<simb::MCTruth> &mct );

    /// Required of any class that inherits from
    /// G4VUserPrimaryGeneratorAction; append primary particles to a
    /// G4Event object.  This method is invoked by Geant4, and is not
    /// directly called by the user application.
    virtual void GeneratePrimaries( G4Event* );

  private:
    static G4ParticleTable*           fParticleTable; ///> Geant4's table of particle definitions.
    edm::PtrVector<simb::MCTruth>     fConvertList;   ///> List of MCTruth objects to convert for this spill
    std::map<G4int, G4int>            fUnknownPDG;    ///> map of unknown PDG codes to instances
  };

} // namespace g4b

#endif // G4BASE_CONVERTMCTRUTHTOG4_H
