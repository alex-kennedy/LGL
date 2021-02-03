#include "grid.h"

#include <cassert>
#include <cstdlib>
#include <iostream>

#include "cube.h"
#include "fixedVec.hpp"
#include "particle.h"
#include "voxel.h"
#include "voxelInteractionHandler.hpp"

namespace lgl {
namespace lib {

namespace NbhrVoxelPositions {
//---------------------------------------------------------------------------
//            See "Art of Molecular Dynamics Simulations" by Rapaport
//          These have an offset of 0 though and includes a self reference
//                        0   1   2   3   4   5   6   7   8   9  10  11  12  13
const int off_[3][14] = {{0, 1, 1, 0, -1, 0, 1, 1, 0, -1, -1, -1, 0, 1},
                         {0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, -1, -1, -1},
                         {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

// The above index offset is good for 1D, 2D, and 3D if you stop BEFORE
// the following locations.
const unsigned int iterMax1D = 2;
const unsigned int iterMax2D = 5;
const unsigned int iterMax3D = 14;
}  // namespace NbhrVoxelPositions

template <typename Occupant>
void Grid<Occupant>::initVoxels() {
  vec_type xyz;
  vec_type xyzMin;
  precision vr = voxelLength * .5;
  // Init the mins in all dimensions and initialize
  // for the smallest value.
  for (size_type ii = 0; ii < n_dimensions_; ++ii) {
    xyzMin[ii] = mins[ii] + vr;
    xyz[ii] = xyzMin[ii];
  }
  size_type ctr = 0;
  while (ctr < voxelCount) {
    // Do a stretch of x's first (First dimension)
    long upper = ctr + voxPerEdge[0];
    for (long ii = ctr; ii < upper; ++ii) {
      voxels_[ii].index(ii);
      voxels_[ii].origin(xyz);
      voxels_[ii].radius(vr);
      xyz[0] += voxelLength;
    }
    ctr += voxPerEdge[0];
    xyz[0] = xyzMin[0];
    for (size_type d = 1; d < n_dimensions_; ++d) {
      // Check for a wall or edge in all higher dimensions
      if (ctr % voxPerDim[d] == 0) {
        xyz[d] += voxelLength;
        // Here we have to reset all lower dimensions
        // because of the increment in the higher one
        for (size_type dd = 0; dd < d; ++dd) {
          xyz[dd] = xyzMin[dd];
        }
      }
    }
  }  // End Of While
}

template <typename Occupant>
void Grid<Occupant>::allocate() {
  voxels_ = new voxel_type[voxelCount];
  assert(voxels_);
}

template <typename Occupant>
void Grid<Occupant>::initGrid() {
  // This sets the number of voxels in each dimension
  for (size_type ii = 0; ii < n_dimensions_; ++ii) {
    precision ctr = mins[ii] - voxelLength;  // Add some pad
    mins[ii] = ctr;
    voxPerEdge[ii] = (unsigned int)ceil((maxs[ii] - ctr) / voxelLength);
    maxs[ii] = mins[ii] + voxPerEdge[ii] * voxelLength;
  }
  voxelCount = voxPerEdge.product();
  // This determines the number of voxels enclosed
  // in each dimension.
  size_type count = 1;
  voxPerDim[0] = count;
  for (size_type ii = 1; ii < n_dimensions_; ++ii) {
    count *= voxPerEdge[ii - 1];
    voxPerDim[ii] = count;
  }
  Grid_::allocate();
  Grid_::initVoxels();
}

template <typename Occupant>
typename Grid<Occupant>::voxel_type* Grid<Occupant>::getVoxelFromPosition(
    const Grid<Occupant>::vec_type& x) const {
  Vec_l coord;
  for (size_type d = 0; d < n_dimensions_; ++d) {
    coord[d] = static_cast<long>((x[d] - mins[d]) / voxelLength);
  }
  // This converts dimension to 1D
  size_type entry = coord.dotProduct(voxPerDim);
  // Double check
  if ((voxels_ + entry)->check_inclusion_fuzzy(x)) {
    return voxels_ + entry;
  } else {
    // The location will not fit in this grid
    return 0;
  }
}

template <typename Occupant>
void Grid<Occupant>::print(std::ostream& o) const {
  o << "voxelLength: " << voxelLength << '\t' << "voxelCount: " << voxelCount
    << '\n';
  o << "VoxPerEdge: ";
  voxPerEdge.print(o);
  o << "Mins: ";
  mins.print(o);
  o << "Maxs: ";
  maxs.print(o);
  o << "voxPerDim: ";
  voxPerDim.print(o);
  o << " ***** VOXELS ***** \n";
  for (size_type ii = 0; ii < voxelCount; ++ii) {
    voxels_[ii].print(o);
  }
}

template class Grid<Particle<k2Dimensions>>;
template class Grid<Particle<k3Dimensions>>;

// GridIter

template <typename Grid>
void GridIter<Grid>::initVals() {
  iterID = 0;
  start_ = 0;
  begin_ = 0;
  current_ = 0;
  end_ = 0;
  neighbor_ = 0;
  neighborCtr = 0;
  currentVoxel = 0;
  currentNbhr = 0;
  startingPoint = 0;
  voxPerDim = 0;
  if (n_dimensions_ == 1)
    iterMax = NbhrVoxelPositions::iterMax1D;
  else if (n_dimensions_ == 2)
    iterMax = NbhrVoxelPositions::iterMax2D;
  else if (n_dimensions_ == 3)
    iterMax = NbhrVoxelPositions::iterMax3D;
  else {
    cerr << "!! GridIter Can only handle dimensions 1-3 !!\n";
    std::exit(1);
  }
}

template <typename Grid>
void GridIter<Grid>::initFromGrid(Grid& g) {
  iterator::initVals();
  voxPerDim = g.voxPerDim;
  dimensions = g.voxPerEdge;
  start_ = neighbor_ = current_ = begin_ = &g[0];
  end_ = &g[g.size() - 1];
}

template <typename Grid>
void GridIter<Grid>::print(std::ostream& o) const {
  o << "Iter: " << iterID << '\n'
    << "\tPtrs: "
    << "Beg: " << begin_ << '\t' << "Start: " << start_ << '\t'
    << "End: " << end_ << '\t' << "Current: " << current_ << '\t'
    << "Nbhr: " << neighbor_ << '\n'
    << "\tGridDimensions: ";
  dimensions.print(o);
  o << "\tStartingPoints: ";
  startingPoint.print(o);
  o << "\tCurrentVoxel: ";
  currentVoxel.print(o);
  o << "CurrentNbhr: " << neighborCtr << " [" << iterMax << "] : ";
  currentNbhr.print(o);
  o << '\n';
}

template <typename Grid>
void GridIter<Grid>::reset() {
  current_ = start_;
  neighbor_ = start_;
  neighborCtr = 0;
  currentVoxel = startingPoint;
  currentNbhr = startingPoint;
}

template <typename Grid>
void GridIter<Grid>::copy(const iterator& g) {
  iterID = g.iterID;
  iterMax = g.iterMax;
  start_ = g.start_;
  begin_ = g.begin_;
  current_ = g.current_;
  end_ = g.end_;
  neighbor_ = g.neighbor_;
  neighborCtr = g.neighborCtr;
  dimensions = g.dimensions;
  currentVoxel = g.currentVoxel;
  currentNbhr = g.currentNbhr;
  startingPoint = g.startingPoint;
  voxPerDim = g.voxPerDim;
}

template <typename Grid>
void GridIter<Grid>::start(const Vec_l& s) {
  startingPoint = s;
  neighborCtr = 0;
  start_ = neighbor_ = current_ = begin_ + voxPerDim.dotProduct(startingPoint);
}

template <typename Grid>
void GridIter<Grid>::current(const Vec_l& c) {
  currentVoxel = c;
  neighborCtr = 0;
  neighbor_ = current_ = begin_ + voxPerDim.dotProduct(currentVoxel);
}

template <typename Grid>
void GridIter<Grid>::rewindNbhrInc() {
  neighbor_ = current_;
  neighborCtr = 0;
}

template <typename Grid>
bool GridIter<Grid>::inc() {
  neighborCtr = 0;
  ++currentVoxel[0];
  ++current_;
  ++neighbor_;
  for (size_type ii = 0; ii < n_dimensions_; ++ii) {
    if (currentVoxel[ii] == dimensions[ii]) {
      currentVoxel[ii] = 0;
      if ((ii + 1) != n_dimensions_) {
        ++currentVoxel[ii + 1];
      }
    } else {
      return 1;
    }
  }
  return 0;
}

template <typename Grid>
bool GridIter<Grid>::inc(size_type jj) {
  for (size_type ii = 0; ii < jj; ++ii) {
    if (!iterator::inc()) {
      return 0;
    }
  }
  return 1;
}

template <typename Grid>
bool GridIter<Grid>::dec() {
  neighborCtr = 0;
  --currentVoxel[0];
  --current_;
  --neighbor_;
  for (size_type ii = 0; ii < n_dimensions_; ++ii) {
    if (currentVoxel[ii] == 0) {
      currentVoxel[ii] = dimensions[ii] - 1;
      if ((ii + 1) != n_dimensions_) {
        --currentVoxel[ii + 1];
      }
    } else {
      return 1;
    }
  }
  return 0;
}

template <typename Grid>
bool GridIter<Grid>::dec(long jj) {
  for (long ii = 0; ii < jj; ++ii) {
    if (!iterator::dec()) {
      return 0;
    }
  }
  return 1;
}

template <typename Grid>
bool GridIter<Grid>::incNbhr() {
  bool gotOne = 0;
  while (neighborCtr < iterMax && !gotOne) {
    for (unsigned int ii = 0; ii < n_dimensions_; ++ii) {
      currentNbhr[ii] =
          currentVoxel[ii] + NbhrVoxelPositions::off_[ii][neighborCtr];
      // This does a boundary check
      if (currentNbhr[ii] == dimensions[ii] || currentNbhr[ii] == -1) {
        break;
      } else if (ii == n_dimensions_ - 1) {
        gotOne = 1;
        ++neighborCtr;
      }
    }
    if (!gotOne) {
      ++neighborCtr;
    }
  }  // End of for
  if (gotOne) {
    neighbor_ = begin_ + voxPerDim.dotProduct(currentNbhr);
    return 1;
  } else {
    return 0;
  }
}

template class GridIter<Grid<Particle<k2Dimensions>>>;
template class GridIter<Grid<Particle<k3Dimensions>>>;

template <typename Particle, typename Grid>
void _placement_error(Particle& p, Grid& g, const char* message) {
  std::cerr << "An error occured shifting a particle around in the voxels: "
            << message << '\n';
  std::cerr << "If the entry is outside the grid, this could be fixed by "
               "initializing a larger grid\n";
  std::cerr << "Particle: ";
  p.print(std::cerr);
  std::cerr << "Grid: ";
  g.print(std::cerr);
  std::exit(EXIT_FAILURE);
}

template void
_placement_error<Particle<k2Dimensions>, Grid<Particle<k2Dimensions>>>(
    Particle<k2Dimensions>& p, Grid<Particle<k2Dimensions>>& g,
    const char* message);

template void
_placement_error<Particle<k3Dimensions>, Grid<Particle<k3Dimensions>>>(
    Particle<k3Dimensions>& p, Grid<Particle<k3Dimensions>>& g,
    const char* message);

template <typename Particle, typename Grid>
void _place_particle(Particle& p, Grid& g) {
  typename Grid::voxel_type* vox = g.getVoxelFromPosition(p.X());
  if (vox == 0) {
    return _placement_error(p, g, "Entry Is Outside Of Grid");
  }
  if (vox->lock() != 0)
    return _placement_error(p, g, "Failed to acquire lock on voxel");
  vox->insert(p);
  vox->unlock();
  p.container(vox->index());
}

template void
_place_particle<Particle<k2Dimensions>, Grid<Particle<k2Dimensions>>>(
    Particle<k2Dimensions>& p, Grid<Particle<k2Dimensions>>& g);

template void
_place_particle<Particle<k3Dimensions>, Grid<Particle<k3Dimensions>>>(
    Particle<k3Dimensions>& p, Grid<Particle<k3Dimensions>>& g);

template <typename Particle, typename Grid>
void _remove_particle(Particle& p, Grid& g) {
  if (p.container() < 0) {
    return _placement_error(p, g, "Remove is illegitimate");
  }
  typename Grid::voxel_type* vox = &(g[p.container()]);
  vox->lock();
  vox->remove(p);
  vox->unlock();
  p.container(-1);
}

template void
_remove_particle<Particle<k2Dimensions>, Grid<Particle<k2Dimensions>>>(
    Particle<k2Dimensions>& p, Grid<Particle<k2Dimensions>>& g);

template void
_remove_particle<Particle<k3Dimensions>, Grid<Particle<k3Dimensions>>>(
    Particle<k3Dimensions>& p, Grid<Particle<k3Dimensions>>& g);

template <typename Particle, typename Grid>
void shift_particle(Particle& p, Grid& g) {
  // Check to see if the particle has even left the current voxel, which is
  // usually not the case.
  if (p.container() >= 0) {
    const typename Grid::voxel_type& v = g[p.container()];
    if (v.check_inclusion_fuzzy(p.X())) {
      // The particle has not left the voxel
      return;
    }
    _remove_particle(p, g);
  }
  _place_particle(p, g);
}

template void
shift_particle<Particle<k2Dimensions>, Grid<Particle<k2Dimensions>>>(
    Particle<k2Dimensions>& p, Grid<Particle<k2Dimensions>>& g);

template void
shift_particle<Particle<k3Dimensions>, Grid<Particle<k3Dimensions>>>(
    Particle<k3Dimensions>& p, Grid<Particle<k3Dimensions>>& g);

}  // namespace lib
}  // namespace lgl
