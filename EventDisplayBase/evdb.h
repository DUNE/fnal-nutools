////////////////////////////////////////////////////////////////////////
//  $Id: evdb.h,v 1.4 2011-04-05 22:26:55 messier Exp $
//
//! Collection of global resources for the event display
//
//  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVDB_H
#define EVDB_H
#include "EventDisplayBase/ButtonBar.h"
#include "EventDisplayBase/Canvas.h"
#include "EventDisplayBase/ColorScale.h"
#include "EventDisplayBase/DisplayWindow.h"
#include "EventDisplayBase/EditMenu.h"
#include "EventDisplayBase/EventDisplay.h"
#include "EventDisplayBase/EventHolder.h"
#include "EventDisplayBase/FileMenu.h"
#include "EventDisplayBase/PrintDialog.h"
#include "EventDisplayBase/Functors.h"
#include "EventDisplayBase/HelpMenu.h"
#include "EventDisplayBase/JobMenu.h"
#include "EventDisplayBase/ListWindow.h"
#include "EventDisplayBase/MenuBar.h"
#include "EventDisplayBase/NavState.h"
#include "EventDisplayBase/ObjListCanvas.h"
#include "EventDisplayBase/ParameterSetEdit.h"
#include "EventDisplayBase/Printable.h"
#include "EventDisplayBase/RootEnv.h"
#include "EventDisplayBase/ScanOptions.h"
#include "EventDisplayBase/ScanWindow.h"
#include "EventDisplayBase/StatusBar.h"
#include "EventDisplayBase/View2D.h"
#include "EventDisplayBase/View3D.h"
#include "EventDisplayBase/WindowMenu.h"


class TGMainFrame;
class TGWindow;
class TGPicturePool;

/// Base package for construction of an event display
namespace evdb {
  const TGWindow* TopWindow();
  TGPicturePool*  PicturePool();
}

#endif // EVDB_H
////////////////////////////////////////////////////////////////////////
