/// LArG4::UserActionManager.h
/// 18-Sep-2007 Bill Seligman
///
/// 27-Jan-2009 <seligman@nevis.columbia.edu> Revised for LArSoft.
///
/// In my experience, people barely understand what the UserAction
/// interface does.  So why do we need a UserActionManager class?
///
/// Suppose I've written a class that inherits from UserAction that
/// makes histograms.  You've written a class that inherits from
/// UserAction to write events to a disk file.  Jane has written an
/// UserAction that makes ntuples.  A big massive 500-CPU-hour run of
/// G4 is planned, and we're all planning how to put our user-action
/// classes together.
///
/// By using a UserActionManager class, each one of us can develop our
/// own user-action classes independently.  Then, when we have the big
/// run, the user-action classes can be put successively called by the
/// UserActionManager without changing any of the classes.
///
/// Another feature is the ability to separate different user-action
/// functions.  For example, you don't have to mix your code that
/// writes hits with the code that makes histograms; the code can be
/// put into separate classes.
///
/// 18-Sep-2007: Make this a true "Manager" class by turning it into a
/// singleton.  Give the UserAction-derived classes access to the
/// Geant4 user-class managers.

#ifndef G4BASE_UserActionManager_H
#define G4BASE_UserActionManager_H

#include "G4Base/UserAction.h"

#include "G4UserRunAction.hh"
#include "G4UserEventAction.hh"
#include "G4UserTrackingAction.hh"
#include "G4UserSteppingAction.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4Step.hh"

#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"

#include <vector>

namespace g4b {

  class UserActionManager : public G4UserRunAction
			  , public G4UserEventAction
			  , public G4UserTrackingAction
			  , public G4UserSteppingAction {
  public:
  
    // Access to instance:
    static UserActionManager* Instance();

    virtual ~UserActionManager();
  
    // Delete all the UserAction classes we manage.
    void Close();

    G4int GetSize()                             const { return fuserActions.size(); }
    UserAction* GetAction(G4int i)              const { return fuserActions[i];     }
    static void AddAndAdoptAction(UserAction* a)      { fuserActions.push_back(a);  }

    virtual void BeginOfRunAction      (const G4Run*  );
    virtual void EndOfRunAction        (const G4Run*  );
    virtual void BeginOfEventAction    (const G4Event*);
    virtual void EndOfEventAction      (const G4Event*);
    virtual void PreUserTrackingAction (const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);
    virtual void UserSteppingAction    (const G4Step* );

    // "Mysterious accessors": Where do the pointers to these managers
    // come from?  They are all defined in the G4User*Action classes.
    // Use care when calling these accessors; for example, the
    // SteppingManager is probably not available in a TrackingAction
    // method.  Keep the heirarchy in mind: Run > Event > Track >
    // Step.
    G4EventManager* GetEventManager()       const { return fpEventManager;    }
    G4TrackingManager* GetTrackingManager() const { return fpTrackingManager; }
    G4SteppingManager* GetSteppingManager() const { return fpSteppingManager; }

  private:
    typedef std::vector<UserAction*>       fuserActions_t;
    typedef fuserActions_t::const_iterator fuserActions_ptr_t;
    static  fuserActions_t                 fuserActions; 

  protected:
    // The constructor is protected according to the standard
    // singleton pattern.
    UserActionManager();
  
  };

} // namespace g4b

#endif // G4BASE_UserActionManager_H
