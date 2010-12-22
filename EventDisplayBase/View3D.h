////////////////////////////////////////////////////////////////////////
/// \file  View3D.h
/// \brief A collection of 3D drawable objects
/// 
/// \version $Id: View3D.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVDB_VIEW3D_H
#define EVDB_VIEW3D_H
#include <list>

class TMarker3DBox;
class TPolyMarker3D;
class TPolyLine3D;
class TText;

namespace evdb {
  class View3D {
  public:
    View3D();
    ~View3D();
    
    void Draw();
    void Clear();
    
    TMarker3DBox&  AddMarker3DBox(double x,  double y,  double z,
				  double dx, double dy, double dz,
				  double th=0.0, double ph=0.0);
    TPolyMarker3D& AddPolyMarker3D(int n, int c, int st, double sz);
    TPolyLine3D&   AddPolyLine3D(int n, int c, int w, int s);
    TText&         AddText(double x, double y, const char* text);
    
  private:
    std::list<TMarker3DBox*>  fMarker3DBoxL;  //!< List of 3D marker boxes
    std::list<TPolyMarker3D*> fPolyMarker3DL; //!< List of poly markers
    std::list<TPolyLine3D*>   fPolyLine3DL;   //!< List of poly lines
    std::list<TText*>         fText3DL;       //!< List of texts
    
    std::list<TMarker3DBox*>::iterator  fNextMarker3DBox; //!< Top of the list
    std::list<TPolyMarker3D*>::iterator fNextPolyMarker3D;//!< Top of the list
    std::list<TPolyLine3D*>::iterator   fNextPolyLine3D;  //!< Top of the list
    std::list<TText*>::iterator         fNextText3D;      //!< Top of the list
  };
}

#endif // B_VIEW3D_H
////////////////////////////////////////////////////////////////////////
