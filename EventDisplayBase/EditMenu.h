////////////////////////////////////////////////////////////////////////
/// \file  EditMenu.h
/// \brief The edit pull down menu
///
/// \version $Id: EditMenu.h,v 1.4 2011-04-17 14:55:31 brebel Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVDB_EDITMENU_H
#define EVDB_EDITMENU_H
#include "TQObject.h"
#include "RQ_OBJECT.h"
class TGMainFrame;
class TGMenuBar;
class TGPopupMenu;
class TGLayoutHints;

namespace evdb {
  /// The edit pull down menu
  class EditMenu {
    RQ_OBJECT("evdb::EditMenu")
      
  public:
    EditMenu(TGMenuBar* menubar, TGMainFrame* mf);
    virtual ~EditMenu();

    void SetWorkers(const std::vector<std::string>& w);
  
    // slots
    void EditDrawingOptions(int drawopt);
  
  private:
    int Preferences();
    int NoImpl(const char* m);

  private:
    TGMainFrame*   fMainFrame;          ///< Main graphics frame
    TGPopupMenu*   fEditMenu;           ///< The file menu
    TGLayoutHints* fLayout;             ///< How to layout the menu
  };
}

#endif // EVDB_EDITMENU_H
////////////////////////////////////////////////////////////////////////
