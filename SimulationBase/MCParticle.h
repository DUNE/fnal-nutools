////////////////////////////////////////////////////////////////////////
/// \file  MCParticle.h
/// \brief Particle class
/// \version $Id: MCParticle.h,v 1.16 2012-11-20 17:39:38 brebel Exp $
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////

/// This class describes a particle created in the detector Monte
/// Carlo simulation.

#ifndef SIMB_MCPARTICLE_H
#define SIMB_MCPARTICLE_H

#include "SimulationBase/MCTrajectory.h"

#include <set>
#include <string>
#include <iostream>
#include "TVector3.h"
#include "TLorentzVector.h"

namespace simb {

  class MCParticle  {
  public:

    // An indicator for an uninitialized variable (see MCParticle.cxx).
    static const int s_uninitialized; //! Don't write this as ROOT output

    MCParticle();

    // Destructor.
    virtual ~MCParticle();

  protected:
    typedef std::set<int>   daughters_type;

    int                     fstatus;        ///< Status code from generator, geant, etc
    int                     ftrackId;       ///< TrackId
    int                     fpdgCode;       ///< PDG code
    int                     fmother;        ///< Mother
    std::string             fprocess;       ///< Detector-simulation physics process that created the particle
    simb::MCTrajectory      ftrajectory;    ///< particle trajectory (position,momentum)
    double                  fmass;          ///< Mass; from PDG unless overridden Should be in GeV
    TVector3                fpolarization;  ///< Polarization
    daughters_type          fdaughters;     ///< Sorted list of daughters of this particle.
    double                  fWeight;        ///< Assigned weight to this particle for MC tests
    TLorentzVector          fGvtx;          ///< Vertex needed by generater (genie) to rebuild 
                                            ///< genie::EventRecord for event reweighting
    int                     frescatter;     ///< rescatter code

#ifndef __GCCXML__
  public:

    // Standard constructor.  If the mass is not supplied in the
    // argument, then the PDG mass is used.
    // status code = 1 means the particle is to be tracked, default it to be tracked
    // mother = -1 means that this particle has no mother
    MCParticle(const int trackId, 
	       const int pdg, 
	       const std::string process,
	       const int mother  = -1, 
	       const double mass = s_uninitialized,
	       const int status  = 1);


    // our own copy and assignment constructors.
    MCParticle(MCParticle const &)            = default; // Copy constructor.
    MCParticle& operator=( const MCParticle&) = default;
 
    // Accessors.
    //
    // The track ID number assigned by the Monte Carlo.  This will be
    // unique for each Particle in an event. - 0 for primary particles
    const int TrackId() const;

    // Get at the status code returned by GENIE, Geant4, etc
    const int StatusCode() const;

    // The PDG code of the particle.  Note that Geant4 uses the
    // "extended" system for encoding nuclei; e.g., 1000180400 is an
    // Argon nucleus.  See "Monte Carlo PArticle Numbering Scheme" in
    // any Review of Particle Physics.
    const int PdgCode() const;

    // The track ID of the mother particle.  Note that it's possible
    // for a particle to have a mother that's not recorded in the
    // Particle list; e.g., an excited nucleus with low kinetic energy
    // emits a photon with high kinetic energy.
    const int Mother() const;

    const TVector3&  Polarization() const;
    void             SetPolarization( const TVector3& p );

    // The detector-simulation physics process that created the
    // particle. If this is a primary particle, it will have the
    // value "primary"
    std::string Process()   const;

    // Accessors for daughter information.  Note that it's possible
    // (even likely) for a daughter track not to be included in a
    // Particle list, if that daughter particle falls below the energy
    // cut.
    void AddDaughter( const int trackID );
    int  NumberDaughters()               const;
    int  Daughter(const int i)           const; //> Returns the track ID for the "i-th" daughter.

    // Accessors for trajectory information.  
    unsigned int NumberTrajectoryPoints() const;

    const TLorentzVector& Position( const int i = 0 ) const;
    // To avoid confusion with the X() and Y() methods of MCTruth
    // (which return Feynmann x and y), use "Vx,Vy,Vz" for the
    // vertex.
    double Vx(const int i = 0)   const;
    double Vy(const int i = 0) 	 const;
    double Vz(const int i = 0) 	 const;
    double  T(const int i = 0) 	 const;
				                                                     
