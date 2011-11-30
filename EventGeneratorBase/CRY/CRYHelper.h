////////////////////////////////////////////////////////////////////////
/// \file CRYHelper.h
/// \brief Interface to the CRY cosmic ray generator
///
/// For documentation on CRY, see: http://nuclear.llnl.gov/simulation/
/// and http://nuclear.llnl.gov/simulations/additional_bsd.html
/// 
/// \version $Id: CRYHelper.h,v 1.7 2011-11-30 19:32:38 bckhouse Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVGB_CRYHELPER_H
#define EVGB_CRYHELPER_H
#include <string>
#include <vector>
#include "CLHEP/Random/RandEngine.h"

namespace simb { class MCTruth;  }
namespace geo  { class Geometry; }
class CRYSetup;
class CRYGenerator;
class CRYParticle;

namespace evgb {
    /// Interface to the CRY cosmic-ray generator
  class CRYHelper {
  public:
    CRYHelper();
    explicit CRYHelper(fhicl::ParameterSet const& pset, 
		       CLHEP::HepRandomEngine& engine);
    ~CRYHelper();

    void Sample(simb::MCTruth& mctruth, double* w);
    
  private:
    
    void ProjectToBoxEdge(const double xyz[],
			  const double dxyz[],
			  double &xlo, double &xhi,
			  double &ylo, double &yhi,
			  double &zlo, double &zhi,
			  double xyzout[]);

    CRYSetup*                  fSetup;      ///< CRY configuration
    CRYGenerator*              fGen;        ///< The CRY generator
    double                     fSampleTime; ///< Amount of time to sample (seconds)
    double                     fToffset;    ///< Shift in time of particles (s)
    double                     fEthresh;    ///< Cut on kinetic energy (GeV)
    std::string                fLatitude;   ///< Latitude of detector need space after value
    std::string                fAltitude;   ///< Altitude of detector need space after value
    std::string                fSubBoxL;    ///< Length of subbox (m) need space after value
    double                     fBoxDelta;   ///< Adjustment to the size of the world box in 
                                            ///< each dimension to avoid G4 rounding errors
  };

  // The following stuff is for the random number gererator
  template<class T> class RNGWrapper {
  public:
    static void set(T* object, double (T::*func)(void));
    static double rng(void);
  private:
    static T* m_obj;
    static double (T::*m_func)(void);
  };// end of RNGWrapper class
  
  template<class T> T* RNGWrapper<T>::m_obj;
  
  template<class T> double (T::*RNGWrapper<T>::m_func)(void);
  
  template<class T> void RNGWrapper<T>::set(T* object, double (T::*func)(void)) {
    m_obj = object; m_func = func;
  }
  
  template<class T> double RNGWrapper<T>::rng(void) { return (m_obj->*m_func)(); }

}
#endif // EVGB_CRYHELPER_H
////////////////////////////////////////////////////////////////////////
