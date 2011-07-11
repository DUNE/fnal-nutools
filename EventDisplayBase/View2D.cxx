////////////////////////////////////////////////////////////////////////
/// \file  View2D.cxx
/// \brief A collection of drawable 2-D objects
///
/// \version $Id: View2D.cxx,v 1.2 2011-07-11 21:34:39 bckhouse Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include "EventDisplayBase/View2D.h"
#include "EventDisplayBase/Functors.h"
using namespace evdb;

//......................................................................

View2D::View2D() 
{ 
  fNextMarker     = fMarkerL.end();
  fNextPolyMarker = fPolyMarkerL.end();
  fNextLine       = fLineL.end();
  fNextPolyLine   = fPolyLineL.end();
  fNextArc        = fArcL.end();
  fNextBox        = fBoxL.end();
  fNextText       = fTextL.end();
  fNextLatex      = fLatexL.end();

  this->AddMarker(0.0,0.0,2,1,0.1);
  this->AddPolyMarker(1,2,20,0.1);
  this->AddLine(0.0,0.0,1.0,1.0);
  this->AddPolyLine(1,2,1,1);
  this->AddArc(0.0,0.0,1.0);
  this->AddBox(0.0,1.0,0.0,1.0);
  this->AddText(0.0,0.0,"");
  this->AddLatex(0.0,0.0,"");
  this->Clear();
}

//......................................................................

View2D::~View2D() 
{
  for_each(fMarkerL.begin(),    fMarkerL.end(),    delete_marker());
  for_each(fPolyMarkerL.begin(),fPolyMarkerL.end(),delete_polymarker());
  for_each(fLineL.begin(),      fLineL.end(),      delete_line());
  for_each(fPolyLineL.begin(),  fPolyLineL.end(),  delete_polyline());
  for_each(fArcL.begin(),       fArcL.end(),       delete_arc());
  for_each(fBoxL.begin(),       fBoxL.end(),       delete_box());
  for_each(fTextL.begin(),      fTextL.end(),      delete_text());
  for_each(fLatexL.begin(),     fLatexL.end(),     delete_latex());
}

//......................................................................

void View2D::Draw()
{
  for_each(fArcL.begin(),       fNextArc,       draw_tobject());
  for_each(fBoxL.begin(),       fNextBox,       draw_tobject());
  for_each(fPolyLineL.begin(),  fNextPolyLine,  draw_tobject());
  for_each(fLineL.begin(),      fNextLine,      draw_tobject());
  for_each(fMarkerL.begin(),    fNextMarker,    draw_tobject());
  for_each(fPolyMarkerL.begin(),fNextPolyMarker,draw_tobject());
  for_each(fTextL.begin(),      fNextText,      draw_tobject());
  for_each(fLatexL.begin(),     fNextLatex,     draw_tobject());
}

//......................................................................

void View2D::Clear() 
{
  // To clear, just reset the next positions to use for inserts to the
  // begining of the lists. Keep all the memory around as it it very likely
  // we will reuse it
  fNextMarker     = fMarkerL.begin();
  fNextPolyMarker = fPolyMarkerL.begin();
  fNextLine       = fLineL.begin();
  fNextPolyLine   = fPolyLineL.begin();
  fNextArc        = fArcL.begin();
  fNextBox        = fBoxL.begin();
  fNextText       = fTextL.begin();
  fNextLatex      = fLatexL.begin();
}

//......................................................................

TMarker& View2D::AddMarker(double x, double y, int c, int st, double sz)
{
  TMarker* m = 0;
  if (fNextMarker == fMarkerL.end()) {
    // Grow the list...
    m = new TMarker(x,y,st);
    m->SetBit(kCanDelete,kFALSE);
    m->SetMarkerColor(c);
    m->SetMarkerSize(sz);
    if (fNextMarker == fMarkerL.begin()) fMarkerL.push_front(m);
    else                                 fMarkerL.push_back(m);
    fNextMarker = fMarkerL.end();
  }
  else {
    // Reuse the marker at the current position
    m = *fNextMarker;
    m->SetX(x);
    m->SetY(y);
    m->SetMarkerSize(sz);
    m->SetMarkerColor(c);
    m->SetMarkerStyle(st);
    ++fNextMarker;
  }
  // Return the marker just added so users can twiddle it
  return *m;
}

//......................................................................

TPolyMarker& View2D::AddPolyMarker(int n, int c, int st, double sz)
{
  TPolyMarker* pm = 0;
  if (fNextPolyMarker == fPolyMarkerL.end()) {
    // Grow the list...
    pm = new TPolyMarker(n);
    pm->SetBit(kCanDelete,kFALSE);
    pm->SetMarkerColor(c);
    pm->SetMarkerStyle(st);
    pm->SetMarkerSize(sz);
    fPolyMarkerL.push_back(pm);
    fNextPolyMarker = fPolyMarkerL.end();
  }
  else {
    // Reuse the polymarker at the current position
    pm = *fNextPolyMarker;
    pm->SetPolyMarker(n); // reset elements in PolyMarket
    pm->SetMarkerColor(c);
    pm->SetMarkerSize(sz);
    pm->SetMarkerStyle(st);
    ++fNextPolyMarker;
  }
  // Return the marker just added so users can twiddle it (ie. add points)
  return *pm;
}

