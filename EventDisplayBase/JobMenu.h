////////////////////////////////////////////////////////////////////////
/// \file  JobMenu.h
/// \brief The job pull down menu
///
/// \version $Id: JobMenu.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVDB_JOBMENU_H
#define EVDB_JOBMENU_H
#include <vector>
#include "TObject.h"
#include "TQObject.h"
#include "RQ_OBJECT.h"
class TGMainFrame;
class TGMenuBar;
class TGPopupMenu;
class TGLayoutHints;

namespace evdb {
  /// The job pull dow menu
  class JobMenu 
  {
    RQ_OBJECT("evdb::JobMenu");
    
  public:
    JobMenu(TGMenuBar* menubar, TGMainFrame* mf);
    virtual ~JobMenu();
    void SetWorkers(const std::vector<std::string>& w);

    // slots
    void HandleMenu(int menu);
    void EditConfig(int cfg);
    
  private:
    int  OpenJobXML();
    void ResetJob();
    
  private:
    TGMainFrame*   fMainFrame;  ///< Main graphics frame
    TGPopupMenu*   fJobMenu;    ///< The file menu
    TGPopupMenu*   fConfigMenu; ///< The configuration menu
    TGLayoutHints* fLayout;     ///< How to layout the menu
  };
}
#endif // EVDB_FILEMENU_H
////////////////////////////////////////////////////////////////////////
