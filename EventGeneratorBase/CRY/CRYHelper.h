////////////////////////////////////////////////////////////////////////
/// \file CRYHelper.h
/// \brief Interface to the CRY cosmic ray generator
///
/// For documentation on CRY, see: http://nuclear.llnl.gov/simulation/
/// and http://nuclear.llnl.gov/simulations/additional_bsd.html
/// 
/// \version $Id: CRYHelper.h,v 1.5 2011-07-18 17:01:33 brebel Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVGB_CRYHELPER_H
#define EVGB_CRYHELPER_H
#include <string>
#include <vector>

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
    explicit CRYHelper(fhicl::ParameterSet const& pset);
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
    std::vector<CRYParticle*>* fEvt;        ///< List of particles
    double                     fSampleTime; ///< Amount of time to sample (seconds)
    double                     fToffset;    ///< Shift in time of particles (s)
    double                     fEthresh;    ///< Cut on kinetic energy (GeV)
    std::string                fLatitude;   ///< Latitude of detector need space after value
    std::string                fAltitude;   ///< Altitude of detector need space after value
    std::string                fSubBoxL;    ///< Length of subbox (m) need space after value
    double                     fBoxDelta;   ///< Adjustment to the size of the world box in each dimension to avoid G4 rounding errors
  };
}
#endif // EVGB_CRYHELPER_H
////////////////////////////////////////////////////////////////////////
