////////////////////////////////////////////////////////////////////////
/// \file  UserActionManager.cxx
/// \brief Create the physics lists to be used by Geant4.
///
/// \version $Id: UserActionManager.cxx,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
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

using namespace g4b;

UserActionManager::m_userActions_t UserActionManager::m_userActions;

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
  for ( m_userActions_t::iterator i = m_userActions.begin(); i != m_userActions.end(); i++ ){
    delete *i;
  }
  
  m_userActions.clear();
}


//-------------------------------------------------
// For the rest of the UserAction methods: invoke the corresponding
// method for each of the user-action classes we're managing.

// Reminder: i is a vector<UserAction*>::iterator
//          *i is a UserAction*

//-------------------------------------------------
void UserActionManager::BeginOfRunAction(const G4Run* a_run)
{
  for ( m_userActions_ptr_t i = m_userActions.begin(); i != m_userActions.end(); i++ ){
    (*i)->BeginOfRunAction(a_run);
  }
}

//-------------------------------------------------
void UserActionManager::EndOfRunAction(const G4Run* a_run)
{
  for ( m_userActions_ptr_t i = m_userActions.begin(); i != m_userActions.end(); i++ ){
    (*i)->EndOfRunAction(a_run);
  }
}

//-------------------------------------------------
void UserActionManager::BeginOfEventAction(const G4Event* a_event)
{
  for ( m_userActions_ptr_t i = m_userActions.begin(); i != m_userActions.end(); i++ ){
    (*i)->BeginOfEventAction(a_event);
  }
}

//-------------------------------------------------
void UserActionManager::EndOfEventAction(const G4Event* a_event)
{
  for ( m_userActions_ptr_t i = m_userActions.begin(); i != m_userActions.end(); i++ ) {
    (*i)->EndOfEventAction(a_event);
  }
}

//-------------------------------------------------
void UserActionManager::PreUserTrackingAction(const G4Track* a_track)
{
  for ( m_userActions_ptr_t i = m_userActions.begin(); i != m_userActions.end(); i++ ){
    (*i)->PreTrackingAction(a_track);
  }
}

//-------------------------------------------------
void UserActionManager::PostUserTrackingAction(const G4Track* a_track)
{
  for ( m_userActions_ptr_t i = m_userActions.begin(); i != m_userActions.end(); i++ ){
    (*i)->PostTrackingAction(a_track);
  }
}

//-------------------------------------------------
void UserActionManager::UserSteppingAction(const G4Step* a_step)
{
  for ( m_userActions_ptr_t i = m_userActions.begin(); i != m_userActions.end(); i++ ){
    (*i)->SteppingAction(a_step);
  }
}

