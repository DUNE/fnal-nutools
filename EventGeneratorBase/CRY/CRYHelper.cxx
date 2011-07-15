////////////////////////////////////////////////////////////////////////
/// \file  CRYHelper.cxx
/// \brief Implementation of an interface to the CRY cosmic-ray generator.
///
/// \version $Id: CRYHelper.cxx,v 1.10 2011-07-15 20:36:49 rhatcher Exp $
/// \author messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#include <cmath>
#include <iostream>
#include <cassert>

// CRY include files
#include "CRYSetup.h"
#include "CRYParticle.h"
#include "CRYGenerator.h"

// ROOT include files
#include "TRandom3.h"
#include "TDatabasePDG.h"
#include "TLorentzVector.h"
#include "TGeoManager.h"

// Framework includes
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "fhiclcpp/ParameterSet.h"

// NuTools include files
#include "EventGeneratorBase/evgenbase.h"
#include "SimulationBase/simbase.h"

// Experiment include files
#include "Geometry/geo.h"

#include "CLHEP/Random/RandFlat.h"

namespace evgb{

  //......................................................................
  CRYHelper::CRYHelper() 
  {
  }

  //......................................................................
  CRYHelper::CRYHelper(fhicl::ParameterSet const& pset) 
    : fSampleTime(pset.get< double      >("SampleTime")     )
    , fToffset   (pset.get< double      >("TimeOffset")     )
    , fEthresh   (pset.get< double      >("EnergyThreshold"))
    , fLatitude  (pset.get< std::string >("Latitude")       )
    , fAltitude  (pset.get< std::string >("Altitude")       )
    , fSubBoxL   (pset.get< std::string >("SubBoxLength")   ) 
  {    
    // Construct the CRY generator
    std::string config("date 1-1-2014 "
		       "returnGammas 1 "
		       "returnElectrons 1 "
		       "returnMuons 1 "
		       "returnPions 1 "
		       "returnNeutrons 1 "
		       "returnProtons 1 ");
  
    config += fLatitude;
    config += fAltitude;
    config += fSubBoxL;

    // Find the pointer to the CRY data tables
    std::string crydatadir;
    const char* datapath = getenv("CRYDATAPATH");
    if( datapath != 0) crydatadir = datapath;
    else{
      std::cerr << "no variable CRYDATAPATH set for cry data location, bail" << std::endl;
      exit(0);
    }
      
    // Construct the event generator object
    fSetup = new CRYSetup(config, crydatadir);
    fSetup->setRandomFunction(CLHEP::RandFlat::shoot);
  
    fGen = new CRYGenerator(fSetup);
    fEvt = new std::vector<CRYParticle*>;
  }  

  //......................................................................
  CRYHelper::~CRYHelper() 
  {
    delete fEvt;
    delete fGen;
    delete fSetup;
  }

