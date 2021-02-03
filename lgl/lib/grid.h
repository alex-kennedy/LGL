#ifndef LGL_LIB_GRID_H_
#define LGL_LIB_GRID_H_

#include "pthread_wrapper.h"
#include "types.h"
#include "voxel.h"

namespace lgl {
namespace lib {

template <typename Grid>
class GridIter;

// This is a simple cubical grid. The number of rows=cols=levels.
// It is essential a handler for voxels that generate the grid.
template <typename Occupant>
class Grid : public Amutex {
  friend class GridIter<Grid<Occupant>>;

 public:
  static const Dimension n_dimensions_ = Occupant::n_dimensions_;
  typedef Occupant occupant_type;
  typedef unsigned int size_type;
  typedef typename Occupant::precision precision;
  typedef typename Occupant::vec_type vec_type;
  typedef Voxel<n_dimensions_> voxel_type;
  typedef GridIter<Grid<Occupant>> iterator;

 private:
  typedef Grid<Occupant> Grid_;
  typedef FixedVec<long, n_dimensions_> Vec_l;

 protected:
  precision voxelLength;  // The length of each voxel
  Vec_l voxPerEdge;
  size_type voxelCount;  // Total voxels
  voxel_type* voxels_;   // Pointer the allocated voxels
  vec_type mins;         // Xmin Ymin Zmin etc ...
  vec_type maxs;         // Xmax Ymax Zmax etc ...
  Vec_l voxPerDim;

 private:
  void initVoxels();

  void allocate();

 public:
  Grid() : voxels_(0) {}

  Grid(const vec_type& minsXYZ, const vec_type& maxsXYZ, precision l)
      : voxelLength(l), voxels_(0), mins(minsXYZ), maxs(maxsXYZ) {}

  // This determines the number of voxels for each dimension,
  // and calls a couple of other init methods.
  void initGrid();

  // This will determine which voxel the provided
  // point would be in. The current
  voxel_type* getVoxelFromPosition(const vec_type& x) const;

  void print(std::ostream& o = std::cout) const;

  void min(const vec_type& m) { mins = m; }
  void max(const vec_type& m) { maxs = m; }
  void voxelWidth(precision l) { voxelLength = l; }
  const vec_type& min() const { return mins; }
  const vec_type& max() const { return maxs; }
  precision voxelWidth() const { return voxelLength; }
  size_type size() const { return voxelCount; }
  size_type voxelsPerDim(size_type ii) const { return voxPerDim[ii]; }
  size_type voxelsPerEdge(size_type ii) const { return voxPerEdge[ii]; }

  bool checkInclusion(const vec_type& p) const {
    for (size_type d = 0; d < n_dimensions_; ++d) {
      if (p[d] < mins[d] || p[d] > maxs[d]) return false;
    }
    return true;
  }

  voxel_type& voxel(size_type entry) { return operator[](entry); }

  const voxel_type& voxel(size_type entry) const { return operator[](entry); }

  voxel_type& operator[](size_type entry) { return *(voxels_ + entry); }

  const voxel_type& operator[](size_type entry) const {
    return *(voxels_ + entry);
  }

  ~Grid() { delete[] voxels_; }
};

//---------------------------------------------------------------------------
// Todo:
//       *Should iterate in the direction of max(x,y,z)
//              and not x then y then z
template <typename Grid>
class GridIter {
 public:
  static const Dimension n_dimensions_ = Grid::n_dimensions_;
  typedef typename Grid::occupant_type occupant_type;
  typedef typename Grid::voxel_type voxel_type;
  typedef GridIter<Grid> iterator;
  typedef typename Grid::size_type size_type;
  typedef typename Grid::vec_type vec_type;
  typedef FixedVec<long, n_dimensions_> Vec_l;

 protected:
  int iterID;
  size_type iterMax;
  size_type neighborCtr;
  voxel_type* begin_;  // THE beginning of the grid
  voxel_type* start_;  // Where this iterater is initialized
  voxel_type* end_;    // THE end of the grid
  voxel_type* current_;
  voxel_type* neighbor_;
  Vec_l dimensions;     // Dimensions of the grid
  Vec_l currentVoxel;   // Current location of the itr in the grid
  Vec_l currentNbhr;    // Current location of the nbhr in the grid
  Vec_l startingPoint;  // Starting location of the itr in the grid
  Vec_l voxPerDim;      // Number of voxels each dimension consumes

  void initVals();

 public:
  GridIter() {}

  GridIter(Grid& g) { iterator::initFromGrid(g); }

  GridIter(const iterator& g) { iterator::copy(g); }

  void initFromGrid(Grid& g);

  void print(std::ostream& o = std::cout) const;

  void reset();

  void copy(const iterator& g);

  // This will set/get the starting point of the
  // iterator in the grid
  void start(const Vec_l& s);

  const Vec_l& start() const { return startingPoint; }

  // This will set/get the current point of the
  // iterator in the grid
  void current(const Vec_l& c);

  const Vec_l& current() const { return currentVoxel; }

  // This sets the nbhr counter back to the first position
  void rewindNbhrInc();

  // This will set/get the id of this iterator
  void id(int i) { iterID = i; }
  const int id() const { return iterID; }

  // This gives voxel info
  const voxel_type& startVox() const { return *start_; }
  const voxel_type& firstVox() const { return *begin_; }
  const voxel_type& endVox() const { return *end_; }
  const voxel_type& currentVox() const { return *current_; }
  const voxel_type& nhbrVox() const { return *neighbor_; }
  voxel_type& startVox() { return *start_; }
  voxel_type& firstVox() { return *begin_; }
  voxel_type& endVox() { return *end_; }
  voxel_type& currentVox() { return *current_; }
  voxel_type& nhbrVox() { return *neighbor_; }

  // This moves the iterator one voxel in the + direction
  // 0 is returned if the end is reached
  bool inc();

  // This is a pretty sad arbitrary incrementer.
  bool inc(size_type jj);

  // This moves the iterator one voxel in the - direction
  // 0 is returned if the end is reached
  bool dec();

  // This is a pretty sad arbitrary decrementer.
  bool dec(long jj);

  // Through subsequent calls this will iterate
  // through the neighbors of the current voxel.
  // It determines the boundaries based on the grid
  // size (whether the current voxel is next to a
  // wall or not). It will return 1 if a neighbor
  // is left to inspect and 0 if one has iterated
  // through them all.
  bool incNbhr();

  friend bool operator++(iterator& i) { return i.inc(); }

  bool operator+=(size_type i) { return iterator::inc(i); }

  friend bool operator--(iterator& i) { return i.dec(); }

  bool operator-=(size_type i) { return iterator::dec(i); }

  const iterator& operator=(const iterator& g) {
    iterator::copy(g);
    return *this;
  }

  ~GridIter() { /* Don't delete any voxels */
  }
};

template <typename Particle, typename Grid>
void _placement_error(Particle& p, Grid& g, const char* message);

// The particle does not have to be in the grid
// this to work.
template <typename Particle, typename Grid>
void _place_particle(Particle& p, Grid& g);

// This assumes the particle is already in the grid.
// If the particle is not....!!
template <typename Particle, typename Grid>
void _remove_particle(Particle& p, Grid& g);

// This assumes the particle is already in the grid.
template <typename Particle, typename Grid>
void shift_particle(Particle& p, Grid& g);

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_GRID_H_
