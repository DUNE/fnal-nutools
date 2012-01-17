#include <algorithm>
#include "EventDisplayBase/View3D.h"
#include "EventDisplayBase/Functors.h"
using namespace evdb;

//......................................................................

View3D::View3D() 
{ 
  fNextMarker3DBox  = fMarker3DBoxL.end();
  fNextPolyMarker3D = fPolyMarker3DL.end();
  fNextPolyLine3D   = fPolyLine3DL.end();
  fNextText3D       = fText3DL.end();

  // Add one object of every type to get lists started
  this->AddMarker3DBox(0.0,0.0,0.0,50.0,50.0,50.0,0.0,0.0);
  this->AddPolyMarker3D(1,2,24,1.0);
  this->AddPolyLine3D(1,2,5,1);
  this->AddText(0.0,0.0,"");
  this->Clear();
}

//......................................................................

View3D::~View3D() 
{
  for_each(fMarker3DBoxL.begin(), fMarker3DBoxL.end(), delete_marker3dbox());
  for_each(fPolyMarker3DL.begin(),fPolyMarker3DL.end(),delete_polymarker3d());
  for_each(fPolyLine3DL.begin(),  fPolyLine3DL.end(),  delete_polyline3d());
  for_each(fText3DL.begin(),      fText3DL.end(),      delete_text());
}

//......................................................................

void View3D::Draw()
{
  for_each(fMarker3DBoxL.begin(), fNextMarker3DBox, draw_tobject());
  for_each(fPolyMarker3DL.begin(),fNextPolyMarker3D,draw_tobject());
  for_each(fPolyLine3DL.begin(),  fNextPolyLine3D,  draw_tobject());
  for_each(fText3DL.begin(),      fNextText3D,      draw_tobject());
}

//......................................................................

void View3D::Clear() 
{
  // To clear, just reset the next positions to use for inserts to the
  // begining of the lists. Keep all the memory around as it it very likely
  // we will reuse it
  fNextMarker3DBox  = fMarker3DBoxL.begin();
  fNextPolyMarker3D = fPolyMarker3DL.begin();
  fNextPolyLine3D   = fPolyLine3DL.begin();
  fNextText3D       = fText3DL.begin();
}

//......................................................................

TMarker3DBox& View3D::AddMarker3DBox(double x,  double y,  double z,
					double dx, double dy, double dz,
					double th, double ph)
{
  TMarker3DBox* m = 0;
  if (fNextMarker3DBox == fMarker3DBoxL.end()) {
    // Grow the list...
    m = new TMarker3DBox(x,y,z,dx,dy,dz,th,ph);
    m->SetBit(kCanDelete,kFALSE);
    fMarker3DBoxL.push_back(m);
    fNextMarker3DBox = fMarker3DBoxL.end();
  }
  else {
    // Reuse the marker at the current position
    m = *fNextMarker3DBox;
    m->SetPosition(x,y,z);
    m->SetSize(dx,dy,dz);
    ++fNextMarker3DBox;
  }
  // Return the marker just added so users can twiddle it
  return *m;
}

//......................................................................

TPolyMarker3D& View3D::AddPolyMarker3D(int n, int c, int st, double sz)
{
  TPolyMarker3D* pm = 0;
  if (fNextPolyMarker3D == fPolyMarker3DL.end()) {
    // Grow the list...
    pm = new TPolyMarker3D(n);
    pm->SetBit(kCanDelete,kFALSE);
    pm->SetMarkerColor(c);
    pm->SetMarkerStyle(st);
    pm->SetMarkerSize(sz);
    fPolyMarker3DL.push_back(pm);
    fNextPolyMarker3D = fPolyMarker3DL.end();
  }
  else {
    // Reuse the polymarker at the current position
    // the first call to SetPolyMarker3D with the 0
    // deletes the current set of points before trying
    // to make a new set
    pm = *fNextPolyMarker3D;
    pm->SetPolyMarker(0,(double*)0,1,"");
    pm->SetPolyMarker(n,(double*)0,1,"");
    pm->SetMarkerColor(c);
    pm->SetMarkerStyle(st);
    pm->SetMarkerSize(sz);
    ++fNextPolyMarker3D;
  }
  // Return the marker just added so users can twiddle it (ie. add points)
  return *pm;
}

//......................................................................

TPolyLine3D& View3D::AddPolyLine3D(int n, int c, int w, int s)
{
  TPolyLine3D* pl = 0;
  if (fNextPolyLine3D == fPolyLine3DL.end()) {
    // Grow the list...
    pl = new TPolyLine3D(n);
    pl->SetBit(kCanDelete,kFALSE);
    pl->SetLineColor(c);
    pl->SetLineWidth(w);
    pl->SetLineStyle(s);
    fPolyLine3DL.push_back(pl);
    fNextPolyLine3D = fPolyLine3DL.end();
  }
  else {
    // Reuse the polyline at the current position
    // the first call to SetPolyMarker3D with the 0
    // deletes the current set of points before trying
    // to make a new set
    pl = *fNextPolyLine3D;
    pl->SetPolyLine(0,(double*)0,"");
    pl->SetPolyLine(n,(double*)0,"");
    pl->SetLineColor(c);
    pl->SetLineWidth(w);
    pl->SetLineStyle(s);
    ++fNextPolyLine3D;
  }
  // Return the marker just added so users can twiddle it (ie. add points)
  return *pl;
}

//......................................................................

TText& View3D::AddText(double x, double y, const char* text)
{
  TText* itxt = 0;
  if (fNextText3D == fText3DL.end()) {
    // Grow the list...
    itxt = new TText(x,y,text);
    itxt->SetBit(kCanDelete,kFALSE);
    fText3DL.push_back(itxt);
    fNextText3D = fText3DL.end();
  }
  else {
    // Reuse the text at the current position
    itxt = *fNextText3D;
    itxt->SetText(x,y,text);
    ++fNextText3D;
  }

  // Return the marker just added so users can twiddle it
  return *itxt;
}

////////////////////////////////////////////////////////////////////////
