////////////////////////////////////////////////////////////////////////
/// \file  MCNeutrino.cxx
/// \brief Simple MC truth class, holds a vector of TParticles
///
/// \version $Id: MCNeutrino.cxx,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  jpaley@indiana.edu
////////////////////////////////////////////////////////////////////////
#include "SimulationBase/simbase.h"
#include "TVector3.h"
#include <iostream>

namespace simb{

  //......................................................................
  MCNeutrino::MCNeutrino()
  {
  }

  //......................................................................
  ///nu is the incoming neutrino and lep is the outgoing lepton
  MCNeutrino::MCNeutrino(MCParticle &nu, MCParticle &lep, 
			 int CCNC, int mode, int target, int nucleon,
			 int quark, double w, double x, double y, double qsqr) :
    fNu(nu),
    fLepton(lep),
    fMode(mode),
    fCCNC(CCNC),
    fTarget(target),
    fHitNuc(nucleon),
    fHitQuark(quark),
    fW(w),
    fX(x),
    fY(y),
    fQSqr(qsqr)
  { 
  }

  //......................................................................
  double MCNeutrino::Theta() const
  {
    ///make TVector3 objects for the momenta of the incoming neutrino
    ///and outgoing lepton
    TVector3 in(fNu.Px(), fNu.Py(), fNu.Pz());
    TVector3 out(fLepton.Px(), fLepton.Py(), fLepton.Pz());

    return in.Angle(out);
  }

  //......................................................................
  double MCNeutrino::Pt() const
  {
    return fNu.Pt();
  }

  //......................................................................
  std::ostream&  operator<< (std::ostream& output, const simb::MCNeutrino &mcnu)
  {
    output << " neutrino =        " << mcnu.Nu().PdgCode() 
	   << " neutrino energy = " << mcnu.Nu().E()
	   << " CCNC =            " << mcnu.CCNC()
	   << " mode =            " << mcnu.Mode();
    output << " target =          " << mcnu.Target() 
	   << " nucleon =         " << mcnu.HitNuc()
	   << " quark =           " << mcnu.HitQuark() << std::endl;
    output << " W =               " << mcnu.W() 
	   << " X =               " << mcnu.X() 
	   << " Y =               " << mcnu.Y() 
	   << " Q^2 =             " << mcnu.QSqr() << std::endl;
  }

  MCNeutrino::~MCNeutrino() { }
}
////////////////////////////////////////////////////////////////////////
