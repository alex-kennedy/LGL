#include "particle_container.h"

#include "types.h"

namespace lgl {
namespace lib {

template <Dimension D>
void ParticleContainer<D>::initParticles() {
  for (size_type ii = 0; ii < size(); ++ii) {
    particles_[ii].index(ii);
  }
}

template <Dimension D>
void ParticleContainer<D>::resize(size_type s) {
  const size_type old_size = size();
  ids.resize(s);
  particles_.resize(s);
  for (size_type ii = old_size; ii < s; ++ii) {
    particles_[ii].index(ii);
    particles_[ii].container(-1);
  }
}

template <Dimension D>
void ParticleContainer<D>::erase(size_type index) {
  particles_.erase(particles_.begin() + index);
  ids.erase(ids.begin() + index);
  for (size_type ii = index; ii < size(); ++ii) {
    particles_[ii].index(ii);
    particles_[ii].container(-1);
  }
}

template class ParticleContainer<k2Dimensions>;
template class ParticleContainer<k3Dimensions>;

}  // namespace lib
}  // namespace lgl