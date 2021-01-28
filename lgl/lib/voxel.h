#ifndef LGL_LIB_VOXEL_H_
#define LGL_LIB_VOXEL_H_

#include <iostream>
#include <unordered_set>

#include "aPthread.hpp"
#include "cube.h"
#include "particle.h"

namespace lgl {
namespace lib {

template <Dimension NDimensions>
class Voxel : public Cube<NDimensions> {
 private:
  using Occupant = Particle<NDimensions>;
  typedef typename std::unordered_set<Occupant*> OL_;
  typedef Voxel<NDimensions> Voxel_;
  typedef Cube<NDimensions> Cube_;

 public:
  typedef typename OL_::size_type size_type;
  typedef typename OL_::iterator occupant_iterator;
  typedef typename OL_::const_iterator const_occupant_iterator;
  typedef Occupant occupant_type;

 protected:
  unsigned int index_;
  long interactionCtr_;
  Amutex mutex;
  std::unordered_set<Particle<NDimensions>*> occupants;

 public:
  Voxel() : Cube<NDimensions>(), index_(0), interactionCtr_(0), mutex() {}

  Voxel(const Voxel<NDimensions>& v) { Voxel<NDimensions>::operator=(v); }

  unsigned int index() { return index_; }

  int lock() { return mutex.lock(); }
  int unlock() { return mutex.unlock(); }
  int trylock() { return mutex.trylock(); }

  auto begin() { return occupants.begin(); }
  auto end() { return occupants.end(); }
  auto begin() const { return occupants.begin(); }
  auto end() const { return occupants.end(); }

  auto occupancy() const { return occupants.size(); }

  long interactionCtr() const { return interactionCtr_; }

  bool empty() const { return occupants.empty(); }

  void index(unsigned int i) { index_ = i; }

  void insert(Particle<NDimensions>& o) { occupants.insert(&o); }
  void remove(Particle<NDimensions>& o) { occupants.erase(&o); }

  void incInteractionCtr(long i = 1) { interactionCtr_ += i; }
  void interactionCtr(long i) { interactionCtr_ = i; }

  void copy(const Voxel<NDimensions>& v);

  void print(std::ostream& o = std::cout) const;

  void resize(const Cube<NDimensions>& c) { Cube_::operator=(c); }

  bool operator==(const Voxel<NDimensions>& v) const {
    return index_ == v.index_;
  }

  bool operator!=(const Voxel<NDimensions>& v) const { return !(*this == v); }

  const Voxel<NDimensions>& operator=(const Voxel<NDimensions>& v);
};

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_VOXEL_H_