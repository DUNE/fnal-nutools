////////////////////////////////////////////////////////////////////////
// $Id: EventGeneratorTest.cxx,v 1.11 2011-10-31 14:41:40 greenc Exp $
//
// gGENIE neutrino event generator
//
// brebel@fnal.gov
//
////////////////////////////////////////////////////////////////////////
#include <cassert>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <unistd.h>

// Framework includes
#include "art/Framework/Principal/Event.h"
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Services/Optional/TFileDirectory.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "SimulationBase/simbase.h"
#include "EventGeneratorBase/test/EventGeneratorTest.h"
#include "Geometry/geo.h"
#include "EventGeneratorBase/evgenbase.h"



namespace evgen {

  //____________________________________________________________________________
  EventGeneratorTest::EventGeneratorTest(fhicl::ParameterSet const& pset)
    : fTotalGENIEPOT         ( pset.get< double      >("TotalGENIEPOT",          5e18))
    , fTotalGENIEInteractions( pset.get< double      >("TotalGENIEInteractions", 100) )
    , fTotalCRYSpills        ( pset.get< double      >("TotalCRYSpills",         1000))
    , fTopVolume             ( pset.get< std::string >("TopVolume"                   ))
  {  
    /// Create a Art Random Number engine
    int seed = (pset.get< int >("Seed", evgb::GetRandomNumberSeed()));
    createEngine(seed);
  }

  //____________________________________________________________________________
  EventGeneratorTest::~EventGeneratorTest()
  {  
  }

  //____________________________________________________________________________
  void EventGeneratorTest::beginJob()
  {  
  }

  //____________________________________________________________________________
  void EventGeneratorTest::analyze(art::Event const& evt)
  {
    mf::LogWarning("EventGeneratorTest") << "testing GENIE...";
    mf::LogWarning("EventGeneratorTest") << "\t histogram flux...";
    this->GENIEHistogramFluxTest();
    mf::LogWarning("EventGeneratorTest") << "\t \t done."
					 << "\t simple flux...";
    this->GENIESimpleFluxTest();
    mf::LogWarning("EventGeneratorTest") << "\t \t done."
					 << "\t atmo flux...";
    this->GENIEAtmoFluxTest();
    mf::LogWarning("EventGeneratorTest") << "\t \t done."
					 << "\t mono flux...";
    this->GENIEMonoFluxTest();
    mf::LogWarning("EventGeneratorTest") << "\t \t done.\n"
					 << "GENIE tests done";

    mf::LogWarning("EventGeneratorTest") << "testing CRY...";
    this->CRYTest();
    mf::LogWarning("EventGeneratorTest") << "\t CRY test done.";
  }

  //____________________________________________________________________________
  fhicl::ParameterSet EventGeneratorTest::GENIEParameterSet(std::string fluxType,
							    bool usePOTPerSpill)
  {
    // make a parameter set first so that we can pass it to the GENIEHelper
    // object we are going to make
    std::vector<double> beamCenter; 
    beamCenter.push_back(0.0); beamCenter.push_back(0.); beamCenter.push_back(0.0);

    std::vector<double> beamDir; 
    beamDir.push_back(0.); beamDir.push_back(0.); beamDir.push_back(1.);

    std::vector<int> flavors;
    if(fluxType.compare("atmo_FLUKA") == 0){
      flavors.push_back(14);
    }
    else{
      flavors.push_back(12); flavors.push_back(14); flavors.push_back(-12); flavors.push_back(-14);
    }
    

    std::vector<std::string> env;
    env.push_back("GSPLOAD");   env.push_back("gxspl-NUMI-R2.6.0.xml");
    env.push_back("GPRODMODE"); env.push_back("YES");
    env.push_back("GEVGL");     env.push_back("Default");

    double potPerSpill = 5.e13;
    double eventsPerSpill = 0;
    if(!usePOTPerSpill) eventsPerSpill = 1;

    std::vector<std::string> fluxFiles;
    fluxFiles.push_back("samples_for_geniehelper/L010z185i_lowthr_ipndshed.root");
    if(fluxType.compare("simple_flux") == 0){
      fluxFiles.clear();
      fluxFiles.push_back("samples_for_geniehelper/gsimple_NOvA-NDOS_le010z185i_20100521_RHC_lowth_s_00001.root");
    }
    else if(fluxType.compare("atmo_FLUKA") == 0){
      fluxFiles.clear();
      // at FNAL this is installed relative to in /nusoft/data/flux
      fluxFiles.push_back("atmospheric/battistoni/sdave_numu07.dat");
    }

    else if(fluxType.compare("ntuple") == 0){
      std::cerr <<"No ntuple flux file exists, bail ungracefully";
      assert(0);
    }

    fhicl::ParameterSet pset;
    pset.put("FluxType",         fluxType);
    pset.put("FluxFiles",        fluxFiles);
    pset.put("BeamName",         "numi");
    pset.put("TopVolume",        fTopVolume);
    pset.put("EventsPerSpill",   eventsPerSpill);
    pset.put("POTPerSpill",      potPerSpill);
    pset.put("BeamCenter",       beamCenter);
    pset.put("BeamDirection",    beamDir);
    pset.put("GenFlavors",       flavors);    
    pset.put("Environment",      env);
    pset.put("DetectorLocation", "NOvA-ND");

    mf::LogWarning("EventGeneratorTest") << pset.to_string();

    return pset;
  }
  
