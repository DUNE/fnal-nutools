////////////////////////////////////////////////////////////////////////
/// \file  GENIEHelper.h
/// \brief Wrapper for generating neutrino interactions with GENIE
///
/// \version $Id: GENIEHelper.cxx,v 1.22 2011-08-15 18:53:29 brebel Exp $
/// \author  brebel@fnal.gov
/// \update 2010/3/4 Sarah Budd added simple_flux
////////////////////////////////////////////////////////////////////////

// C/C++ includes
#include <math.h>
#include <map>
#include <cassert>

//ROOT includes
#include "TH1.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TCollection.h"
#include "TSystem.h"
#include "TString.h"
#include "TRandom.h" //needed for gRandom to be defined
#include "TRegexp.h"

//GENIE includes
#include "Conventions/Units.h"
#include "EVGCore/EventRecord.h"
#include "EVGDrivers/GMCJDriver.h"
#include "GHEP/GHepUtils.h"
#include "FluxDrivers/GCylindTH1Flux.h"
#include "FluxDrivers/GMonoEnergeticFlux.h"
#include "FluxDrivers/GNuMIFlux.h"
#ifndef  MISSING_GSIMPLENTPFLUX
#include "FluxDrivers/GSimpleNtpFlux.h"
#endif
#include "Geo/ROOTGeomAnalyzer.h"
#include "Interaction/InitialState.h"
#include "Interaction/Interaction.h"
#include "Interaction/Kinematics.h"
#include "Interaction/KPhaseSpace.h"
#include "Interaction/ProcessInfo.h"
#include "Interaction/XclsTag.h"
#include "GHEP/GHepParticle.h"
#include "PDG/PDGCodeList.h"

// currently NuTools, but eventually GENIE
#ifdef BLENDER_IN_GENIE
#include "FluxDrivers/GFluxBlender.h"
#include "FluxDrivers/GFlavorMixerI.h"
#include "FluxDrivers/GFlavorMap.h"
#else
#include "EventGeneratorBase/GFluxBlender.h"
#include "EventGeneratorBase/GFlavorMixerI.h"
#include "EventGeneratorBase/GFlavorMap.h"
#endif

//NuTools includes
#include "EventGeneratorBase/evgenbase.h"
#include "SimulationBase/simbase.h"

//experiment includes - assumes every experiment has a Geometry package
#include "Geometry/geo.h"

// Framework includes
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "cetlib/search_path.h"
#include "cetlib/getenv.h"
#include "cetlib/split_path.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"


namespace evgb {

  static const int kNue      = 0;
  static const int kNueBar   = 1;
  static const int kNuMu     = 2;
  static const int kNuMuBar  = 3;
  static const int kNuTau    = 4;
  static const int kNuTauBar = 5;

