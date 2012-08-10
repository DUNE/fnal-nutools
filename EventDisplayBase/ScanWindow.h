////////////////////////////////////////////////////////////////////////
///
/// \file    ScanWindow.h
/// \brief   A window containing dialog boxes for handscans
/// \author  brebel@fnal.gov
/// \version $Id: ScanWindow.h,v 1.5 2012-08-10 02:45:08 messier Exp $
///
////////////////////////////////////////////////////////////////////////

#ifndef EVDB_SCANWINDOW_H
#define EVDB_SCANWINDOW_H

#include "EventDisplayBase/Canvas.h"
#include "RQ_OBJECT.h"
#include "TGCanvas.h"
#include "TGFrame.h"
#include <vector>
#include <iostream>
#include <fstream>


// Forward declarations
class TGNumberEntry;
class TGTextEntry;
class TGCheckButton;
class TGRadioButton;
class TGCanvas;
class TGMainFrame;
class TGCompositeFrame;
class TGGroupFrame;
class TGMatrixLayout;
class TGLayoutHints;
class TGLabel;
class TGTextButton;
class TGTextEntry;
class TGVScrollBar;

namespace evdb{

  /// Helper class to setup scroll bars in evdb::ScanWindow
  class ScanFrame {
    RQ_OBJECT("evdb::ScanFrame");
    
  public:
    ScanFrame(const TGWindow *p);
    virtual ~ScanFrame();
    
    TGGroupFrame *GetFrame() const { return fFrame; }
    
    void SetCanvas(TGCanvas *canvas) { fCanvas = canvas; }
    void HandleMouseWheel(Event_t *event);
    void RadioButton();
    void ClearFields();
    void Record(std::string outfilename, 
		const char* comments);
    
    int  GetHeight() const;
    int  GetWidth() const;

  private:
    unsigned int    fHeight; // Estimated height of frame
    unsigned int    fWidth;  // Estimated width of frame
    TGGroupFrame*   fFrame;
    TGCanvas*       fCanvas;
    TGMatrixLayout* fML;
    
    std::vector<TGGroupFrame*>      fCatFrames;       ///< frames to hold fields for a given category
    std::vector<TGTextEntry*>       fTextBoxes;       ///< Text box entry fields
    std::vector<TGNumberEntry*>     fNumberBoxes;     ///< Number box entry fields
    std::vector<TGRadioButton*>     fRadioButtons;    ///< Radio button entry fields
    std::vector<TGCheckButton*>     fCheckButtons;    ///< Check button entry fields
    std::vector<int>                fRadioButtonIds;  ///< Ids for the radio buttons
    
  };
}

//......................................................................

namespace evdb {

  class ScanWindow : public TGTransientFrame {

  public:
    
    RQ_OBJECT("evdb::ScanWindow");

  public:
    ScanWindow();
    ~ScanWindow();

    void CloseWindow();
    void Rec();
    void Prev();
    void Next();

    void BuildButtonBar(TGHorizontalFrame* f);
    void BuildUserFields(TGCompositeFrame* f);
    void OpenOutputFile();

  private:
    /// Scrollable frame for all user defined fields
    TGCanvas*         fUserFieldsCanvas;
    TGCompositeFrame* fUserFieldsFrame;
    TGLayoutHints*    fUserFieldsHints;

    /// Frame to hold the buttons at the bottom of the window
    TGHorizontalFrame* fButtonBar;
    TGLayoutHints*     fButtonBarHints;
    TGLabel*           fCommentLabel;
    TGTextEntry*       fCommentEntry;
    TGTextButton*      fPrevButton;
    TGTextButton*      fNextButton;
    TGTextButton*      fRcrdButton;
    TGLayoutHints*     fButtonBarHintsL;
    TGLayoutHints*     fButtonBarHintsC;
    TGLayoutHints*     fButtonBarHintsR;
    
    /// The frame containing the scanner check boxes etc.
    ScanFrame*  fScanFrame;
    std::string fOutFileName; ///< Output file name for scan results

    ClassDef(ScanWindow, 0);
  };

}// namespace

#endif //EVD_SCANVIEW_H