  //____________________________________________________________________________
  void EventGeneratorTest::GENIETest(fhicl::ParameterSet const& pset)
  {
    std::cout << "Test GENIE" << std::endl;

    // make the GENIEHelper object
    evgb::GENIEHelper help(pset);
    help.Initialize();

    std::cout << "GENIE initialized" << std::endl;
    
    int interactionCount = 0;

    int nspill = 0;
    int spillLimit = 0;

    // decide if we are in POT/Spill or Events/Spill mode
    double eps = pset.get<double>("EventsPerSpill");
    if(eps > 0.) spillLimit = TMath::Nint(fTotalGENIEInteractions/eps);
    else         spillLimit = 1000;

    while(nspill < spillLimit){
      ++nspill;
      while( !help.Stop() ){

	simb::MCTruth truth;
	simb::MCFlux  flux;

	if( help.Sample(truth, flux) )
	  ++interactionCount;

      } // end creation loop for this spill

    } // end loop over spills

    // count the POT used and the number of events made
    mf::LogWarning("EventGeneratorTest") << "made " << interactionCount << " interactions with " 
				      << help.TotalExposure() << " POTs";

    // compare to a simple expectation
    double totalExp = 0.;
    if(help.FluxType().compare("histogram") == 0 && pset.get<double>("EventsPerSpill") == 0){
      std::vector<TH1D*> fluxhist = help.FluxHistograms();

      if(fluxhist.size() < 1){
	mf::LogWarning("EventGeneratorTest") << "using histogram fluxes but no histograms provided!";
	assert(0);
      }
      
      // see comments in GENIEHelper::Initialize() for how this calculation was done.
      totalExp = 1.e-38*1.e-20*help.TotalHistFlux();
      totalExp *= help.TotalExposure()*help.TotalMass()/(1.67262158e-27);

      mf::LogWarning("EventGeneratorTest") << "expected " << totalExp << " interactions";
      if(fabs(interactionCount - totalExp) > 3.*TMath::Sqrt(totalExp) ){
	mf::LogWarning("EventGeneratorTest") << "generated count is more than 3 sigma off expectation";
	assert(0);
      }

    }// end if histogram fluxes


    return;

  }

  //____________________________________________________________________________
  void EventGeneratorTest::GENIEHistogramFluxTest()
  {

    mf::LogWarning("EventGeneratorTest") << "\t\t\t 1 event per spill...\n";

    // make the parameter set
    fhicl::ParameterSet pset1(this->GENIEParameterSet("histogram", false));

    this->GENIETest(pset1);

    mf::LogWarning("EventGeneratorTest") <<"\t\t\t events based on POT per spill...\n";

    fhicl::ParameterSet pset2(this->GENIEParameterSet("histogram", true));
    this->GENIETest(pset2);

    return;
  } 

  //____________________________________________________________________________
  void EventGeneratorTest::GENIESimpleFluxTest()
  {

    // make the parameter set
    mf::LogWarning("EventGeneratorTest") << "testing GENIEHelper in simple_flux mode with \n"
	      << "\t 1 event per spill...\n";

    fhicl::ParameterSet pset1 = this->GENIEParameterSet("simple_flux", false);
    this->GENIETest(pset1);

    mf::LogWarning("EventGeneratorTest") <<"\t events based on POT per spill...\n";

    fhicl::ParameterSet pset2 = this->GENIEParameterSet("simple_flux", true);
    this->GENIETest(pset2);

    return;
  } 

  //____________________________________________________________________________
  void EventGeneratorTest::GENIEMonoFluxTest()
  {

    // make the parameter set
    fhicl::ParameterSet pset1 = this->GENIEParameterSet("mono", false);

    mf::LogWarning("EventGeneratorTest") << "\t\t 1 event per spill...\n";

    this->GENIETest(pset1);

    return;

  } 

  //____________________________________________________________________________
  void EventGeneratorTest::GENIEAtmoFluxTest()
  {
    
    // make the parameter set
    fhicl::ParameterSet pset1 = this->GENIEParameterSet("atmo_FLUKA", false);
    
    mf::LogWarning("EventGeneratorTest") << "\t\t 1 event per spill...\n";
    
    this->GENIETest(pset1);
    
    return;
    
  } 


  //____________________________________________________________________________


  fhicl::ParameterSet EventGeneratorTest::CRYParameterSet()
  {
    fhicl::ParameterSet pset;
    pset.put("SampleTime",       600e-6            );
    pset.put("TimeOffset",      -30e-6             );
    pset.put("EnergyThreshold",  50e-3             );
    pset.put("Latitude",         "latitude 41.8 "  );
    pset.put("Altitude",         "altitude 0 "     );
    pset.put("SubBoxLength",     "subboxLength 75 ");

    mf::LogWarning("EventGeneratorTest") << pset.to_string();

    return pset;
  }

