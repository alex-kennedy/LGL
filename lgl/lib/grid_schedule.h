#ifndef LGL_LIB_GRID_SCHEDULE_H_
#define LGL_LIB_GRID_SCHEDULE_H_

#include <cassert>
#include <iostream>

#include "grid.h"
#include "particle.h"
#include "types.h"
#include "voxel.h"

namespace lgl {
namespace lib {

template <typename Grid>
class GridSchedule_MTS {
 public:
  static const Dimension n_dimensions_ = Grid::n_dimensions_;
  typedef long size_type;

 private:
  typedef FixedVec<long, n_dimensions_> Vec_l;
  typedef typename Grid::iterator iterator;
  typedef typename Grid::voxel_type voxel_type;
  typedef GridSchedule_MTS<Grid> GS_;

  bool threadCheck();

  void initVars();

 protected:
  size_type threadCount;
  iterator iter;
  size_type offsetCtr;
  Grid* grid;
  Vec_l* voxelList;
  bool inGridCheck;
  bool endOfGrid;
  size_type gridSize;
  Amutex mutex;
  size_type currentVoxel;

  // Not sure what to do with these
  GridSchedule_MTS() { GS_::initVars(); }

  void initFromGrid(Grid& g);

 public:
  GridSchedule_MTS(Grid& g) : iter(g), grid(&g), gridSize(g.size()) {
    GS_::initVars();
  }

  int lock() { return mutex.lock(); }
  int unlock() { return mutex.unlock(); }

  // void generateVoxelList();

  void generateVoxelList_MT();

  // This method presumes a single thread will be iterating through the grid.
  void generateVoxelList_ST();

  size_type getVoxelList(long thread, Vec_l* l);

  bool getNextVoxel(iterator& i);
  void renew();

  bool inGrid() const { return !endOfGrid; }

  bool threads(long p) {
    threadCount = p;
    return GS_::threadCheck();
  }
  long threads() const { return threadCount; }

  void printVoxelList(std::ostream& o = std::cout) const;

  ~GridSchedule_MTS() { delete[] voxelList; }
};

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_GRID_SCHEDULE_H_
