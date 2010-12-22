////////////////////////////////////////////////////////////////////////
///
/// \file    ScanWindow.h
/// \brief   A window containing dialog boxes for handscans
/// \author  brebel@fnal.gov
/// \version $Id: ScanWindow.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
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
    void RadioButton();

  private:

    TGCompositeFrame*               fScanCompFrame;   ///< Frame to hold all Category Frames
    TGCompositeFrame*               fButtonCompFrame; ///< Frame to hold all Button Frames
    TGCompositeFrame*               fMainFrame;       ///< Frame to hold all fScanCompFrame and fButtonCompFrame

    TGCompositeFrame*               fF3;              ///< Frame to hold record button
    TGTextButton*                   fB3;              ///< Record button
    TGTextButton*                   fB4;              ///< Prev button
    TGTextButton*                   fB5;              ///< Next button

    std::string                     fOutFileName;     ///< Output file name for scan results

    std::vector<TGGroupFrame*>      fCatFrames;       ///< frames to hold fields for non-button entry
    std::vector<TGGroupFrame*>      fButtonFrames;    ///< frames to hold fields for button entry
    std::vector<TGTextEntry*>       fTextBoxes;       ///< Text box entry fields
    std::vector<TGNumberEntry*>     fNumberBoxes;     ///< Number box entry fields
    std::vector<TGRadioButton*>     fRadioButtons;    ///< Radio button entry fields
    std::vector<TGCheckButton*>     fCheckButtons;    ///< Check button entry fields
    std::vector<int>  fRadioButtonIds;
    
  };

}// namespace

#endif //EVD_SCANVIEW_H
