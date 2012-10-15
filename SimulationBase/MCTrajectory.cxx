////////////////////////////////////////////////////////////////////////
/// \file  MCTrajectory.cxx
/// \brief Container of trajectory information for a particle
///
/// \author  seligman@nevis.columbia.edu
////////////////////////////////////////////////////////////////////////

#include "SimulationBase/MCTrajectory.h"

#include <TLorentzVector.h>

#include <iterator>
#include <vector>
#include <iostream>
#include <cmath>

namespace simb {

  // Nothing special need be done for the default constructor or destructor.
  MCTrajectory::MCTrajectory() 
    : ftrajectory()
  {}

  //----------------------------------------------------------------------------
  MCTrajectory::MCTrajectory( const TLorentzVector& position, 
			      const TLorentzVector& momentum )
  {
    ftrajectory.push_back( value_type( position, momentum ) );
  }

  //----------------------------------------------------------------------------
  const TLorentzVector& MCTrajectory::Position( const size_type index ) const
  {
    const_iterator i = ftrajectory.begin();
    std::advance(i,index);
    return (*i).first;
  }

  //----------------------------------------------------------------------------
  const TLorentzVector& MCTrajectory::Momentum( const size_type index ) const
  {
    const_iterator i = ftrajectory.begin();
    std::advance(i,index);
    return (*i).second;
  }

  //----------------------------------------------------------------------------
  double MCTrajectory::TotalLength() const
  {
    const int N = size();
    if(N < 2) return 0;

    // We take the sum of the straight lines between the trajectory points
    double dist = 0;
    for(int n = 0; n < N-1; ++n){
      dist += (Position(n+1)-Position(n)).Vect().Mag();
    }

    return dist;
  }

  //----------------------------------------------------------------------------
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

  //----------------------------------------------------------------------------
  void MCTrajectory::Sparsify(double margin)
  {
    // Need at least three points to think of removing one
    if(size() <= 2) return;

    // Always keep the first point
    list_type newTraj;
    newTraj.push_back(*begin());

    // We keep the last point we kept, and then next point we're planning to
    // keep. Keep scanning the next point forwards until we go outside margin.
    unsigned int prevIdx = 0;
    unsigned int nextIdx = 1;

    do{
      const TVector3 prev = at(prevIdx).first.Vect();
      const TVector3 next = at(nextIdx).first.Vect();

      const TVector3 dir = (next-prev).Unit();
      // Are all the points in between close enough?
      bool ok = true;
      for(unsigned int midIdx = prevIdx+1; midIdx < nextIdx; ++midIdx){
	const TVector3 here = at(midIdx).first.Vect();
	// Perpendicular distance from the line joining prev to next
	const double impact = (here-prev-dir.Dot(here-prev)*dir).Mag();
	if(impact > margin){ok = false; break;}
      }

      if(ok){
	// All the points in between were OK, try skipping one more
	++nextIdx;
      }
      else{
	// Not OK. Go back one point to where we know it worked and save that
	// one. Call that prev and keep searching for the next point.
	newTraj.push_back(at(nextIdx-1));
	prevIdx = nextIdx-1;
      }
      // If we ran off the end of the list then stop.
    } while(nextIdx < size());

    // Always keep the last point
    newTraj.push_back(*rbegin());

    // Replace trajectory with new version
    ftrajectory = newTraj;
  }

} // namespace sim