    const TLorentzVector& EndPosition() const;
    double EndX()              	        const;
    double EndY()              	 	const;
    double EndZ()              	 	const;
    double EndT()              	 	const;

    const TLorentzVector& Momentum( const int i = 0 ) const;
    double   Px(const int i = 0)                      const;
    double   Py(const int i = 0) 		      const;
    double   Pz(const int i = 0) 		      const;
    double    E(const int i = 0) 		      const;
    double    P(const int i = 0) 		      const;
    double   Pt(const int i = 0) 		      const;
    double Mass()                		      const;

    const TLorentzVector& EndMomentum() const;
    double EndPx()                      const;
    double EndPy()               	const;
    double EndPz()               	const;
    double EndE()                	const;

    // Getters and setters for the generator vertex
    //These are for setting the generator vertex.  In the case of genie
    //the generator assumes a cooridnate system with origin at the nucleus.
    //These variables save the particle vertexs in this cooridnate system.
    //After genie generates the event, a cooridnate transformation is done 
    //to place the event in the detector cooridnate system.  These variables 
    //store the vertex before that cooridnate transformation happens.
    void           SetGvtx(double *v);				 
    void 	   SetGvtx(float *v);				 
    void 	   SetGvtx(TLorentzVector v);			 
    void 	   SetGvtx(double x, double y, double z, double t);
    TLorentzVector GetGvtx();
    double         Gvx();
    double 	   Gvy();
    double 	   Gvz();
    double 	   Gvt();

    //Getters and setters for first and last daughter data members
    const int FirstDaughter() const;
    const int LastDaughter()  const;

    //Getters and setters for rescatter status
    void      SetRescatter(int code);
    const int Rescatter() const;
    
    // Access to the trajectory in both a const and non-const context.
    const simb::MCTrajectory& Trajectory() const;

    // Make it easier to add a (position,momentum) point to the
    // trajectory. You must add this information for every point you wish to keep
    void AddTrajectoryPoint( const TLorentzVector& position, const TLorentzVector& momentum );

    // methods for giving/accessing a weight to this particle for use
    // in studies of rare processes, etc
    const double Weight() const;
    void         SetWeight(double wt);

    void SparsifyTrajectory();

    // Define a comparison operator for particles.  This allows us to
    // keep them in sets or maps.  It makes sense to order a list of
    // particles by track ID... but take care!  After we get past the
    // primary particles in an event, it is NOT safe to assume that a
    // particle with a lower track ID is "closer" to the event
    // vertex.
    bool operator<( const simb::MCParticle& other ) const;

    friend std::ostream& operator<< ( std::ostream& output, const simb::MCParticle& );

#endif
  };

} // namespace simb

#ifndef __GCCXML__

#include <functional> // so we can redefine less<> below
#include <math.h>

// methods to access data members and other information
inline const int                 simb::MCParticle::TrackId()                const { return ftrackId;                           }
inline const int             	 simb::MCParticle::StatusCode()    	    const { return fstatus;            		       }
inline const int             	 simb::MCParticle::PdgCode()       	    const { return fpdgCode;           		       }
inline const int             	 simb::MCParticle::Mother()        	    const { return fmother;            		       }
inline const TVector3&       	 simb::MCParticle::Polarization()  	    const { return fpolarization;      		       }
inline       std::string     	 simb::MCParticle::Process()       	    const { return fprocess;           		       }
inline       int             	 simb::MCParticle::NumberDaughters() 	    const { return fdaughters.size();  		       }
inline       unsigned int    	 simb::MCParticle::NumberTrajectoryPoints() const { return ftrajectory.size(); 		       }
inline const TLorentzVector&     simb::MCParticle::Position( const int i )  const { return ftrajectory.Position(i);            }
inline const TLorentzVector&     simb::MCParticle::Momentum( const int i )  const { return ftrajectory.Momentum(i);            }
inline       double          	 simb::MCParticle::Vx(const int i)          const { return Position(i).X();    		       }
inline       double          	 simb::MCParticle::Vy(const int i)          const { return Position(i).Y();    		       }
inline       double          	 simb::MCParticle::Vz(const int i)          const { return Position(i).Z();    		       }
inline       double          	 simb::MCParticle::T(const int i)           const { return Position(i).T();    		       }
inline const TLorentzVector& 	 simb::MCParticle::EndPosition()            const { return Position(ftrajectory.size()-1);     }
inline       double          	 simb::MCParticle::EndX()                   const { return Position(ftrajectory.size()-1).X(); }
inline       double          	 simb::MCParticle::EndY()                   const { return Position(ftrajectory.size()-1).Y(); }
inline       double          	 simb::MCParticle::EndZ()                   const { return Position(ftrajectory.size()-1).Z(); }
inline       double          	 simb::MCParticle::EndT()                   const { return Position(ftrajectory.size()-1).T(); }
inline       double          	 simb::MCParticle::Px(const int i)          const { return Momentum(i).Px();                   }
inline       double          	 simb::MCParticle::Py(const int i)          const { return Momentum(i).Py();    	       }
inline       double          	 simb::MCParticle::Pz(const int i)          const { return Momentum(i).Pz();    	       }
inline       double          	 simb::MCParticle::E(const int i)           const { return Momentum(i).E();     	       }
inline       double          	 simb::MCParticle::P(const int i)           const { return std::sqrt(std::pow(Momentum(i).E(),
													      2.)  
												     - std::pow(fmass,2.));    }
