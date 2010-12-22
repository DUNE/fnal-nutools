////////////////////////////////////////////////////////////////////////
/// \file  MCTrajectory.h
/// \version $Id: MCTrajectory.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \brief Trajectory class
///
/// \author  seligman@nevis.columbia.edu
////////////////////////////////////////////////////////////////////////

/// This class describes the trajectory of a particle created in the
/// Monte Carlo simulation.  It generally behaves like a 
/// vector< pair<TLorentzVector,TLorentzVector> >, where the first
/// TLorentzVector is the position and the seoond is the momentum,
/// with the following additions:

/// - Methods Position(int) and Momentum(int) for those who are unfamiliar with the
///   concept of "first" and "second" as used with STL pairs:
///      sim::Trajectory* trajectory = sim::Particle.Trajectory();
///      int numberOfPonts = trajectory->size();
///      for (int i=0; i<numberOfPoints; ++i)
///        {
///           TLorentzVector position = trajectory->Position(i);
///           TLorentzVector momentum = trajectory->Momentum(i);
///        }
///   The STL equivalent to the above statements (more efficient):
///      sim::Trajectory* trajectory = sim::Particle.Trajectory();
///      for ( sim::Trajectory::const_iterator i = trajectory->begin();
///            i != trajectory->end(); ++i )
///        {
///            const TLorentzVector& position = (*i).first;
///            const TLorentzVector& momentum = (*i).second;
///        }

/// - As above, but for each position or momentum component; e.g.,
///   trajectory->X(i).

/// - In addition to push_back(pair< TLorentzVector, TLorentzVector>),
///   there's also push_back(TLorentzVector,TLorentzVector) and
///   Add(TLorentzVector,TLorentzVector).  They all do the same thing:
///   add another point to the trajectory.

/// - Print() and operator<< methods for ROOT display and ease of
///   debugging.

/// There are no units defined in this class.  If it's used with
/// Geant4, the units will be (mm,ns,GeV), but this class does not
/// enforce this.

#ifndef SIMB_MCTRAJECTORY_H
#define SIMB_MCTRAJECTORY_H

#include "TLorentzVector.h"

#include <vector>
#include <iostream>

namespace simb {

  class MCTrajectory {
  public:
    /// Some type definitions to make life easier, and to help "hide"
    /// the implementation details.  (If you're not familiar with STL,
    /// you can ignore these definitions.)
    typedef std::vector< std::pair< TLorentzVector, TLorentzVector> >  list_type;
    typedef list_type::value_type                   value_type;
    typedef list_type::iterator                     iterator;
    typedef list_type::const_iterator               const_iterator;
    typedef list_type::reverse_iterator             reverse_iterator;
    typedef list_type::const_reverse_iterator       const_reverse_iterator;
    typedef list_type::size_type                    size_type;
    typedef list_type::difference_type              difference_type;

    /// Standard constructor: Start with initial position and momentum
    /// of the particle.
    MCTrajectory();
    MCTrajectory( const TLorentzVector& vertex, 
		  const TLorentzVector& momentum );

    /// Destructor.
    ~MCTrajectory();

    /// The accessor methods described above.
    const TLorentzVector& Position( const size_type ) const;
    const TLorentzVector& Momentum( const size_type ) const;
    double  X( const size_type i ) const { return Position(i).X();  }
    double  Y( const size_type i ) const { return Position(i).Y();  }
    double  Z( const size_type i ) const { return Position(i).Z();  }
    double  T( const size_type i ) const { return Position(i).T();  }
    double Px( const size_type i ) const { return Momentum(i).Px(); }
    double Py( const size_type i ) const { return Momentum(i).Py(); }
    double Pz( const size_type i ) const { return Momentum(i).Pz(); }
    double  E( const size_type i ) const { return Momentum(i).E();  }

    friend std::ostream& operator<< ( std::ostream& output, const MCTrajectory& );

    /// Standard STL methods, to make this class look like an STL map.
    /// Again, if you don't know STL, you can just ignore these
    /// methods.
    iterator               begin()        { return m_trajectory.begin();  }
    const_iterator         begin()  const { return m_trajectory.begin();  }
    iterator               end()          { return m_trajectory.end();    }
    const_iterator         end()    const { return m_trajectory.end();    }
    reverse_iterator       rbegin()       { return m_trajectory.rbegin(); }
    const_reverse_iterator rbegin() const { return m_trajectory.rbegin(); }
    reverse_iterator       rend()         { return m_trajectory.rend();   }
    const_reverse_iterator rend()   const { return m_trajectory.rend();   }

    size_type size()                    const { return m_trajectory.size();              }
    bool      empty()                   const { return m_trajectory.empty();             }
    void      swap(MCTrajectory& other)       { m_trajectory.swap( other.m_trajectory ); }
    void      clear()                         { m_trajectory.clear();                    }

    // Note that there's no non-const version of operator[] or at() here; once
    // you've added a point to a trajectory, you can't modify it.
    const value_type& operator[](const size_type i) const { return m_trajectory[i]; }
    const value_type& at(const size_type i)         const { return m_trajectory.at(i); }

    /// The only "set" methods for this class; once you've added a
    /// trajectory point, you can't take it back.
    void push_back( const value_type& v )                              { m_trajectory.push_back(v); }
    void push_back( const TLorentzVector& p, const TLorentzVector& m ) { m_trajectory.push_back( value_type(p,m) ); }
    void Add( const TLorentzVector& p, const TLorentzVector& m )       { push_back(p,m); }

  private:
    list_type m_trajectory;

  };

} // namespace simb

#endif // SIMB_MCTRAJECTORY_H
