////////////////////////////////////////////////////////////////////////
/// \file  MCTrajectory.cxx
/// \brief Container of trajectory information for a particle
///
/// \author  seligman@nevis.columbia.edu
////////////////////////////////////////////////////////////////////////

#include "SimulationBase/simbase.h"

#include <TLorentzVector.h>

#include <iterator>
#include <vector>
#include <iostream>
#include <cmath>

namespace simb {


  MCTrajectory::MCTrajectory( const TLorentzVector& position, 
			      const TLorentzVector& momentum )
  {
    m_trajectory.push_back( value_type( position, momentum ) );
  }

  // Nothing special need be done for the default constructor or destructor.
  MCTrajectory::MCTrajectory() {}
  MCTrajectory::~MCTrajectory() {}

  const TLorentzVector& MCTrajectory::Position( const size_type index ) const
  {
    const_iterator i = m_trajectory.begin();
    std::advance(i,index);
    return (*i).first;
  }

  const TLorentzVector& MCTrajectory::Momentum( const size_type index ) const
  {
    const_iterator i = m_trajectory.begin();
    std::advance(i,index);
    return (*i).second;
  }

  std::ostream& operator<< ( std::ostream& output, const MCTrajectory& list )
  {
    // Determine a field width for the voxel number.
    MCTrajectory::size_type numberOfTrajectories = list.size();
    int numberOfDigits = (int) std::log10( (double) numberOfTrajectories ) + 1;

    // A simple header.
    output.width( numberOfDigits );
    output << "#" << ": < position (x,y,z,t), momentum (Px,Py,Pz,E) >" << std::endl; 

    // Write each trajectory point on a separate line.
    MCTrajectory::size_type nTrajectory = 0;
    for ( MCTrajectory::const_iterator trajectory = list.begin(); trajectory != list.end(); ++trajectory, ++nTrajectory )
      {
	output.width( numberOfDigits );
	output << nTrajectory << ": " 
	       << "< (" << (*trajectory).first.X() 
	       << "," << (*trajectory).first.Y() 
	       << "," << (*trajectory).first.Z() 
	       << "," << (*trajectory).first.T() 
	       << ") , (" << (*trajectory).second.Px() 
	       << "," << (*trajectory).second.Py() 
	       << "," << (*trajectory).second.Pz() 
	       << "," << (*trajectory).second.E() 
	       << ") >" << std::endl;
      }

    return output;
  }


} // namespace sim