  //--------------------------------------------------
  GENIEHelper::GENIEHelper(fhicl::ParameterSet const& pset) :
    fGeomD             (0),
    fFluxD             (0),
    fFluxD2GMCJD       (0),
    fDriver            (0),
    fFluxType          (pset.get< std::string              >("FluxType")               ),
    fBeamName          (pset.get< std::string              >("BeamName")               ),
    fTopVolume         (pset.get< std::string              >("TopVolume")              ),
    fWorldVolume       ("volWorld"),						         
    fDetLocation       (pset.get< std::string              >("DetectorLocation"     )  ),
    fTargetA           (pset.get< double                   >("TargetA",          12.)  ),
    fEventsPerSpill    (pset.get< double                   >("EventsPerSpill",   0)    ),
    fPOTPerSpill       (pset.get< double                   >("POTPerSpill",      5.e13)),
    fHistEventsPerSpill(0.),
    fMonoEnergy        (pset.get< double                   >("MonoEnergy",       2.0)  ),
    fPOTUsed           (0.),							         
    fBeamRadius        (pset.get< double                   >("BeamRadius",       3.0)  ),
    fSurroundingMass   (pset.get< double                   >("SurroundingMass",  0.)   ),
    fGlobalTimeOffset  (pset.get< double                   >("GlobalTimeOffset", 1.e4) ),
    fRandomTimeOffset  (pset.get< double                   >("RandomTimeOffset", 1.e4) ),
    fZCutOff           (pset.get< double                   >("ZCutOff",          2.5)  ),
    fEnvironment       (pset.get< std::vector<std::string> >("Environment")            ),
    fMixerConfig       (pset.get< std::string              >("MixerConfig",     "none")),
    fMixerBaseline     (pset.get< double                   >("MixerBaseline",    0.)   ),
    fDebugFlags        (pset.get< unsigned int             >("DebugFlags",       0)    ) 
  {
    std::vector<double> beamCenter   (pset.get< std::vector<double> >("BeamCenter")   );
    std::vector<double> beamDirection(pset.get< std::vector<double> >("BeamDirection"));
    fBeamCenter.SetXYZ(beamCenter[0], beamCenter[1], beamCenter[2]);
    fBeamDirection.SetXYZ(beamDirection[0], beamDirection[1], beamDirection[2]);

    std::vector<int> genFlavors(pset.get< std::vector<int> >("GenFlavors"));

    for (unsigned int i = 0; i < genFlavors.size(); ++i) fGenFlavors.insert(genFlavors[i]);

    // need to find the right alternative in FW_SEARCH_PATH to find 
    // the flux files without attempting to expand any actual wildcard
    // that might be in the name; the cet::search_path class seemed
    // like it could help in this.  In the end, it can't deal with
    // wildcarding in the way we want.
    /* was:
    cet::search_path sp("FW_SEARCH_PATH");
    sp.find_file(pset.get< std::string>("FluxFile"), fFluxFile);
    */
    FindFluxPath(pset.get<std::string>("FluxFile"));

    cet::search_path sp("FW_SEARCH_PATH");
    // set the environment, the vector should come in pairs of variable name, then value
    TString junk = "";
    junk += pset.get< int >("RandomSeed", evgb::GetRandomNumberSeed());
    std::string seed(junk);
    fEnvironment.push_back("GSEED");
    fEnvironment.push_back(seed);

    for(unsigned int i = 0; i < fEnvironment.size(); i += 2){
      if(fEnvironment[i].compare("GSPLOAD") == 0){
	std::string fullpath;
	sp.find_file(fEnvironment[i+1], fullpath);
	fEnvironment[i+1] = fullpath;
      }
      gSystem->Setenv(fEnvironment[i].c_str(), fEnvironment[i+1].c_str());
      mf::LogInfo("GENIEHelper") << "setting GENIE environment " << fEnvironment[i]
				 << " to " << fEnvironment[i+1];
    }

    // make the histograms
    if(fFluxType.compare("histogram") == 0){
      mf::LogInfo("GENIEHelper") << "setting beam direction and center at "
				 << fBeamDirection.X() << " " << fBeamDirection.Y() << " " << fBeamDirection.Z()
				 << " (" << fBeamCenter.X() << "," << fBeamCenter.Y() << "," << fBeamCenter.Z()
				 << ") with radius " << fBeamRadius;

      TDirectory *savedir = gDirectory;
    
      fFluxHistograms.clear();
      TFile tf(fFluxFile.c_str());
      tf.ls();

      for(std::set<int>::iterator flvitr = fGenFlavors.begin(); flvitr != fGenFlavors.end(); flvitr++){
	if(*flvitr ==  12) fFluxHistograms.push_back(dynamic_cast<TH1D *>(tf.Get("nue")));
	if(*flvitr == -12) fFluxHistograms.push_back(dynamic_cast<TH1D *>(tf.Get("nuebar")));
	if(*flvitr ==  14) fFluxHistograms.push_back(dynamic_cast<TH1D *>(tf.Get("numu")));
	if(*flvitr == -14) fFluxHistograms.push_back(dynamic_cast<TH1D *>(tf.Get("numubar")));
	if(*flvitr ==  16) fFluxHistograms.push_back(dynamic_cast<TH1D *>(tf.Get("nutau")));
	if(*flvitr == -16) fFluxHistograms.push_back(dynamic_cast<TH1D *>(tf.Get("nutaubar")));
      }

      for(unsigned int i = 0; i < fFluxHistograms.size(); ++i){
	fFluxHistograms[i]->SetDirectory(savedir);
	fTotalHistFlux += fFluxHistograms[i]->Integral();
      }

      mf::LogInfo("GENIEHelper") << "total histogram flux over desired flavors = " 
				 << fTotalHistFlux;

    }//end if getting fluxes from histograms

    std::string flvlist;
    for(std::set<int>::iterator itr = fGenFlavors.begin(); itr != fGenFlavors.end(); itr++)
      flvlist += Form(" %d",*itr);

    mf::LogInfo("GENIEHelper") << "Generating flux with the following flavors: " << flvlist;

    if(fFluxType.compare("mono")==0){
      fEventsPerSpill = 1;
      mf::LogInfo("GENIEHelper") << "Generating monoenergetic (" << fMonoEnergy 
				 << " GeV) neutrinos ";
    }

    if(fEventsPerSpill != 0)
      mf::LogInfo("GENIEHelper") << "Generating " << fEventsPerSpill 
				 << " events for each spill";
    else
      mf::LogInfo("GENIEHelper") << "Using " << fPOTPerSpill << " pot for each spill";

    return;
  }

  //--------------------------------------------------
  GENIEHelper::~GENIEHelper()
  {
  }

  //--------------------------------------------------
  double GENIEHelper::TotalHistFlux() 
  {
    if(   fFluxType.compare("ntuple")       == 0
       || fFluxType.compare("mono")         == 0 
       || fFluxType.compare("simple_flux" ) == 0 ) return -999.;

    return fTotalHistFlux;
  }

