////////////////////////////////////////////////////////////////////////
// $Id: ButtonBar.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
//
// A bar containing the next event, previous event, (etc.) buttons
//
// messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVDB_BUTTONBAR_H
#define EVDB_BUTTONBAR_H
#include "TQObject.h"
#include "RQ_OBJECT.h"
#include "TGButton.h"
#include "TGTextEntry.h"
class TGMainFrame;
class TGCompositeFrame;
class TGPictureButton;
class TGLayoutHints;
class TGLabel;

namespace evdb {
  class ButtonBar {
    RQ_OBJECT("evdb::ButtonBar")
    
  public:
    ButtonBar(TGMainFrame* frame);
    virtual ~ButtonBar();
    
  public: 
    void PrevEvt();
    void NextEvt();
    void ReloadEvt();
    void FileList();
    void GoTo();
    void Print();
    int  NoImpl(const char* c);
    
    void SetRunEvent(int run, int event);

  private:
    TGCompositeFrame* fButtonBar;   // The top button bar
    TGLayoutHints*    fLayout;      // Layout for button bar
    TGTextButton*     fPrevEvt;     // Goto to previous event
    TGTextButton*     fNextEvt;     // Goto to next event
    TGTextButton*     fReload;      // Reload current event
    TGTextEntry*      fCurrentFile; // Currently loaded file
    TGPictureButton*  fFileList;    // Access to the list of files attached
    
    TGLabel*          fRunEvtLabel;    // Run/Event number label
    TGTextEntry*      fRunTextEntry;   // Run number text entry
    TGTextEntry*      fEventTextEntry; // Event number text entry
    
    TGTextButton*     fGoTo;        // Go To event button
    TGTextButton*     fPrint;       // Print button
  };
}

#endif // EVDB_BUTTONBAR_H
////////////////////////////////////////////////////////////////////////