inline       double          	 simb::MCParticle::Pt(const int i)          const { return std::sqrt(std::pow(Momentum(i).Px(),
													      2.) 
												     + std::pow(Momentum(i).Py(),
														2.));          }
inline       double              simb::MCParticle::Mass()                   const { return fmass;                              }
inline const TLorentzVector& 	 simb::MCParticle::EndMomentum()            const { return Momentum(ftrajectory.size()-1);     }
inline       double          	 simb::MCParticle::EndPx()                  const { return Momentum(ftrajectory.size()-1).X(); }
inline       double          	 simb::MCParticle::EndPy()                  const { return Momentum(ftrajectory.size()-1).Y(); }
inline       double          	 simb::MCParticle::EndPz()                  const { return Momentum(ftrajectory.size()-1).Z(); }
inline       double          	 simb::MCParticle::EndE()                   const { return Momentum(ftrajectory.size()-1).T(); }
inline       TLorentzVector  	 simb::MCParticle::GetGvtx()                      { return fGvtx;                              }
inline       double          	 simb::MCParticle::Gvx()                          { return fGvtx.X();                          }
inline       double          	 simb::MCParticle::Gvy()                          { return fGvtx.Y();                          }
inline       double          	 simb::MCParticle::Gvz()                          { return fGvtx.Z();                          }
inline       double          	 simb::MCParticle::Gvt()                          { return fGvtx.T();                          }
inline const int             	 simb::MCParticle::FirstDaughter()          const { return *(fdaughters.begin());              }
inline const int             	 simb::MCParticle::LastDaughter()           const { return *(fdaughters.rbegin());             }
inline const int             	 simb::MCParticle::Rescatter()              const { return frescatter;                         }
inline const simb::MCTrajectory& simb::MCParticle::Trajectory()             const { return ftrajectory;                        }
inline const double              simb::MCParticle::Weight()                 const { return fWeight;                            }

// methods to set information
inline       void                simb::MCParticle::AddTrajectoryPoint(const TLorentzVector& position, 
								      const TLorentzVector& momentum )
                                                                                      { ftrajectory.Add( position, momentum ); }
inline       void                simb::MCParticle::SparsifyTrajectory()               { ftrajectory.Sparsify();                }
inline       void         	 simb::MCParticle::AddDaughter(const int trackID)     { fdaughters.insert(trackID); 	       }
inline       void         	 simb::MCParticle::SetPolarization(TVector3 const& p) { fpolarization = p;          	       }
inline       void         	 simb::MCParticle::SetRescatter(int code)             { frescatter    = code;       	       }
inline       void         	 simb::MCParticle::SetWeight(double wt)               { fWeight       = wt;         	       }

// definition of the < operator
inline bool simb::MCParticle::operator<( const simb::MCParticle& other )    const { return ftrackId < other.ftrackId;          }

// A potentially handy definition: At this stage, I'm not sure
// whether I'm going to be keeping a list based on Particle or on
// Particle*.  We've already defined operator<(Particle,Particle),
// that is, how to compare two Particle objects; by default that also
// defines less<Particle>, which is what the STL containers use for
// comparisons.

// The following defines less<Particle*>, that is, how to compare two
// Particle*: by looking at the objects, not at the pointer
// addresses.  The result is that, e.g., a set<Particle*> will be
// sorted in the order I expect.

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
#endif

#endif // SIMB_MCPARTICLE_H