  //--------------------------------------------------
  void GENIEHelper::Initialize()
  {

    // initialize the Geometry and Flux drivers
    InitializeGeometry();
    InitializeFluxDriver();
    fDriver = new genie::GMCJDriver();
    fDriver->UseFluxDriver(fFluxD2GMCJD);
    fDriver->UseGeomAnalyzer(fGeomD);

    // turn on following line to speed up driver initialization
    //driver->UseMaxPathLengths(***supply some xml file name***);

    fDriver->Configure();
    fDriver->UseSplines();
    fDriver->ForceSingleProbScale();

    if(fFluxType.compare("histogram") == 0 && fEventsPerSpill < 0.01){
      // fluxes are assumed to be given in units of neutrinos/cm^2/1e20POT/energy 
      // integral over all fluxes removes energy dependence
      // histograms should have bin width that reflects the value of the /energy bit
      // ie if /energy = /50MeV then the bin width should be 50 MeV
      
      // determine product of pot/spill, mass, and cross section
      // events = flux * pot * 10^-38 cm^2 (xsec) * (mass detector (in kg) / nucleon mass (in kg))
      fXSecMassPOT  = 1.e-38*1.e-20;
      fXSecMassPOT *= fPOTPerSpill*(fDetectorMass+fSurroundingMass)/(1.67262158e-27); 

      mf::LogInfo("GENIEHelper") << "Number of events per spill will be based on poisson mean of "
				 << fXSecMassPOT*fTotalHistFlux;

      fHistEventsPerSpill = gRandom->Poisson(fXSecMassPOT*fTotalHistFlux);
    }

    // set the pot/event counters to zero
    fSpillTotal = 0.;
    fPOTUsed   = 0.;

    return;
  }

  //--------------------------------------------------
  void GENIEHelper::InitializeGeometry()
  {
    art::ServiceHandle<geo::Geometry> geo;
    TGeoManager* rootgeom = geo->ROOTGeoManager();
    genie::geometry::ROOTGeomAnalyzer *rgeom = 
      new genie::geometry::ROOTGeomAnalyzer(rootgeom);

    // get the world volume name from the geometry
    fWorldVolume = geo->ROOTGeoManager()->GetTopVolume()->GetName();

    // the detector geometry uses cgs units.
    rgeom->SetLengthUnits(genie::units::centimeter);
    rgeom->SetDensityUnits(genie::units::gram_centimeter3);
    rgeom->SetTopVolName(fTopVolume.c_str());
    rgeom->SetMixtureWeightsSum(1.);

    //  casting to the GENIE geometry driver interface
    fGeomD        = rgeom; // dynamic_cast<genie::GeomAnalyzerI *>(rgeom);
    fDetLength    = geo->DetLength()*0.01;  
    fDetectorMass = geo->TotalMass(fTopVolume.c_str());

    return;
  }

  //--------------------------------------------------
  void GENIEHelper::InitializeFluxDriver()
  {

    if(fFluxType.compare("ntuple") == 0){

      genie::flux::GNuMIFlux* numiFlux = new genie::flux::GNuMIFlux();
      numiFlux->LoadBeamSimData(fFluxFile, fDetLocation);
    
      // initialize to only use neutrino flavors requested by user
      genie::PDGCodeList probes;
      for(std::set<int>::iterator flvitr = fGenFlavors.begin(); flvitr != fGenFlavors.end(); flvitr++)
        probes.push_back(*flvitr);
      numiFlux->SetFluxParticles(probes);

      // set the number of cycles to run
      // +++++++++this is stupid - to really set it i have to get a 
      // value from the MCJDriver and i am not even sure what i have 
      // below is approximately correct.
      // for now just run on a set number of events that is kept track of 
      // in the sample method
      //  numiFlux->SetNumOfCycles(int(fPOT/fFluxNormalization));
    
      fFluxD = numiFlux; // dynamic_cast<genie::GFluxI *>(numiFlux);
    } //end if using ntuple flux files
    else if(fFluxType.compare("simple_flux")==0){

#ifdef MISSING_GSIMPLENTPFLUX
      mf::LogError("GENIEHelper") << "Not built with GSimpleNtpFlux enabled";
      assert(0);
#else
      genie::flux::GSimpleNtpFlux* simpleFlux = 
        new genie::flux::GSimpleNtpFlux();
      simpleFlux->LoadBeamSimData(fFluxFile, fDetLocation);

      // initialize to only use neutrino flavors requested by user
      genie::PDGCodeList probes;
      for(std::set<int>::iterator flvitr = fGenFlavors.begin(); flvitr != fGenFlavors.end(); flvitr++)
        probes.push_back(*flvitr);
      simpleFlux->SetFluxParticles(probes);

      fFluxD = simpleFlux; // dynamic_cast<genie::GFluxI *>(simpleFlux);
#endif    
    
    } //end if using simple_flux flux files
    else if(fFluxType.compare("histogram") == 0){

      genie::flux::GCylindTH1Flux* histFlux = new genie::flux::GCylindTH1Flux();
    
      // now add the different fluxes - fluxes were added to the vector in the same 
      // order that the flavors appear in fGenFlavors
      int ctr = 0;
      for(std::set<int>::iterator i = fGenFlavors.begin(); i != fGenFlavors.end(); i++){
	histFlux->AddEnergySpectrum(*i, fFluxHistograms[ctr]);
	++ctr;
      } //end loop to add flux histograms to driver

      histFlux->SetNuDirection(fBeamDirection);
      histFlux->SetBeamSpot(fBeamCenter);
      histFlux->SetTransverseRadius(fBeamRadius);
    
      fFluxD = histFlux; // dynamic_cast<genie::GFluxI *>(histFlux);
    } //end if using a histogram
    else if(fFluxType.compare("mono") == 0){

      // weight each species equally in the generation
      double weight = 1./(1.*fGenFlavors.size());
      //make a map of pdg to weight codes
      std::map<int, double> pdgwmap;
      for(std::set<int>::iterator i = fGenFlavors.begin(); i != fGenFlavors.end(); i++)
	pdgwmap[*i] = weight;

      genie::flux::GMonoEnergeticFlux *monoflux = new genie::flux::GMonoEnergeticFlux(fMonoEnergy, pdgwmap);
      monoflux->SetDirectionCos(fBeamDirection.X(), fBeamDirection.Y(), fBeamDirection.Z());
      monoflux->SetRayOrigin(fBeamCenter.X(), fBeamCenter.Y(), fBeamCenter.Z());
      fFluxD = monoflux; // dynamic_cast<genie::GFluxI *>(monoflux);
    } //end if using monoenergetic beam

    //
    // Is the user asking to do flavor mixing?
    //
    fFluxD2GMCJD = fFluxD;  // default: genie's GMCJDriver uses the bare flux generator
    if( fMixerConfig.find_first_not_of(" \t\n") != 0) // trim any leading whitespace
        fMixerConfig.erase( 0, fMixerConfig.find_first_not_of(" \n")  );
    std::string keyword = fMixerConfig.substr(0,fMixerConfig.find_first_of(" "));
    if ( keyword != "none" ) {
      // Wrap the true flux driver up in the adapter to allow flavor mixing
      genie::flux::GFlavorMixerI* mixer = 0;
      // here is where we map MixerConfig string keyword to actual class
      if ( keyword == "map" || keyword == "swap" || keyword == "fixedfrac" )
        mixer = new genie::flux::GFlavorMap();
      //-- if there are alternative mixers the map from keyword => class goes here
      // configure the mixer
      if ( mixer ) mixer->Config(fMixerConfig);
      else {
	mf::LogWarning("GENIEHelper") << "GENIEHelper MixerConfig keyword was \"" << keyword
				      << "\" but that did not map to a class" << std::endl
				      << "GFluxBlender in use, but no mixer";
        
      }
      // 
      genie::GFluxI* realFluxD = fFluxD;
      genie::flux::GFluxBlender* blender = new genie::flux::GFluxBlender();
      blender->SetBaselineDist(fMixerBaseline);
      blender->AdoptFluxGenerator(realFluxD);
      blender->AdoptFlavorMixer(mixer);
      fFluxD2GMCJD = blender;
      if ( fDebugFlags & 0x01 ) {
        if ( mixer ) mixer->PrintConfig();
        blender->PrintConfig();
        std::cout << std::flush;
      }
    }

    return;
  }