  //......................................................................
  void CRYHelper::Sample(simb::MCTruth& mctruth, double* w)
  {
    // Generator time at start of sample
    double tstart = fGen->timeSimulated();
    while (1) {
      fEvt->clear();
      fGen->genEvent(fEvt);
    
      for (unsigned int i=0; i<fEvt->size(); ++i) {
	CRYParticle* cryp = (*fEvt)[i];
      
	// Pull out the PDG code
	int pdg = cryp->PDGid();

	// Get the energies of the particles
	double ke = cryp->ke()*1.0E-3; // MeV to GeV conversion
	if (ke<fEthresh) continue;
		
	double m    = 0.; // in GeV

	static TDatabasePDG*  pdgt = TDatabasePDG::Instance();
	TParticlePDG* pdgp = pdgt->GetParticle(pdg);
	if (pdgp) m = pdgp->Mass();

	double etot = ke + m;
	double ptot = etot*etot-m*m;
	if (ptot>0.0) ptot = sqrt(ptot);
	else          ptot = 0.0;
      
	// Sort out the momentum components. Remember that the NOvA
	// frame has y up and z along the beam. So uvw -> zxy
	double px = ptot * cryp->v();
	double py = ptot * cryp->w();
	double pz = ptot * cryp->u();
      
	art::ServiceHandle<geo::Geometry> geo;

	// Particle start position. CRY distributes uniformly in x-y
	// plane at fixed z, where z is the vertical direction. This
	// requires some offsets and rotations to put the particles at
	// the surface in the geometry as well as some rotations
	// since the coordinate frame has y up and z along the
	// beam.
	double vx = cryp->y()*100.0;
	double vy = cryp->z()*100.0 + geo->SurfaceY();
	double vz = cryp->x()*100.0 + 0.5*geo->DetLength();
	double t  = cryp->t()-tstart + fToffset; // seconds

	// Project backward to edge of world volume
	double xyz[3]  = { vx,  vy,  vz};
	double xyzo[3];
	double dxyz[3] = {-px, -py, -pz};
	double x1, x2;
	double y1, y2;
	double z1, z2;
	geo->WorldBox(&x1, &x2, &y1, &y2, &z1, &z2);
	
	double box_delta=0.00001;//eventually this will be set from the .fcl file
	x1=x1+box_delta;
	x2=x2-box_delta;
	y2=y2+box_delta;
	y2=y2-box_delta;
	z1=z1+box_delta;
	z2=z2-box_delta;
	mf::LogDebug("CRYHelper") << xyz[0] << " " << xyz[1] << " " << xyz[2] << " " 
				  << x1 << " " << x2 << " " 
				  << y1 << " " << y2 << " " 
				  << z1 << " " << z2;

	this->ProjectToBoxEdge(xyz, dxyz, x1, x2, y1, y2, z1, z2, xyzo);

	// us the floor function to make sure we don't have rounding errors making the 
	// points beyond the edge of the world
	vx = floor(xyzo[0]);
	vy = floor(xyzo[1]);
	vz = floor(xyzo[2]);
      
	// Boiler plate...
	int istatus    =  1;
	int imother1   = kCosmicRayGenerator;
      
	// Push the particle onto the stack
	std::string primary("primary");

	simb::MCParticle p(-1*(i+1),
			   pdg,
			   primary,
			   imother1,
			   m,
			   istatus);
	TLorentzVector pos(vx,vy,vz,t*1e9);// time needs to be in ns to match GENIE, etc
	TLorentzVector mom(px,py,pz,etot);
	p.AddTrajectoryPoint(pos,mom);

	mctruth.Add(p);
      } // Loop on particles in event
    
      // Check if we're done with this time sample
      if (fGen->timeSimulated()-tstart > fSampleTime) break;
    
    } // Loop on events simulated

    mctruth.SetOrigin(simb::kCosmicRay);

    // Check if this time slice passes selection criteria
    // TODO
    if (w) *w = 1.0;
  }

  ///----------------------------------------------------------------
  /// Project along a direction from a particular starting point to the
  /// edge of a box
  ///
  /// \param xyz    - The starting x,y,z location. Must be inside box.
  /// \param dxyz   - Direction vector
  /// \param xlo    - Low edge of box in x
  /// \param xhi    - Low edge of box in x
  /// \param ylo    - Low edge of box in y
  /// \param yhi    - Low edge of box in y
  /// \param zlo    - Low edge of box in z
  /// \param zhi    - Low edge of box in z
  /// \param xyzout - On output, the position at the box edge
  ///
  /// Note: It should be safe to use the same array for input and
  /// output.
  ///
  void CRYHelper::ProjectToBoxEdge(const double xyz[],
				   const double dxyz[],
				   double xlo, double xhi,
				   double ylo, double yhi,
				   double zlo, double zhi,
				   double xyzout[])
  {
    // Make sure we're inside the box!
    assert(xyz[0]>=xlo && xyz[0]<=xhi);
    assert(xyz[1]>=ylo && xyz[1]<=yhi);
    assert(xyz[2]>=zlo && xyz[2]<=zhi);
    
    // Compute the distances to the x/y/z walls
    double dx = 99.E99;
    double dy = 99.E99;
    double dz = 99.E99;
    if      (dxyz[0]>0.0) { dx = (xhi-xyz[0])/dxyz[0]; }
    else if (dxyz[0]<0.0) { dx = (xlo-xyz[0])/dxyz[0]; }
    if      (dxyz[1]>0.0) { dy = (yhi-xyz[1])/dxyz[1]; }
    else if (dxyz[1]<0.0) { dy = (ylo-xyz[1])/dxyz[1]; }
    if      (dxyz[2]>0.0) { dz = (zhi-xyz[2])/dxyz[2]; }
    else if (dxyz[2]<0.0) { dz = (zlo-xyz[2])/dxyz[2]; }
    
    // Choose the shortest distance
    double d = 0.0;
    if      (dx<dy && dx<dz) d = dx;
    else if (dy<dz && dy<dx) d = dy;
    else if (dz<dx && dz<dy) d = dz;
    
    // Make the step
    for (int i=0; i<3; ++i) {
      xyzout[i] = xyz[i] + dxyz[i]*d;
    }
  }
}
////////////////////////////////////////////////////////////////////////
