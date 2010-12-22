////////////////////////////////////////////////////////////////////////
//  $Id: evdb.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
//
//! Collection of global resources for the event display
//
//  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVDB_H
#define EVDB_H
#include "EventDisplayBase/Canvas.h"
#include "EventDisplayBase/ObjListCanvas.h"
// #include "EventDisplayBase/CfgEdit.h"
#include "EventDisplayBase/DisplayWindow.h"
#include "EventDisplayBase/ListWindow.h"
// #include "EventDisplayBase/PrintDialog.h"
#include "EventDisplayBase/RootEnv.h"
// #include "EventDisplayBase/ScanCat.h"
#include "EventDisplayBase/View2D.h"
#include "EventDisplayBase/View3D.h"

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
