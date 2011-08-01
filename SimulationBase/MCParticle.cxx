////////////////////////////////////////////////////////////////////////
/// \file  Particle.cxx
/// \brief Description of a particle passed to Geant4
///
/// \version $Id: MCParticle.cxx,v 1.7 2011-08-01 20:56:42 brebel Exp $
/// \author  seligman@nevis.columbia.edu
////////////////////////////////////////////////////////////////////////
#include "SimulationBase/simbase.h"

#include <TDatabasePDG.h>
#include <TParticlePDG.h>
#include <TLorentzVector.h>
#include <TVector3.h>

#include <iterator>
#include <iostream>
#include <climits>

namespace simb {

  // static variables

  /// How do we indicate an uninitialized variable?  I don't want to
  /// use "0" for PDG, because that's potentially a valid value.
  /// Instead, let the compiler give us a value.  The following
  /// template (from climits) evaluates the lower possible negative
  /// number that you can store in an int.

  const int MCParticle::s_uninitialized = std::numeric_limits<int>::min();

  //------------------------------------------------------------
  MCParticle::MCParticle()
  {
  }

  //------------------------------------------------------------
  /// Standard constructor.
  MCParticle::MCParticle(const int trackId, 
			 const int pdg, 
			 const std::string process,
			 const int mother, 
			 const double mass,
			 const int status)
    : fstatus(status)
    , ftrackId(trackId)
    , fpdgCode(pdg)
    , fmother(mother)
    , fprocess(process)
    , fmass(0)
    , fWeight(0.)
  {
    // If the user has supplied a mass, use it.  Otherwise, get the
    // particle mass from the PDG table.
    if ( mass < 0 ){
      const TDatabasePDG* databasePDG = TDatabasePDG::Instance();
      const TParticlePDG* definition = databasePDG->GetParticle( pdg );
      // Check that the particle is known to ROOT.  If not, this is
      // not a major error; Geant4 has an internal particle coding
      // scheme for nuclei that ROOT doesn't recognize.
      if ( definition != 0 ){
	fmass = definition->Mass();
      }
    }
    else fmass = mass;

  }

  //------------------------------------------------------------
  MCParticle::~MCParticle() 
  {
  }

//   // Copy constructor.  Note that since this class inherits from
//   // TObject, we have to copy its information explicitly.
//   MCParticle::MCParticle( const MCParticle& rhs ) 
//   {
//     ftrackId      = rhs.ftrackId;
//     fpdgCode      = rhs.fpdgCode;
//     fmother       = rhs.fmother;
//     ftrajectory   = new sim::Trajectory(*(rhs.ftrajectory));
//     fmass         = rhs.fmass;
//     fprocess      = rhs.fprocess;
//     fpolarization = rhs.fpolarization;
//     fWeight       = rhs.fWeight;
//   }

  //------------------------------------------------------------
  // Assignment operator
  MCParticle& MCParticle::operator=( const MCParticle& rhs )
  {
    // Usual test for self-assignment.
    if ( this == &rhs ) return *this;

    // As a trivial exercise, let's make this operator exception-safe:

    // Copy the non-pointer items.
    fstatus       = rhs.fstatus;
    ftrackId      = rhs.ftrackId;
    fpdgCode      = rhs.fpdgCode;
    fmother       = rhs.fmother;
    fprocess      = rhs.fprocess;
    ftrajectory   = rhs.ftrajectory;
    fmass         = rhs.fmass;
    fpolarization = rhs.fpolarization;
    fdaughters    = rhs.fdaughters;
    fWeight       = fWeight;

    return *this;
  }

  //------------------------------------------------------------
  // Return the "index-th' daughter in the list.
  int MCParticle::Daughter( const int index ) const
  {
    daughters_type::const_iterator i = fdaughters.begin();
    std::advance( i, index );
    return *i;
  }

  //------------------------------------------------------------
  void MCParticle::AddTrajectoryPoint( const TLorentzVector& position, const TLorentzVector& momentum )
  {
    ftrajectory.Add( position, momentum );
  }

  //------------------------------------------------------------
  const TLorentzVector& MCParticle::Position( const int i ) const
  {
    return ftrajectory.Position(i);
  }

  //------------------------------------------------------------
  const TLorentzVector& MCParticle::Momentum( const int i ) const
  {
    return ftrajectory.Momentum(i);
  }

  //------------------------------------------------------------
  std::ostream& operator<< ( std::ostream& output, const MCParticle& particle )
  {
    output << "ID=" << particle.TrackId() << ", ";
    int pdg =  particle.PdgCode();

    // Try to translate the PDG code into text.
    const TDatabasePDG* databasePDG = TDatabasePDG::Instance();
    const TParticlePDG* definition = databasePDG->GetParticle( pdg );
    // Check that the particle is known to ROOT.  If not, this is
    // not a major error; Geant4 has an internal particle coding
    // scheme for nuclei that ROOT doesn't recognize.
    if ( definition != 0 ) output << definition->GetName();
    else output << "PDG=" << pdg;
    
    output << ", Mother ID=" << particle.Mother()
	   << ", Process=" << particle.Process()
	   << ", there are " << particle.NumberTrajectoryPoints() << " trajectory points";

    if(particle.NumberTrajectoryPoints() > 0 )
      output << ", Initial vtx (x,y,z,t)=(" << particle.Vx()
	     << "," << particle.Vy()
	     << "," << particle.Vz()
	     << "," << particle.T()
	     << "), Initial mom (Px,Py,Pz,E)=(" << particle.Px()
	     << "," << particle.Py()
	     << "," << particle.Pz()
	     << "," << particle.E()
	     << ")" << std::endl;
    else
      output << std::endl;

    return output;
  }

} // namespace sim
