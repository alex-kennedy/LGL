#include "grid_schedule.h"

namespace lgl {
namespace lib {

template <typename Grid>
bool GridSchedule_MTS<Grid>::threadCheck() {
  // Make sure the threadCount is reasonable.
  // This is for linux boxes
  long _PROCESSOR_COUNT_ = 100;  // sysconf(_SC_NPROCESSORS_ONLN);
  // This is necessary on SGIs
  //  long _PROCESSOR_COUNT_ = sysconf(_SC_NPROC_ONLN);
  long threadCount2 =
      (threadCount < _PROCESSOR_COUNT_) ? threadCount : _PROCESSOR_COUNT_;
  threadCount2 = min<long>(threadCount, grid->voxelsPerEdge(0) / 2);
  // LeapCount_=3 guarantees no two interacting voxels are accessed
  // simultaneously
  return threadCount == threadCount2;
}

template <typename Grid>
void GridSchedule_MTS<Grid>::initVars() {
  threadCount = 1;
  offsetCtr = 0;
  endOfGrid = 0;
  voxelList = 0;
  inGridCheck = 1;
  currentVoxel = 0;
}

template <typename Grid>
void GridSchedule_MTS<Grid>::initFromGrid(Grid& g) {
  iter.initFromGrid(g);
  grid = &g;
  gridSize = g.size();
}

template <typename Occupant>
void GridSchedule_MTS<Occupant>::generateVoxelList_MT() {
  if (threadCount == 1) {
    return GS_::generateVoxelList_ST();
  }
  GS_::renew();
  delete[] voxelList;
  voxelList = new Vec_l[gridSize];
  long iteration = 1;
  long voxelCtr = 0;
  bool finishedGrid = 0;

  while (!finishedGrid) {
    bool activatedNbhr = 0;
    voxel_type& v = iter.currentVox();

    // Check to see if any neighbors
    // are used this iteration
    if (v.interactionCtr() == 0) {
      while (iter.incNbhr()) {
        voxel_type& v2 = iter.nhbrVox();
        if (v2.interactionCtr() == iteration) {
          activatedNbhr = 1;
          break;
        }
      }
    }

    // If the voxel is "alone" then mark it
    // and its nbhrs.
    if (activatedNbhr != 1 && (v.interactionCtr() == 0)) {
      voxelList[voxelCtr] = iter.current();
      ++voxelCtr;
      v.interactionCtr(-1);
      iter.rewindNbhrInc();
      iter.incNbhr();  // Get past self.
      while (iter.incNbhr()) {
        voxel_type& v2 = iter.nhbrVox();
        if (v2.interactionCtr() != -1) v2.interactionCtr(1);
      }
    }

    inGridCheck = ++iter;

    // We have hit the end of this pass
    if (!inGridCheck) {
      // Clear all counters.
      iter.reset();
      long remainingVoxels = 0;
      for (long ii = 0; ii < gridSize; ++ii) {
        voxel_type& vc = grid->voxel(ii);
        if (vc.interactionCtr() > 0) {
          vc.interactionCtr(0);
          ++remainingVoxels;
        }
      }
      // We have to keep iterating until all
      // voxels are included in the list
      if (remainingVoxels == 0) {
        finishedGrid = 1;
      } else {
        ++iteration;
      }
    }
  }
}

template <typename Grid>
void GridSchedule_MTS<Grid>::generateVoxelList_ST() {
  GS_::renew();
  delete[] voxelList;
  voxelList = new Vec_l[gridSize];
  long voxelCtr = 0;
  bool inGridCheck = true;
  while (inGridCheck) {
    voxelList[voxelCtr] = iter.current();
    inGridCheck = ++iter;
    ++voxelCtr;
  }
}

template <typename Grid>
typename GridSchedule_MTS<Grid>::size_type GridSchedule_MTS<Grid>::getVoxelList(
    long thread, GridSchedule_MTS<Grid>::Vec_l* l) {
  GridSchedule_MTS<Grid>::size_type jj = 0;
  for (GridSchedule_MTS<Grid>::size_type ii = thread; ii < gridSize;
       ii += threadCount, ++jj) {
    // cout << ii << " " << jj << " "; voxelList[ii].print();
    l[jj] = voxelList[ii];
  }
  return jj;
}

template <typename Occupant>
bool GridSchedule_MTS<Occupant>::getNextVoxel(iterator& i) {
  size_type occupancy = 0;
  while (currentVoxel < gridSize && occupancy == 0) {
    voxel_type& v = grid->voxel(currentVoxel);
    occupancy = v.occupancy();
    if (occupancy != 0) {
      i.current(voxelList[currentVoxel]);
    }
    ++currentVoxel;
  }
  if (currentVoxel > gridSize) {
    endOfGrid = 1;
    return 0;
  } else {
    return 1;
  }
}

template <typename Occupant>
void GridSchedule_MTS<Occupant>::renew() {
  offsetCtr = 0;
  endOfGrid = 0;
  iter.reset();
  inGridCheck = true;
  currentVoxel = 0;
}

template <typename Grid>
void GridSchedule_MTS<Grid>::printVoxelList(std::ostream& o) const {
  if (voxelList != 0) {
    o << "VoxelList:\tDimensions: ";
    for (size_type ii = 0; ii < n_dimensions_; ++ii)
      o << grid->voxelsPerEdge(ii) << '\t';
    o << '\n';
    for (size_type ii = 0; ii < gridSize; ++ii) {
      o << "Step " << ii << '\t';
      voxelList[ii].print(o);
    }
  }
}

template class GridSchedule_MTS<Grid<Particle<k2Dimensions>>>;
template class GridSchedule_MTS<Grid<Particle<k3Dimensions>>>;

}  // namespace lib
}  // namespace lgl
