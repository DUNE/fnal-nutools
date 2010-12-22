////////////////////////////////////////////////////////////////////////
/// \file  UserAction.h
/// \brief see below
///
/// \version $Id: UserAction.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  seligman@nevis.columbia.edu, brebel@fnal.gov
////////////////////////////////////////////////////////////////////////
/// G4Base::UserAction.h
/// 1-Sep-1999 Bill Seligman
///
/// 27-Jan-2009 <seligman@nevis.columbia.edu> Revised for LArSoft.
///
/// This is an abstract base class to be used with Geant 4.0.1 (and
/// possibly higher, if the User classes don't change).  
///
/// Why is this interface useful?  Answer: Geant4 provides several
/// classes that are meant to be "user hooks" in G4 processing.  A
/// couple of examples are G4UserRunAction and G4EventAction.  The user
/// is meant to publically inherit from these classes in order to
/// perform tasks at the beginning and ending of run or event
/// processing.
///
/// However, typical tasks that physicists perform generally involve
/// more than one user-hook class.  For example, to make histograms, a
/// physicist might define the histograms at the beginning of a run,
/// fill the histograms after each event, and write the histograms at
/// the end of a run.
///
/// It's handy to have all the code for such tasks (making histograms,
/// studying G4 tracking, event persistency) all in one class, rather
/// than split between two or three different classes.  That's where
/// UserAction comes in.  It gathers all the G4 user-hook or
/// user-action classes into one place.

#ifndef G4BASE_UserAction_H
#define G4BASE_UserAction_H

// The following objects are the arguments to the methods
// invoked in the user action classes.  In other words, they
// contain the variables that we are normally able to record
// in Geant.

class G4Run;
class G4Event;
class G4Track;
class G4Step;

namespace g4b {

  class UserAction {
   
  public:

    UserAction() {};
    virtual ~UserAction() {};

    /// The following a list of methods that correspond to the available 
    /// user action classes in Geant 4.0.1 and higher.

    /// Note that the user stacking action is not included; it's a
    /// little tricky to implement in Geant4.  Let's see if we have a
    /// need for it first.

    virtual void BeginOfRunAction(const G4Run*)     {};
    virtual void EndOfRunAction(const G4Run*)       {};
    virtual void BeginOfEventAction(const G4Event*) {};
    virtual void EndOfEventAction(const G4Event*)   {};
    virtual void PreTrackingAction(const G4Track*)  {};
    virtual void PostTrackingAction(const G4Track*) {};
    virtual void SteppingAction(const G4Step*)      {};
  };

} // namespace g4b

#endif // G4BASE_UserAction_H
