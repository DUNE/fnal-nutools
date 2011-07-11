////////////////////////////////////////////////////////////////////////
/// \file  PrintDialog.h
/// \brief Dialog box for printing displays
///
/// \version $Id: PrintDialog.h,v 1.2 2011-07-11 22:29:16 bckhouse Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVDB_PRINTDIALOG_H
#define EVDB_PRINTDIALOG_H
#include <string>
#include "TQObject.h"
#include "RQ_OBJECT.h"
#include "TGFrame.h"
class TGCompositeFrame;
class TGLayoutHints;
class TGCheckButton;
class TGTextEntry;
class TGTextButton;
namespace evdb { class Printable; }

namespace evdb {
  class PrintDialog : public TGTransientFrame {
    RQ_OBJECT("evdb::PrintDialog")
    
  public:
    PrintDialog();
    ~PrintDialog();
    
    void CloseWindow();
    void Print();
    void Cancel();
    
  private:
    TGCompositeFrame* fPrintFrame[10];  
    TGCheckButton*    fPrintableCB[10];
    TGTextEntry*      fFilename[10];
    TGCheckButton*    fDoEPS[10];
    TGCheckButton*    fDoPDF[10];
    TGCheckButton*    fDoGIF[10];
    TGCheckButton*    fDoPNG[10];
    
    TGCompositeFrame* fButtonFrame;
    TGTextButton*     fPrintButton;
    TGTextButton*     fCancelButton;
    
    TGLayoutHints*    fL1;
    TGLayoutHints*    fL2;
    
    int         fNprintable;    // Number of printable objects
    std::string fPrintTag[10];  // Tag names of printables
    Printable*  fPrintable[10]; // Pointers to printables. Not owner!

    ClassDef(PrintDialog, 0);
  };
}
#endif
