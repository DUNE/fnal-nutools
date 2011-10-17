////////////////////////////////////////////////////////////////////////
///
/// \file    ScanWindow.h
/// \brief   A window containing dialog boxes for handscans
/// \author  brebel@fnal.gov
/// \version $Id: ScanWindow.h,v 1.4 2011-10-17 16:18:32 brebel Exp $
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
    ScanFrame(const TGWindow *p, 
	      unsigned int nCategories,
	      unsigned int maxFields);
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

    TGGroupFrame     *fFrame;
    TGCanvas         *fCanvas;
    TGMatrixLayout   *fML;
    
    std::vector<TGGroupFrame*>      fCatFrames;       ///< frames to hold fields for a given category
    std::vector<TGTextEntry*>       fTextBoxes;       ///< Text box entry fields
    std::vector<TGNumberEntry*>     fNumberBoxes;     ///< Number box entry fields
    std::vector<TGRadioButton*>     fRadioButtons;    ///< Radio button entry fields
    std::vector<TGCheckButton*>     fCheckButtons;    ///< Check button entry fields
    std::vector<int>                fRadioButtonIds;  ///< Ids for the radio buttons
    
  };
}

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

  private:
    
    ScanFrame*          fScan;        ///< Frame holding the scan fields    
    TGCanvas*           fCanvas;      ///< Canvas holding the ScanFrame     
    TGCompositeFrame*   fF3;          ///< Frame to hold record button      
    TGTextButton*       fB3;          ///< Record button		       
    TGTextButton*       fB4;          ///< Prev button		       
    TGTextButton*       fB5;          ///< Next button		       
    TGTextEntry*        fComments;    ///< Next button		       
    TGLabel*            fL1;
    TGCompositeFrame*   fF1;
					                                        
    std::string         fOutFileName; ///< Output file name for scan results

    ClassDef(ScanWindow, 0);
  };

}// namespace

#endif //EVD_SCANVIEW_H
