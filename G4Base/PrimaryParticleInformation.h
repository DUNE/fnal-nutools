////////////////////////////////////////////////////////////////////////
/// \file PrimaryParticleInformation.h
//
/// \version $Id: PrimaryParticleInformation.h,v 1.3 2011-10-20 17:10:56 brebel Exp $
/// \author  seligman@nevis.columbia.edu, brebel@fnal.gov
////////////////////////////////////////////////////////////////////////
/// PrimaryParticleInformation
/// 10-Sep-2007 Bill Seligman
///
/// 11-Feb-2009 <seligman@nevis.columbia.edu> Revised for LArSoft
///
/// Purpose: This class is "attached" to the G4PrimaryParticle.  It's used to
/// save the MCTruth object associated with the event.
///
/// Background: Read this carefully, because this class probably
/// doesn't do what you think it does.
///
/// Geant4 has various "truth" classes: G4Event, G4Track,
/// G4PrimaryVertex, G4PrimaryParticle, etc.  For all of these
/// classes, Geant4 provides a facility for the user to include
/// additional information that's "attached" to the class in question.
///
/// In this case, this class defines additional information to
/// included with the G4PrimaryParticle class.  In particular, it
/// stores the pointer to the simb::MCTruth object that was the
/// source of the G4PrimaryParticle information.
/// 
/// The reason why this class is necessary for the G4Base application
/// is that it allows the ParticleListAction class access to the
/// MCTruth pointer during Geant4's tracking.

#ifndef G4BASE_PrimaryParticleInformation_h
#define G4BASE_PrimaryParticleInformation_h

// G4 Includes
#include "G4VUserPrimaryParticleInformation.hh"
#include "G4Allocator.hh"

// ART Includes
#include "art/Persistency/Common/Ptr.h"
#include "art/Persistency/Common/PtrVector.h"


// Forward declaration for this namespace.
namespace simb {
  class MCTruth;
}

namespace g4b {
 
  class PrimaryParticleInformation : public G4VUserPrimaryParticleInformation {

  public:
    PrimaryParticleInformation();    
    virtual ~PrimaryParticleInformation();

    inline void* operator new(size_t);
    inline void operator delete(void*);
    
    // Accessors:
    const art::Ptr<simb::MCTruth>& GetMCTruth() const { return fMCTruth; }
    void SetMCTruth(art::Ptr<simb::MCTruth> m,
		    unsigned int            index)    { fMCTruth = m; fMCTruthIndex = index; }

    // Required by Geant4:
    void Print() const;

  private:

    // The MCTruth object associated with the G4PrimaryParticle.  If
    // this is zero, then there is no MCTruth object for this
    // particle (although in that case it's more likely that a
    // G4Base::PrimaryParticleInformation object would not have been
    // created in the first place.)
    art::Ptr<simb::MCTruth> fMCTruth;
    unsigned int            fMCTruthIndex; ///< which MCTruth object in the event is this one?
  };

  // It's not likely, but there could be memory issues with these
  // PrimaryParticleInformation objects.  To make things work more smoothly
  // and quickly, use Geant4's memory allocation mechanism.
  
  extern G4Allocator<PrimaryParticleInformation> PrimaryParticleInformationAllocator;
  
  inline void* PrimaryParticleInformation::operator new(size_t)
  {
    void *aPrimaryParticleInformation;
    aPrimaryParticleInformation = (void *) PrimaryParticleInformationAllocator.MallocSingle();
    return aPrimaryParticleInformation;
  }
  
  inline void PrimaryParticleInformation::operator delete(void *aPrimaryParticleInformation)
  {
    PrimaryParticleInformationAllocator.FreeSingle((PrimaryParticleInformation*) aPrimaryParticleInformation);
  }

}

#endif // G4BASE_PrimaryParticleInformation_h