  //--------------------------------------------------
  bool GENIEHelper::Stop()
  {
    //   std::cout << "in GENIEHelper::Stop(), fEventsPerSpill = " << fEventsPerSpill
    // 	    << " fPOTPerSpill = " << fPOTPerSpill << " fSpillTotal = " << fSpillTotal 
    // 	    << " fHistEventsPerSpill = " << fHistEventsPerSpill << std::endl;

    // determine if we should keep throwing neutrinos for 
    // this spill or move on
    if(fEventsPerSpill > 0){
      if(fSpillTotal < fEventsPerSpill) 
	return false;
    }
    else{
      if( ( fFluxType.compare("ntuple")      == 0 || 
            fFluxType.compare("simple_flux") == 0    ) && 
          fSpillTotal < fPOTPerSpill) return false;
      else if(fFluxType.compare("histogram") == 0){
	if(fSpillTotal < fHistEventsPerSpill) return false;
	else fSpillTotal = fPOTPerSpill;
      }
    }

    // made it to here, means need to reset the counters
    fPOTUsed += fSpillTotal;
    fSpillTotal = 0.;
    fHistEventsPerSpill = gRandom->Poisson(fXSecMassPOT*fTotalHistFlux);
    return true;
  }

  //--------------------------------------------------
  bool GENIEHelper::Sample(simb::MCTruth &truth,
			   simb::MCFlux  &flux)
  {
    // set the top volume for the geometry
    art::ServiceHandle<geo::Geometry> geo;
    geo->ROOTGeoManager()->SetTopVolume(geo->ROOTGeoManager()->FindVolumeFast(fTopVolume.c_str()));
    
    genie::EventRecord* record = fDriver->GenerateEvent();

    // now check if we produced a viable event record
    bool viableInteraction = true;
    if ( !record ) viableInteraction = false;

    // update the spill total information, then check to see 
    // if we got an event record that was valid

    // pack the flux information
    if(fFluxType.compare("ntuple") == 0){
      if(fEventsPerSpill == 0.) 
	fSpillTotal += (dynamic_cast<genie::flux::GNuMIFlux *>(fFluxD)->UsedPOTs()/fDriver->GlobProbScale() - fPOTUsed);
      flux.fFluxType = simb::kNtuple;
      PackNuMIFlux(flux);
    }
    else if ( fFluxType.compare("simple_flux")==0 ) {
 
#ifdef MISSING_GSIMPLENTPFLUX
      mf::LogError("GENIEHelper") << "Not built with GSimpleNtpFlux enabled";
      assert(0);
#else
      // pack the flux information
      if(fEventsPerSpill == 0.)
	fSpillTotal += (dynamic_cast<genie::flux::GSimpleNtpFlux *>(fFluxD)->UsedPOTs()/fDriver->GlobProbScale() - fPOTUsed);
#endif
      flux.fFluxType = simb::kSimple_Flux;
      PackSimpleFlux(flux);
    }

    // if no interaction generated return false
    if(!viableInteraction) return false;
    
    TLorentzVector *vertex = record->Vertex();
    if(vertex->Z() > fDetLength + fZCutOff) return false;

    // fill the MC truth information as we have a good interaction
    PackMCTruth(record,truth); 
    
    // check to see if we are using flux ntuples but want to 
    // make n events per spill
    if(fEventsPerSpill > 0 &&
       (fFluxType.compare("ntuple") == 0
	|| fFluxType.compare("simple_flux") == 0)
       ) fSpillTotal += 1.;

    // now check if using either histogram or mono fluxes, using
    // either n events per spill or basing events on POT per spill for the
    // histogram case
    if(fFluxType.compare("histogram") == 0){
      // set the flag in the parent object that says the 
      // fluxes came from histograms and fill related values
      flux.fFluxType = simb::kHistPlusFocus;

      // save the fluxes - fluxes were added to the vector in the same 
      // order that the flavors appear in fGenFlavors
      int ctr = 0;
      int bin = fFluxHistograms[0]->FindBin(truth.GetNeutrino().Nu().E());
      std::vector<double> fluxes(6, 0.);
      for(std::set<int>::iterator i = fGenFlavors.begin(); i != fGenFlavors.end(); i++){
	if(*i ==  12) fluxes[kNue]      = fFluxHistograms[ctr]->GetBinContent(bin);
	if(*i == -12) fluxes[kNueBar]   = fFluxHistograms[ctr]->GetBinContent(bin);
	if(*i ==  14) fluxes[kNuMu]     = fFluxHistograms[ctr]->GetBinContent(bin);
	if(*i == -14) fluxes[kNuMuBar]  = fFluxHistograms[ctr]->GetBinContent(bin);
	if(*i ==  16) fluxes[kNuTau]    = fFluxHistograms[ctr]->GetBinContent(bin);
	if(*i == -16) fluxes[kNuTauBar] = fFluxHistograms[ctr]->GetBinContent(bin);
	++ctr;
      }

      // get the flux for each neutrino flavor of this energy
      flux.SetFluxGen(fluxes[kNue],   fluxes[kNueBar],
		      fluxes[kNuMu],  fluxes[kNuMuBar],
		      fluxes[kNuTau], fluxes[kNuTauBar]);
    
      fSpillTotal += 1.;
    }
    else if(fFluxType.compare("mono") == 0){
      fSpillTotal += 1.;    
    }

    // fill these after the Pack[NuMI|Simple]Flux because those
    // will Reset() the values at the start
    TLorentzVector nuray_pos = fFluxD->Position();
    TVector3 ray2vtx = nuray_pos.Vect() - vertex->Vect();
    flux.fgenx    = nuray_pos.X();
    flux.fgeny    = nuray_pos.Y();
    flux.fgenz    = nuray_pos.Z();
    flux.fgen2vtx = ray2vtx.Mag();

    genie::flux::GFluxBlender* blender = 
      dynamic_cast<genie::flux::GFluxBlender*>(fFluxD2GMCJD);
    if ( blender ) { 
      flux.fdk2gen = blender->TravelDist();
      // / if mixing flavors print the state of the blender
      if ( fDebugFlags & 0x02 ) blender->PrintState();
    }

    if ( fDebugFlags & 0x04 ) {
      LOG_DEBUG("GENIEHelper") << "vertex loc " << vertex->X() << "," 
			       << vertex->Y() << "," << vertex->Z() << std::endl 
			       << "flux ray start " << nuray_pos.X() << ","
			       << nuray_pos.Y() << "," << nuray_pos.Z() << std::endl
			       << " ray2vtx = " << flux.fgen2vtx
			       << " dk2ray = " << flux.fdk2gen;
    }

    // set the top volume of the geometry back to the world volume
    geo->ROOTGeoManager()->SetTopVolume(geo->ROOTGeoManager()->FindVolumeFast(fWorldVolume.c_str()));
					
    return true;
  }

