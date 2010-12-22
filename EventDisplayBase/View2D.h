////////////////////////////////////////////////////////////////////////
/// \file  View2D.h
/// \brief A collection of drawable 2-D objects.
///
/// \version $Id: View2D.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVDB_VIEW2D_H
#define EVDB_VIEW2D_H
#include <list>

class TMarker;
class TPolyMarker;
class TLine;
class TPolyLine;
class TArc;
class TBox;
class TText;
class TLatex;

namespace evdb {
  class View2D {
  public:
    View2D();
    ~View2D();
    
    void Draw();
    void Clear();
    
    // The list of object which make up the view
    TMarker&     AddMarker(double x, double y, int c, int st, double sz);
    TPolyMarker& AddPolyMarker(int n, int c, int st, double sz);
    TLine&       AddLine(double x1, double y1, double x2, double y2);
    TPolyLine&   AddPolyLine(int n, int c, int w, int s);
    TArc&        AddArc(double x,double t,double r,double a=0.,double b=360.);
    TBox&        AddBox(double x1, double y1, double x2, double y2);
    TText&       AddText(double x, double y, const char* text);
    TLatex&      AddLatex(double x, double y, const char* text);
    
  private:
    std::list<TMarker*>     fMarkerL;     //!< List of markers
    std::list<TPolyMarker*> fPolyMarkerL; //!< List of poly markers
    std::list<TLine*>       fLineL;       //!< List of poly lines
    std::list<TPolyLine*>   fPolyLineL;   //!< List of poly lines
    std::list<TArc*>        fArcL;        //!< List of arcs
    std::list<TBox*>        fBoxL;        //!< List of boxes
    std::list<TText*>       fTextL;       //!< List of texts
    std::list<TLatex*>      fLatexL;      //!< List of LaTeX texts
    
    std::list<TMarker*>::iterator     fNextMarker;    //!< Next in list to use
    std::list<TPolyMarker*>::iterator fNextPolyMarker;//!< for "add"
    std::list<TLine*>::iterator       fNextLine;      //!< Try to avoid lots of
    std::list<TPolyLine*>::iterator   fNextPolyLine;  //!< "news" and "deletes"
    std::list<TArc*>::iterator        fNextArc;       //!< to keep things fast
    std::list<TBox*>::iterator        fNextBox;       //!< to keep things fast
    std::list<TText*>::iterator       fNextText;
    std::list<TLatex*>::iterator      fNextLatex;
  };
}

#endif // EVDB_VIEW2D_H
////////////////////////////////////////////////////////////////////////
