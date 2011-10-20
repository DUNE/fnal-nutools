////////////////////////////////////////////////////////////////////////
/// \file  UserActionManager.cxx
/// \brief Create the physics lists to be used by Geant4.
///
/// \version $Id: UserActionManager.cxx,v 1.2 2011-10-20 17:10:56 brebel Exp $
/// \author  seligman@nevis.columbia.edu
////////////////////////////////////////////////////////////////////////
/// 18-Sep-2007 Bill Seligman
/// Invoke the appropriate action for each stored user-action class.

/// 27-Jan-2009 <seligman@nevis.columbia.edu> Revised for LArSoft.

#include "G4Base/UserActionManager.h"
#include "G4Base/UserAction.h"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4Step.hh"

#include <vector>

namespace g4b{

  UserActionManager::fuserActions_t UserActionManager::fuserActions;

  //-------------------------------------------------
  UserActionManager::UserActionManager() 
  {
  }

  //-------------------------------------------------
  // Standard implementation of a singleton pattern.
  UserActionManager* UserActionManager::Instance()
  {
    static UserActionManager instance;
    return &instance;
  }

  //-------------------------------------------------
  UserActionManager::~UserActionManager()
  {
    // This destructor is probably never going to be called.  If it
    // is, make sure all the UserAction classes we manage are deleted
    // properly.
    Close();
  }


  //-------------------------------------------------
  void UserActionManager::Close()
  {
    // Since we adopted the pointers for the user-action classes we're
    // managing, delete all of them.
    for ( fuserActions_t::iterator i = fuserActions.begin(); i != fuserActions.end(); i++ ){
      delete *i;
    }
  
    fuserActions.clear();
  }


  //-------------------------------------------------
  // For the rest of the UserAction methods: invoke the corresponding
  // method for each of the user-action classes we're managing.

  // Reminder: i is a vector<UserAction*>::iterator
  //          *i is a UserAction*

  //-------------------------------------------------
  void UserActionManager::BeginOfRunAction(const G4Run* a_run)
  {
    for ( fuserActions_ptr_t i = fuserActions.begin(); i != fuserActions.end(); i++ ){
      (*i)->BeginOfRunAction(a_run);
    }
  }

  //-------------------------------------------------
  void UserActionManager::EndOfRunAction(const G4Run* a_run)
  {
    for ( fuserActions_ptr_t i = fuserActions.begin(); i != fuserActions.end(); i++ ){
      (*i)->EndOfRunAction(a_run);
    }
  }

  //-------------------------------------------------
  void UserActionManager::BeginOfEventAction(const G4Event* a_event)
  {
    for ( fuserActions_ptr_t i = fuserActions.begin(); i != fuserActions.end(); i++ ){
      (*i)->BeginOfEventAction(a_event);
    }
  }

  //-------------------------------------------------
  void UserActionManager::EndOfEventAction(const G4Event* a_event)
  {
    for ( fuserActions_ptr_t i = fuserActions.begin(); i != fuserActions.end(); i++ ) {
      (*i)->EndOfEventAction(a_event);
    }
  }

  //-------------------------------------------------
  void UserActionManager::PreUserTrackingAction(const G4Track* a_track)
  {
    for ( fuserActions_ptr_t i = fuserActions.begin(); i != fuserActions.end(); i++ ){
      (*i)->PreTrackingAction(a_track);
    }
  }

  //-------------------------------------------------
  void UserActionManager::PostUserTrackingAction(const G4Track* a_track)
  {
    for ( fuserActions_ptr_t i = fuserActions.begin(); i != fuserActions.end(); i++ ){
      (*i)->PostTrackingAction(a_track);
    }
  }

  //-------------------------------------------------
  void UserActionManager::UserSteppingAction(const G4Step* a_step)
  {
    for ( fuserActions_ptr_t i = fuserActions.begin(); i != fuserActions.end(); i++ ){
      (*i)->SteppingAction(a_step);
    }
  }

}// namespace
