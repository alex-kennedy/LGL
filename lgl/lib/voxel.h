#ifndef LGL_LIB_VOXEL_H_
#define LGL_LIB_VOXEL_H_

#include <iostream>
#include <unordered_set>

#include "aPthread.hpp"
#include "cube.h"
#include "particle.h"

namespace lgl {
namespace lib {

template <Dimension D>
class Voxel : public Cube<D> {
 private:
  using occupant_set_type = typename std::unordered_set<Particle<D>*>;

 public:
  using occupant_type = Particle<D>;
  using size_type = typename occupant_set_type::size_type;
  using occupant_iterator = typename occupant_set_type::iterator;
  using const_occupant_iterator = typename occupant_set_type::const_iterator;

 protected:
  unsigned int index_;
  long interactionCtr_;
  Amutex mutex;
  occupant_set_type occupants;

 public:
  Voxel() : Cube<D>(), index_(0), interactionCtr_(0), mutex() {}

  Voxel(const Voxel<D>& v) { Voxel<D>::operator=(v); }

  unsigned int index() { return index_; }

  int lock() { return mutex.lock(); }
  int unlock() { return mutex.unlock(); }
  int trylock() { return mutex.trylock(); }

  occupant_iterator begin() { return occupants.begin(); }
  occupant_iterator end() { return occupants.end(); }
  const_occupant_iterator begin() const { return occupants.begin(); }
  const_occupant_iterator end() const { return occupants.end(); }

  typename occupant_set_type::size_type occupancy() const {
    return occupants.size();
  }

  long interactionCtr() const { return interactionCtr_; }

  bool empty() const { return occupants.empty(); }

  void index(unsigned int i) { index_ = i; }

  void insert(Particle<D>& o) { occupants.insert(&o); }
  void remove(Particle<D>& o) { occupants.erase(&o); }

  void incInteractionCtr(long i = 1) { interactionCtr_ += i; }
  void interactionCtr(long i) { interactionCtr_ = i; }

  void copy(const Voxel<D>& v);

  void print(std::ostream& o = std::cout) const;

  void resize(const Cube<D>& c) { Cube<D>::operator=(c); }

  bool operator==(const Voxel<D>& v) const { return index_ == v.index_; }

  bool operator!=(const Voxel<D>& v) const { return !(*this == v); }

  const Voxel<D>& operator=(const Voxel<D>& v);
};

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_VOXEL_H_