  //____________________________________________________________________________
  void EventGeneratorTest::CRYTest()
  {
    // make the parameter set
    fhicl::ParameterSet pset = this->CRYParameterSet();

    // get the random number generator service and make some CLHEP generators
    art::ServiceHandle<art::RandomNumberGenerator> rng;
    CLHEP::HepRandomEngine& engine = rng->getEngine();

    // make the CRYHelper
    evgb::CRYHelper help(pset, engine);

    int    nspill         = 0;
    double avPartPerSpill = 0.;
    double avPartIntersectPerSpill = 0.;
    double avMuonIntersectPerSpill = 0.;
    double avEIntersectPerSpill    = 0.;
    while(nspill < TMath::Nint(fTotalCRYSpills) ){

      simb::MCTruth mct;
      
      help.Sample(mct, 0);

      avPartPerSpill += mct.NParticles();

      // now check to see if the particles go through the 
      // detector enclosure
      for(int p = 0; p < mct.NParticles(); ++p){
	if(this->IntersectsDetector(mct.GetParticle(p)) ){
	   avPartIntersectPerSpill += 1.;
	   if(TMath::Abs(mct.GetParticle(p).PdgCode()) == 13)
	     avMuonIntersectPerSpill += 1.;
	   else if(TMath::Abs(mct.GetParticle(p).PdgCode()) == 11)
	     avEIntersectPerSpill += 1.;
	}
      }
    


      ++nspill;
    }

    mf::LogWarning("EventGeneratorTest") << "there are " << avPartPerSpill/(1.*nspill)
					 << " cosmic rays made per spill \n"
					 << avPartIntersectPerSpill/(1.*nspill)
					 << " intersect the detector per spill"
					 << "\n\t " 
					 << avMuonIntersectPerSpill/(1.*nspill)
					 << " muons \n\t"
					 << avEIntersectPerSpill/(1.*nspill)
					 << " electrons";


    return;
  }

  //____________________________________________________________________________
  bool EventGeneratorTest::IntersectsDetector(simb::MCParticle const& part)
  {

    // get the extent of the detector from the geometry
    art::ServiceHandle<geo::Geometry> geo;
    double halfwidth  = geo->DetHalfWidth();
    double halfheight = geo->DetHalfHeight();
    double detlength  = geo->DetLength();

    // the particle's initial position and momentum
    TLorentzVector pos = part.Position();
    TLorentzVector mom = part.Momentum();

    if(TMath::Abs(mom.P()) == 0){
      mf::LogWarning("EventGeneratorTest") << "particle has no momentum!!! bail";
      return false;
    }

    double xyz[3] = {0.};

    // Checking intersection with 6 planes

    // 1. Check intersection with the y = +halfheight plane
    this->ProjectToSurface(pos, mom, 1, halfheight, xyz);

    if( TMath::Abs(xyz[0]) <= halfwidth 
	&& xyz[2] > 0.
	&& TMath::Abs(xyz[2]) <= detlength ) return true;
      

    // 2. Check intersection with the +x plane
    this->ProjectToSurface(pos, mom, 0, halfwidth, xyz);

    if( TMath::Abs(xyz[1]) <= halfheight 
	&& xyz[2] > 0.
	&& TMath::Abs(xyz[2]) <= detlength ) return true;

    // 3. Check intersection with the -x plane
    this->ProjectToSurface(pos, mom, 0, -halfwidth, xyz);

    if( TMath::Abs(xyz[1]) <= halfheight 
	&& xyz[2] > 0.
	&& TMath::Abs(xyz[2]) <= detlength ) return true;

    // 4. Check intersection with the z=0 plane
    this->ProjectToSurface(pos, mom, 2, 0., xyz);

    if( TMath::Abs(xyz[0]) <= halfwidth 
	&& TMath::Abs(xyz[1]) <= halfheight ) return true;

    // 5. Check intersection with the z=detlength plane
    this->ProjectToSurface(pos, mom, 2, detlength, xyz);

    if( TMath::Abs(xyz[0]) <= halfwidth 
	&& TMath::Abs(xyz[1]) <= halfheight ) return true;

    return false;
  }

  //____________________________________________________________________________
  void EventGeneratorTest::ProjectToSurface(TLorentzVector pos,
					    TLorentzVector mom,
					    int axis,
					    double surfaceLoc,
					    double* xyz)
  {
    double momDir = 0.;
    double posDir = 0.;
    if(axis == 0){
      momDir = mom.Px();
      posDir = pos.X();
    }
    else if(axis == 1){
      momDir = mom.Py();
      posDir = pos.X();
    }
    else if(axis == 2){
      momDir = mom.Pz();
      posDir = pos.X();
    }
    
    double ddS        = (momDir/mom.P());
    double length1Dim = (posDir - surfaceLoc);

    if(TMath::Abs(ddS) > 0.){
      length1Dim /= ddS;
      xyz[0] = pos.X() + length1Dim*mom.Px()/mom.P();
      xyz[1] = pos.Y() + length1Dim*mom.Py()/mom.P();
      xyz[2] = pos.Z() + length1Dim*mom.Pz()/mom.P();
    }

    return;
  }

}// namespace
