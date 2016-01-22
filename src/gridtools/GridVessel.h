/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   Copyright (c) 2012 The plumed team
   (see the PEOPLE file at the root of the distribution for a list of names)

   See http://www.plumed-code.org for more information.

   This file is part of plumed, version 2.0.

   plumed is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   plumed is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with plumed.  If not, see <http://www.gnu.org/licenses/>.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
#ifndef __PLUMED_gridtools_GridVessel_h
#define __PLUMED_gridtools_GridVessel_h

#include <string>
#include <cstring>
#include <vector>
#include "vesselbase/Vessel.h"

namespace PLMD {
namespace gridtools {

class GridVessel : public vesselbase::Vessel {
private:
/// Do we have derivatives
 bool noderiv;
/// The grid was recently cleared and bounds can be set
 bool wascleared;
/// Have the minimum and maximum for the grid been set
 bool bounds_set;
/// The number of points in the grid
 unsigned npoints;
/// Units for Gaussian Cube file
 double cube_units;
/// The names of the various columns in the grid file
 std::vector<std::string> arg_names;
/// The minimum and maximum of the grid stored as doubles
 std::vector<double> min, max;
/// The numerical distance between adjacent grid points
 std::vector<unsigned> stride;
/// The number of bins in each grid direction
 std::vector<unsigned> nbin;
///  Flatten the grid and get the grid index for a point
 unsigned getIndex( const std::vector<unsigned>& indices ) const ;
/// The grid point that was requested last by getGridPointCoordinates
 unsigned currentGridPoint;
protected:
/// Are we deleting the data after print
 bool nomemory;
/// The number of pieces of information we are storing for each 
/// point in the grid
 unsigned nper;
/// Is this direction periodic
 std::vector<bool> pbc;
/// The minimum and maximum in the grid stored as strings 
 std::vector<std::string> str_min, str_max;
/// The spacing between grid points
 std::vector<double> dx;
/// The dimensionality of the grid
 unsigned dimension;
/// The grid with all the data values on it
 std::vector<double> data;
/// Get the set of points neighouring a particular location in space
 void getNeighbors( const std::vector<double>& pp, const std::vector<unsigned>& nneigh, 
                    unsigned& num_neighbours, std::vector<unsigned>& neighbors ) const ;
/// Get the indices of a particular point
 void getIndices( const std::vector<double>& point, std::vector<unsigned>& indices ) const ;
/// Convert a point in space the the correspoinding grid point
 unsigned getIndex( const std::vector<double>& p ) const ;
public:
/// keywords
 static void registerKeywords( Keywords& keys );
/// Constructor
 explicit GridVessel( const vesselbase::VesselOptions& );
/// Remove the derivatives 
 void setNoDerivatives();
/// Set the minimum and maximum of the grid
 virtual void setBounds( const std::vector<std::string>& smin, const std::vector<std::string>& smax, const std::vector<unsigned>& nbins, const std::vector<double>& spacing );
/// Get a description of the grid to output to the log
 std::string description();
/// Convert an index into indices
 void convertIndexToIndices( const unsigned& index, const std::vector<unsigned>& nnbin, std::vector<unsigned>& indices ) const ;
/// Get the indices fof a point
 void getIndices( const unsigned& index, std::vector<unsigned>& indices ) const ;

/// Operations on one of the elements of grid point i
 void setGridElement( const unsigned&, const unsigned&, const double& );
 void addToGridElement( const unsigned&, const unsigned&, const double& );

/// Operations on one of the elements of grid point specified by vector
 double getGridElement( const std::vector<unsigned>&, const unsigned& ) const ;
 void setGridElement( const std::vector<unsigned>&, const unsigned&, const double& );
 void addToGridElement( const std::vector<unsigned>&, const unsigned&, const double& );
/// Set the size of the buffer equal to nper*npoints
 virtual void resize();
/// Get the number of points in the grid
 unsigned getNumberOfPoints() const;
/// Get the coordinates for a point in the grid
 void getGridPointCoordinates( const unsigned& , std::vector<double>& ) const ;
/// Get the dimensionality of the function
 unsigned getDimension() const ;
/// Get the number of components in the vector stored on each grid point
 unsigned getNumberOfComponents() const ;
/// Is the grid periodic in the ith direction
 bool isPeriodic( const unsigned& i ) const ;
/// Get the number of quantities we have stored at each grid point
 unsigned getNumberOfQuantities() const ;
/// Get the number of grid points for each dimension
 std::vector<unsigned> getNbin() const ;
/// Get the name of the ith component
 std::string getComponentName( const unsigned& i ) const ;
/// Get the vector containing the minimum value of the grid in each dimension
 std::vector<std::string> getMin() const ;
/// Get the vector containing the maximum value of the grid in each dimension
 std::vector<std::string> getMax() const ;
/// Return the volume of one of the grid cells
 double getCellVolume() const ;
/// Get the value of the ith grid element 
 double getGridElement( const unsigned&, const unsigned& ) const ;
/// Get the points neighboring a particular spline point
 void getSplineNeighbors( const unsigned& mybox, std::vector<unsigned>& mysneigh ) const ;
/// Get the spacing between grid points
 const std::vector<double>& getGridSpacing() const ;
/// Get the extent of the grid in one of the axis
 double getGridExtent( const unsigned& i ) const ;
/// Clear all the data stored on the grid
 virtual void clear();
/// This ensures that Gaussian cube fies are in correct units
 void setCubeUnits( const double& units );
/// This ensures that Gaussian cube files are in correct units
 double getCubeUnits() const ;
/// This gives the normalisation of histograms
 virtual double getNorm() const ;
 virtual void switchOffNormalisation(){}
/// Return a string containing the input to the grid so we can clone it
 std::string getInputString() const ;
/// Does this have derivatives
 bool noDerivatives() const ;
/// Get the value and derivatives at a particular location using spline interpolation
 double getValueAndDerivatives( const std::vector<double>& x, const unsigned& ind, std::vector<double>& der ) const ; 
/// Was the grid cleared on the last step
 bool wasreset() const ;
};

inline
unsigned GridVessel::getNumberOfQuantities() const {
  return nper;
}

inline
unsigned GridVessel::getNumberOfPoints() const {
  return npoints;
}

inline
const std::vector<double>& GridVessel::getGridSpacing() const {
  return dx;
}

inline
double GridVessel::getCellVolume() const {
  double myvol=1.0; for(unsigned i=0;i<dimension;++i) myvol *= dx[i];
  return myvol;
}

inline
unsigned GridVessel::getDimension() const {
  return dimension;
}

inline
bool GridVessel::isPeriodic( const unsigned& i ) const {
  return pbc[i];
}

inline
std::string GridVessel::getComponentName( const unsigned& i ) const {
  return arg_names[i];
}

inline
unsigned GridVessel::getNumberOfComponents() const {
  if( noderiv ) return nper;
  return nper / ( dimension + 1 );
}

inline
double GridVessel::getGridExtent( const unsigned& i ) const {
  return max[i] - min[i];
}

inline
double GridVessel::getNorm() const {
  return 1.0;
}

inline
bool GridVessel::noDerivatives() const {
  return noderiv;
}

inline
bool GridVessel::wasreset() const {
  return wascleared;
}

}
}
#endif
