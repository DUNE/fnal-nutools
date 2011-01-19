////////////////////////////////////////////////////////////////////////
/// \file  MCParticle.h
/// \brief Particle class
/// \version $Id: MCParticle.h,v 1.2 2011-01-19 16:44:04 p-nusoftart Exp $
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////

/// This class describes a particle created in the detector Monte
/// Carlo simulation.

#ifndef SIMB_MCPARTICLE_H
#define SIMB_MCPARTICLE_H

#include <TLorentzVector.h>
#include <TVector3.h>

#include "art/Persistency/Common/Ptr.h"
#include "SimulationBase/MCTrajectory.h"

#include <math.h>
#include <set>
#include <string>
#include <iostream>
#include <functional> // so we can redefine less<> below

namespace simb {

  class MCParticle  {
  public:

    /// An indicator for an uninitialized variable (see MCParticle.cxx).
    static const int s_uninitialized; //! Don't write this as ROOT output

    MCParticle();

    /// Standard constructor.  If the mass is not supplied in the
    /// argument, then the PDG mass is used.
    /// status code = 1 means the particle is to be tracked, default it to be tracked
    /// mother = -1 means that this particle has no mother
    MCParticle(const int trackId, 
	       const int pdg, 
	       const std::string process,
	       const int mother  = -1, 
	       const double mass = s_uninitialized,
	       const int status  = 1);

    /// Desstructor.
    ~MCParticle();

    // We have a pointer-based private member, so we need to create
    // our own copy and assignment constructors.
/*     Particle( const Particle& rhs ); */
    MCParticle& operator=( const MCParticle& rhs );

    // Note that not every member has a "Set" method.  Once you've set
    // those properties of a particle, you can't change them.

    /// Accessors.
    ///
    /// The track ID number assigned by the Monte Carlo.  This will be
    /// unique for each Particle in an event. - 0 for primary particles
    const int TrackId() const { return m_trackId; }

    /// Get at the status code returned by GENIE, Geant4, etc
    const int StatusCode() const { return m_status; }

    /// The PDG code of the particle.  Note that Geant4 uses the
    /// "extended" system for encoding nuclei; e.g., 1000180400 is an
    /// Argon nucleus.  See "Monte Carlo PArticle Numbering Scheme" in
    /// any Review of Particle Physics.
    const int PdgCode() const { return m_pdgCode; }

    /// The track ID of the mother particle.  Note that it's possible
    /// for a particle to have a mother that's not recorded in the
    /// ParticleList; e.g., an excited nucleus with low kinetic energy
    /// emits a photon with high kinetic energy.
    const int Mother() const            { return m_mother; }
    void SetMother( const int m ) { m_mother = m; }

    const TVector3&  Polarization()                 const { return m_polarization; }
    void             SetPolarization( const TVector3& p ) { m_polarization = p;    }

    /// The detector-simulation physics process that created the
    /// particle. If this is a primary particle, it will have the
    /// value "primary"
    std::string Process() const { return m_process; }

    /// Accessors for daughter information.  Note that it's possible
    /// (even likely) for a daughter track not to be included in a
    /// ParticleList, if that daughter particle falls below the energy
    /// cut.
    void AddDaughter( const int trackID )         { m_daughters.insert( trackID ); }
    int  NumberDaughters()               const    { return m_daughters.size(); }
    int  Daughter(const int i)           const; ///> Returns the track ID for the "i-th" daughter.

    /// Accessors for trajectory information.  
    unsigned int NumberTrajectoryPoints() const { return m_trajectory.size(); }

    const TLorentzVector& Position( const int i = 0 ) const;
    /// To avoid confusion with the X() and Y() methods of MCTruth
    /// (which return Feynmann x and y), use "Vx,Vy,Vz" for the
    /// vertex.
    double Vx(const int i = 0) const { return Position(i).X(); }
    double Vy(const int i = 0) const { return Position(i).Y(); }
    double Vz(const int i = 0) const { return Position(i).Z(); }
    double  T(const int i = 0) const { return Position(i).T(); }

    const TLorentzVector& Momentum( const int i = 0 ) const;
    double   Px(const int i = 0) const { return Momentum(i).Px(); }
    double   Py(const int i = 0) const { return Momentum(i).Py(); }
    double   Pz(const int i = 0) const { return Momentum(i).Pz(); }
    double    E(const int i = 0) const { return Momentum(i).E();  }
    double    P(const int i = 0) const { return sqrt(pow(Momentum(i).E(),2.) - pow(m_mass,2.));  }
    double   Pt(const int i = 0) const { return sqrt(pow(Momentum(i).Px(),2.) + pow(Momentum(i).Py(),2.)); }
    double Mass()                const { return m_mass; }

    /// Access to the trajectory in both a const and non-const context.
    const simb::MCTrajectory& Trajectory() const { return m_trajectory; }

    /// Make it easier to add a (position,momentum) point to the
    /// trajectory. You must add this information for every point you wish to keep
    void AddTrajectoryPoint( const TLorentzVector& position, const TLorentzVector& momentum );

    /// Define a comparison operator for particles.  This allows us to
    /// keep them in sets or maps.  It makes sense to order a list of
    /// particles by track ID... but take care!  After we get past the
    /// primary particles in an event, it is NOT safe to assume that a
    /// particle with a lower track ID is "closer" to the event
    /// vertex.
    bool operator<( const MCParticle& other ) const { return m_trackId < other.m_trackId; }

    friend std::ostream& operator<< ( std::ostream& output, const MCParticle& );

  protected:
    int                     m_status;        ///< Status code from generator, geant, etc
    int                     m_trackId;       ///< TrackId
    int                     m_pdgCode;       ///< PDG code
    int                     m_mother;        ///< Mother
    std::string             m_process;       ///< Detector-simulation physics process that created the particle
    simb::MCTrajectory      m_trajectory;    ///< particle trajectory (posn,mom)
    double                  m_mass;          ///< Mass; from PDG unless overridden
    TVector3                m_polarization;  ///< Polarization

    typedef std::set<int>   daughters_type;
    daughters_type          m_daughters;     ///> Sorted list of daughters of this particle.

  public:

  };

} // namespace simb

/// A potentially handy definition: At this stage, I'm not sure
/// whether I'm going to be keeping a list based on Particle or on
/// Particle*.  We've already defined operator<(Particle,Particle),
/// that is, how to compare two Particle objects; by default that also
/// defines less<Particle>, which is what the STL containers use for
/// comparisons.

/// The following defines less<Particle*>, that is, how to compare two
/// Particle*: by looking at the objects, not at the pointer
/// addresses.  The result is that, e.g., a set<Particle*> will be
/// sorted in the order I expect.

namespace std {
  template <>
  class less<simb::MCParticle*>
  {
  public:
    bool operator()( const simb::MCParticle* lhs, const simb::MCParticle* rhs )
    {
      return (*lhs) < (*rhs);
    }
  };
} // std

#endif // SIM_PARTICLE_H
