#ifndef LGL_LIB_PARTICLE_CONTAINER_H_
#define LGL_LIB_PARTICLE_CONTAINER_H_

#include <iostream>
#include <string>
#include <vector>

#include "particle.h"
#include "types.h"

namespace lgl {
namespace lib {

template <Dimension D>
class ParticleContainer {
 public:
  typedef Particle<D> particle_type;
  static const Dimension n_dimensions_ = D;
  using container_type = std::vector<Particle<D>>;
  using size_type = typename container_type::size_type;
  using iterator = typename container_type::iterator;

 protected:
  container_type particles_;
  FloatType avg_temp = 0;
  FloatType avg_dx = 0;

 public:
  vector<string> ids;  // This will hold the names

  void initParticles();

 public:
  ParticleContainer() = default;

  explicit ParticleContainer(size_type s) {
    ParticleContainer<D>::resize(s);
    ParticleContainer<D>::initParticles();
  }

  void resize(size_type s);

  void erase(size_type index);

  iterator begin() noexcept { return particles_.begin(); }
  iterator end() noexcept { return particles_.end(); }

  FloatType temp() const { return avg_temp; }
  FloatType dx() const { return avg_dx; }
  size_type size() const { return particles_.size(); }

  bool operator==(const ParticleContainer<D>& pc) const {
    return particles_ == pc.particles_;
  }

  Particle<D>& particle(size_type entry) { return particles_[entry]; }

  Particle<D>& operator[](size_type entry) {
    return ParticleContainer<D>::particle(entry);
  }

  void print(std::ostream& o = std::cout) const {
    for (size_type ii = 0; ii < size(); ++ii) {
      o << ids[ii] << '\n';
      particles_[ii].print(o);
    }
  }
};

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_PARTICLE_CONTAINER_H_