  //--------------------------------------------------
  void GENIEHelper::PackNuMIFlux(simb::MCFlux &flux)
  {
    flux.Reset();

    // cast the fFluxD pointer to be of the right type
    genie::flux::GNuMIFlux *gnf = dynamic_cast<genie::flux::GNuMIFlux *>(fFluxD);
    const genie::flux::GNuMIFluxPassThroughInfo& nflux = gnf->PassThroughInfo();

    // check the particle codes and the units passed through
    //  nflux.pcodes: 0=original GEANT particle codes, 1=converted to PDG
    //  nflux.units:  0=original GEANT cm, 1=meters
    if(nflux.pcodes != 1 && nflux.units != 0)
      mf::LogWarning("GENIEHelper") << "either wrong particle codes or units "
				    << "from flux object - beware!!";

    // maintained variable names from gnumi ntuples
    // see http://www.hep.utexas.edu/~zarko/wwwgnumi/v19/[/v19/output_gnumi.html]

    flux.frun      = nflux.run;
    flux.fevtno    = nflux.evtno;
    flux.fndxdz    = nflux.ndxdz;
    flux.fndydz    = nflux.ndydz;
    flux.fnpz      = nflux.npz;
    flux.fnenergy  = nflux.nenergy;
    flux.fndxdznea = nflux.ndxdznea;
    flux.fndydznea = nflux.ndydznea;
    flux.fnenergyn = nflux.nenergyn;
    flux.fnwtnear  = nflux.nwtnear;
    flux.fndxdzfar = nflux.ndxdzfar;
    flux.fndydzfar = nflux.ndydzfar;
    flux.fnenergyf = nflux.nenergyf;
    flux.fnwtfar   = nflux.nwtfar;
    flux.fnorig    = nflux.norig;
    flux.fndecay   = nflux.ndecay;
    flux.fntype    = nflux.ntype;
    flux.fvx       = nflux.vx;
    flux.fvy       = nflux.vy;
    flux.fvz       = nflux.vz;
    flux.fpdpx     = nflux.pdpx;
    flux.fpdpy     = nflux.pdpy;
    flux.fpdpz     = nflux.pdpz;
    flux.fppdxdz   = nflux.ppdxdz;
    flux.fppdydz   = nflux.ppdydz;
    flux.fpppz     = nflux.pppz;
    flux.fppenergy = nflux.ppenergy;
    flux.fppmedium = nflux.ppmedium;
    flux.fptype    = nflux.ptype;     // converted to PDG
    flux.fppvx     = nflux.ppvx;
    flux.fppvy     = nflux.ppvy;
    flux.fppvz     = nflux.ppvz;
    flux.fmuparpx  = nflux.muparpx;
    flux.fmuparpy  = nflux.muparpy;
    flux.fmuparpz  = nflux.muparpz;
    flux.fmupare   = nflux.mupare;
    flux.fnecm     = nflux.necm;
    flux.fnimpwt   = nflux.nimpwt;
    flux.fxpoint   = nflux.xpoint;
    flux.fypoint   = nflux.ypoint;
    flux.fzpoint   = nflux.zpoint;
    flux.ftvx      = nflux.tvx;
    flux.ftvy      = nflux.tvy;
    flux.ftvz      = nflux.tvz;
    flux.ftpx      = nflux.tpx;
    flux.ftpy      = nflux.tpy;
    flux.ftpz      = nflux.tpz;
    flux.ftptype   = nflux.tptype;   // converted to PDG
    flux.ftgen     = nflux.tgen;
    flux.ftgptype  = nflux.tgptype;  // converted to PDG
    flux.ftgppx    = nflux.tgppx;
    flux.ftgppy    = nflux.tgppy;
    flux.ftgppz    = nflux.tgppz;
    flux.ftprivx   = nflux.tprivx;
    flux.ftprivy   = nflux.tprivy;
    flux.ftprivz   = nflux.tprivz;
    flux.fbeamx    = nflux.beamx;
    flux.fbeamy    = nflux.beamy;
    flux.fbeamz    = nflux.beamz;
    flux.fbeampx   = nflux.beampx;
    flux.fbeampy   = nflux.beampy;
    flux.fbeampz   = nflux.beampz;    

    flux.fdk2gen   = gnf->GetDecayDist();

    return;
  }

