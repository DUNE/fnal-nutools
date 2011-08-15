////////////////////////////////////////////////////////////////////////
/// \file EventGeneratorTest.h
//
/// \version $Id: EventGeneratorTest.h,v 1.4 2011-08-15 20:39:55 guenette Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVGEN_TEST_H
#define EVGEN_TEST_H

#include "art/Framework/Core/EDAnalyzer.h"
#include "EventGeneratorBase/GENIEHelper.h"
#include "EventGeneratorBase/CRYHelper.h"

#include "TStopwatch.h"

namespace art  { class Event; }
namespace simb { class MCParticle; }

///Monte Carlo event generation
namespace evgen {

  /// A module to check the results from the Monte Carlo generator
  class EventGeneratorTest : public art::EDAnalyzer {

  public:

    explicit EventGeneratorTest(fhicl::ParameterSet const &pset);
    virtual ~EventGeneratorTest();                        

    void analyze(art::Event const& evt);  
    void beginJob();

  private:

    fhicl::ParameterSet GENIEParameterSet(std::string fluxType, 
					  bool usePOTPerSpill);

    void                 GENIETest(fhicl::ParameterSet const& pset);
    void                 GENIEHistogramFluxTest();
    void                 GENIESimpleFluxTest();
    void                 GENIEMonoFluxTest();
    void                 GENIEAtmoFluxTest();
    void                 GENIENtupleFluxTest();

    fhicl::ParameterSet  CRYParameterSet();
    void                 CRYTest();
    bool                 IntersectsDetector(simb::MCParticle const& part);
    void                 ProjectToSurface(TLorentzVector pos,
					  TLorentzVector mom,
					  int axis,
					  double surfaceLoc,
					  double* xyz);


    double      fTotalGENIEPOT;          ///< number of POT to generate with GENIE when 
           	                         ///< in total POT mode			 
    double 	fTotalGENIEInteractions; ///< number of interactions to generate with 
           	                         ///< GENIE when in EventsPerSpill mode	 
    double 	fTotalCRYSpills;         ///< number of spills to use when testing CRY
    std::string fTopVolume;              ///< Top Volume used by GENIE
  };
};

#endif // EVGEN_TEST_H
////////////////////////////////////////////////////////////////////////