//......................................................................

TLine& View2D::AddLine(double x1, double y1, double x2, double y2)
{
  TLine* ln = 0;
  if (fNextLine == fLineL.end()) {
    // Grow the list...
    ln = new TLine(x1,y1,x2,y2);
    ln->SetBit(kCanDelete,kFALSE);
    fLineL.push_back(ln);
    fNextLine = fLineL.end();
  }
  else {
    // Reuse the polyline at the current position
    ln = *fNextLine;
    ln->SetX1(x1);
    ln->SetY1(y1);
    ln->SetX2(x2);
    ln->SetY2(y2);
    ++fNextLine;
  }
  // Return the marker just added so users can twiddle it (ie. add points)
  return *ln;
}

//......................................................................

TPolyLine& View2D::AddPolyLine(int n, int c, int w, int s)
{
  TPolyLine* pl = 0;
  if (fNextPolyLine == fPolyLineL.end()) {
    // Grow the list...
    pl = new TPolyLine(n);
    pl->SetBit(kCanDelete,kFALSE);
    pl->SetLineColor(c);
    pl->SetLineWidth(w);
    pl->SetLineStyle(s);
    fPolyLineL.push_back(pl);
    fNextPolyLine = fPolyLineL.end();
  }
  else {
    // Reuse the polyline at the current position
    pl = *fNextPolyLine;
    pl->SetPolyLine(n); // reset elements in PolyLine
    pl->SetOption("");
    pl->SetLineColor(c);
    pl->SetLineWidth(w);
    pl->SetLineStyle(s);
    ++fNextPolyLine;
  }
  // Return the marker just added so users can twiddle it (ie. add points)
  return *pl;
}

//......................................................................

TArc& View2D::AddArc(double x, double y, double r, double p1, double p2) 
{
  TArc* a = 0;
  if (fNextArc == fArcL.end()) {
    // Grow the list...
    a = new TArc(x,y,r,p1,p2);
    a->SetBit(kCanDelete,kFALSE);
    fArcL.push_back(a);
    fNextArc = fArcL.end();
  }
  else {
    // Reuse the arc at the current position
    a = *fNextArc;
    a->SetX1(x);
    a->SetY1(y);
    a->SetR1(r);
    a->SetR2(r);
    a->SetPhimin(p1);
    a->SetPhimax(p2);
    ++fNextArc;
  }
  // Return the marker just added so users can twiddle it (ie. add points)
  return *a;
}

//......................................................................

TBox& View2D::AddBox(double x1, double y1, double x2, double y2)
{
  TBox* b = 0;
  if (fNextBox == fBoxL.end()) {
    // Grow the list...
    b = new TBox(x1,y1,x2,y2);
    b->SetBit(kCanDelete,kFALSE);
    fBoxL.push_back(b);
    fNextBox = fBoxL.end();
  }
  else {
    // Reuse the arc at the current position
    b = *fNextBox;
    b->SetX1(x1);
    b->SetY1(y1);
    b->SetX2(x2);
    b->SetY2(y2);
    ++fNextBox;
  }
  // Return the box just added so users can twiddle it
  return *b;
}

//......................................................................

TText& View2D::AddText(double x, double y, const char* text)
{
  TText* itxt = 0;
  if (fNextText == fTextL.end()) {
    // Grow the list...
    itxt = new TText(x,y,text);
    itxt->SetBit(kCanDelete,kFALSE);
    fTextL.push_back(itxt);
    fNextText = fTextL.end();
  }
  else {
    // Reuse the text at the current position
    itxt = *fNextText;
    itxt->SetText(x,y,text);
    ++fNextText;
  }

  // Return the text just added so users can twiddle it
  return *itxt;
}

//......................................................................

TLatex& View2D::AddLatex(double x, double y, const char* text)
{
  TLatex* itxt = 0;
  if (fNextLatex == fLatexL.end()) {
    // Grow the list...
    itxt = new TLatex(x,y,text);
    itxt->SetBit(kCanDelete,kFALSE);
    fLatexL.push_back(itxt);
    fNextLatex = fLatexL.end();
  }
  else {
    // Reuse the text at the current position
    itxt = *fNextLatex;
    itxt->SetText(x,y,text);
    ++fNextLatex;
  }

  // Return the text just added so users can twiddle it
  return *itxt;
}

////////////////////////////////////////////////////////////////////////