  //--------------------------------------------------
  void GENIEHelper::PackMCTruth(genie::EventRecord *record,
				simb::MCTruth &truth)
  {

    TLorentzVector *vertex = record->Vertex();

    // get the Interaction object from the record - this is the object
    // that talks to the event information objects and is in m
    genie::Interaction *inter = record->Summary();
  
    // get the different components making up the interaction
    const genie::InitialState &initState  = inter->InitState();
    const genie::ProcessInfo  &procInfo   = inter->ProcInfo();
    const genie::Kinematics   &kine       = inter->Kine();
    //const genie::XclsTag      &exclTag    = inter->ExclTag();
    //const genie::KPhaseSpace  &phaseSpace = inter->PhaseSpace();

    //choose a spill time (ns) to shift the vertex times by:

    double spillTime = fGlobalTimeOffset + gRandom->Uniform()*fRandomTimeOffset;

    // add the particles from the interaction
    TIter partitr(record);
    genie::GHepParticle *part = 0;
    // GHepParticles return units of GeV/c for p.  the V_i are all in fermis
    // and are relative to the center of the struck nucleus.
    // add the vertex X/Y/Z to the V_i for status codes 0 and 1
    int trackid = 0;
    std::string primary("primary");

    while( (part = dynamic_cast<genie::GHepParticle *>(partitr.Next())) ){
      --trackid;
      simb::MCParticle tpart(trackid, 
			     part->Pdg(), 
			     primary, 
			     part->FirstMother(), 
			     part->Mass(), 
			     part->Status());

      double vtx[4] = {part->Vx(), part->Vy(), part->Vz(), part->Vt()};
      
      // set the vertex location for the neutrino, nucleus and everything
      // that is to be tracked.  vertex returns values in meters.
      if(part->Status() == 0 || part->Status() == 1){
	vtx[0] = 100.*(part->Vx()*1.e-15 + vertex->X());
	vtx[1] = 100.*(part->Vy()*1.e-15 + vertex->Y());
	vtx[2] = 100.*(part->Vz()*1.e-15 + vertex->Z());
	vtx[3] = part->Vt() + spillTime;
      }

      TLorentzVector pos(vtx[0], vtx[1], vtx[2], vtx[3]);
      TLorentzVector mom(part->Px(), part->Py(), part->Pz(), part->E());
      tpart.AddTrajectoryPoint(pos,mom);

      truth.Add(tpart);
        
    }// end loop to convert GHepParticles to MCParticles

    // is the interaction NC or CC
    int CCNC = simb::kCC;
    if(procInfo.IsWeakNC()) CCNC = simb::kNC;

    // what is the interaction type
    int mode = simb::kQE;
    if(procInfo.IsDeepInelastic()) mode = simb::kDIS;
    else if(procInfo.IsResonant()) mode = simb::kRes;
    else if(procInfo.IsCoherent()) mode = simb::kCoh;
    
    int itype = simb::kNuanceOffset + genie::utils::ghep::NuanceReactionCode(record);

    // set the neutrino information in MCTruth
    truth.SetOrigin(simb::kBeamNeutrino);
    truth.SetNeutrino(CCNC, mode, itype,
		      initState.Tgt().Pdg(), 
		      initState.Tgt().HitNucPdg(), 
		      initState.Tgt().HitQrkPdg(),
		      kine.W(true), kine.x(true), kine.y(true), kine.Q2(true));


    return;
  }

  void GENIEHelper::PackSimpleFlux(simb::MCFlux &flux)
  {
#ifdef MISSING_GSIMPLENTPFLUX
    mf::LogError("GENIEHelper") << "Not built with GSimpleNtpFlux enabled";
    assert(0);
#else
    flux.Reset();

    // cast the fFluxD pointer to be of the right type
    genie::flux::GSimpleNtpFlux *gsf = dynamic_cast<genie::flux::GSimpleNtpFlux *>(fFluxD);
    
    // maintained variable names from gnumi ntuples
    // see http://www.hep.utexas.edu/~zarko/wwwgnumi/v19/[/v19/output_gnumi.html]
    
    const genie::flux::GSimpleNtpEntry* nflux_entry = gsf->GetCurrentEntry();
    const genie::flux::GSimpleNtpNuMI*  nflux_numi  = gsf->GetCurrentNuMI();
    //const genie::flux::GSimpleNtpMeta*  nflux_meta  = gsf->GetCurrentMeta();
  
    flux.fntype  = nflux_entry->pdg;
    flux.fnimpwt = nflux_entry->wgt;

    if ( nflux_numi ) {
      flux.frun      = nflux_numi->run;
      flux.fevtno    = nflux_numi->evtno;
      flux.ftpx      = nflux_numi->tpx;
      flux.ftpy      = nflux_numi->tpy;
      flux.ftpz      = nflux_numi->tpz;
      flux.ftptype   = nflux_numi->tptype;   // converted to PDG
#ifndef GSIMPLE_NUMI_V1
      flux.fvx       = nflux_numi->vx;
      flux.fvy       = nflux_numi->vy;
      flux.fvz       = nflux_numi->vz;
      flux.fndecay   = nflux_numi->ndecay;
      flux.fppmedium = nflux_numi->ppmedium;
#endif
    }
    //   flux.fndxdz    = nflux.ndxdz;
    //   flux.fndydz    = nflux.ndydz;
    //   flux.fnpz      = nflux.npz;
    //   flux.fnenergy  = nflux.nenergy;
    //   flux.fndxdznea = nflux.ndxdznea;
    //   flux.fndydznea = nflux.ndydznea;
    //   flux.fnenergyn = nflux.nenergyn;
    //   flux.fnwtnear  = nflux.nwtnear;
    //   flux.fndxdzfar = nflux.ndxdzfar;
    //   flux.fndydzfar = nflux.ndydzfar;
    //   flux.fnenergyf = nflux.nenergyf;
    //   flux.fnwtfar   = nflux.nwtfar;
    //   flux.fnorig    = nflux.norig;
    //   flux.fndecay   = nflux.ndecay;
    //   flux.fntype    = nflux.ntype;
    //   flux.fvx       = nflux.vx;
    //   flux.fvy       = nflux.vy;
    //   flux.fvz       = nflux.vz;
    //   flux.fpdpx     = nflux.pdpx;
    //   flux.fpdpy     = nflux.pdpy;
    //   flux.fpdpz     = nflux.pdpz;
    //   flux.fppdxdz   = nflux.ppdxdz;
    //   flux.fppdydz   = nflux.ppdydz;
    //   flux.fpppz     = nflux.pppz;
    //   flux.fppenergy = nflux.ppenergy;
    //   flux.fppmedium = nflux.ppmedium;
    //   flux.fptype    = nflux.ptype;     // converted to PDG
    //   flux.fppvx     = nflux.ppvx;
    //   flux.fppvy     = nflux.ppvy;
    //   flux.fppvz     = nflux.ppvz;
    //   flux.fmuparpx  = nflux.muparpx;
    //   flux.fmuparpy  = nflux.muparpy;
    //   flux.fmuparpz  = nflux.muparpz;
    //   flux.fmupare   = nflux.mupare;
    //   flux.fnecm     = nflux.necm;
    //   flux.fnimpwt   = nflux.nimpwt;
    //   flux.fxpoint   = nflux.xpoint;
    //   flux.fypoint   = nflux.ypoint;
    //   flux.fzpoint   = nflux.zpoint;
    //   flux.ftvx      = nflux.tvx;
    //   flux.ftvy      = nflux.tvy;
    //   flux.ftvz      = nflux.tvz;
    //   flux.ftgen     = nflux.tgen;
    //   flux.ftgptype  = nflux.tgptype;  // converted to PDG
    //   flux.ftgppx    = nflux.tgppx;
    //   flux.ftgppy    = nflux.tgppy;
    //   flux.ftgppz    = nflux.tgppz;
    //   flux.ftprivx   = nflux.tprivx;
    //   flux.ftprivy   = nflux.tprivy;
    //   flux.ftprivz   = nflux.tprivz;
    //   flux.fbeamx    = nflux.beamx;
    //   flux.fbeamy    = nflux.beamy;
    //   flux.fbeamz    = nflux.beamz;
    //   flux.fbeampx   = nflux.beampx;
    //   flux.fbeampy   = nflux.beampy;
    //   flux.fbeampz   = nflux.beampz; 
#endif
    
    flux.fdk2gen   = gsf->GetDecayDist();

    return;
  }

  void GENIEHelper::FindFluxPath(std::string userpattern)
  {
    // Using the the FW_SEARCH_PATH list of directories, apply the
    // user supplied pattern as a suffix to find the flux files.
    // The userpattern might include simple wildcard globs (in contrast 
    // to proper regexp patterns) for the filename part, but not any
    // part of the directory path.  If files are found in more than
    // one FW_SEARCH_PATH alternative take the one with the most files.

    /* was (but only works for single files):
    cet::search_path sp("FW_SEARCH_PATH");
    sp.find_file(pset.get< std::string>("FluxFile"), fFluxFile);
    */

    std::vector<std::string> dirs;
    cet::split_path(cet::getenv("FW_SEARCH_PATH"),dirs);
    if ( dirs.empty() ) dirs.push_back(std::string()); // at least null string 

    // count the number files in each of the distinct alternative paths
    std::map<std::string,size_t> path2n;

    std::vector<std::string>::const_iterator ditr = dirs.begin();
    for ( ; ditr != dirs.end(); ++ditr ) {
      std::string dalt = *ditr;
      // if non-null, does it end with a "/"?  if not add one
      size_t len = dalt.size();
      if ( len > 0 && dalt.rfind('/') != len-1 ) dalt.append("/");

      // GENIE uses 'glob' style wildcards not true regex, i.e. "*" vs ".*"

      std::string filepatt = dalt + userpattern;

      // !WILDCARD only works for file name ... NOT directory                       
      string dirname = gSystem->UnixPathName(gSystem->WorkingDirectory());
      size_t slashpos = filepatt.find_last_of("/");
      size_t fbegin;
      if ( slashpos != std::string::npos ) {
        dirname = filepatt.substr(0,slashpos);
        fbegin = slashpos + 1;
      } else { fbegin = 0; }
      
      const char* epath = gSystem->ExpandPathName(dirname.c_str());
      void* dirp = gSystem->OpenDirectory(epath);
      delete [] epath;
      if ( dirp ) {
        std::string basename = filepatt.substr(fbegin,filepatt.size()-fbegin);
        TRegexp re(basename.c_str(),kTRUE);
        const char* onefile;
        while ( ( onefile = gSystem->GetDirEntry(dirp) ) ) {
          TString afile = onefile;
          if ( afile=="." || afile==".." ) continue;
          if ( basename!=afile && afile.Index(re) == kNPOS ) continue;
          //std::string fullname = dirname + "/" + afile.Data();
          path2n[filepatt]++;  // found one in this directory
        }
        gSystem->FreeDirectory(dirp);
      } // open directory
    } // loop over alternative prefixes 

    // find the path with the maximum # of files in it
    std::map<std::string,size_t>::const_iterator mitr = path2n.begin();
    size_t nfmax = 0, nftot = 0;
    std::string pathmax;
    for ( ; mitr != path2n.end(); ++mitr) {
      nftot += mitr->second;
      if ( mitr->second > nfmax ) {
        pathmax = mitr->first;
        nfmax = mitr->second;
      }
    }

    // no null path allowed for at least these
    if ( fFluxType.compare("ntuple")      == 0 ||
         fFluxType.compare("simple_flux") == 0    )
      assert( pathmax != "" && nftot > 0 );  

    // print something out about what we found
    size_t npath = path2n.size();
    if ( npath > 1 ) {
      mf::LogInfo("GENIEHelper") 
        << " found " << nftot << " files in " << npath << " distinct paths";
      mitr = path2n.begin();
      for ( ; mitr != path2n.end(); ++mitr) {
        mf::LogInfo("GENIEHelper") << mitr->second << " files at: "
                                   << mitr->first;
      }
    }
      
    fFluxFile = pathmax;
    mf::LogInfo("GENIEHelper") << "Generating events for " << fFluxType 
                               << " using " << nfmax << " files at " << fFluxFile;
  } // FindFluxPath

} // namespace evgb

