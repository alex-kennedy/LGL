#include "particle.h"

#include "absl/synchronization/mutex.h"
#include "lgl/lib/types.h"

namespace lgl {
namespace lib_v2 {

template <Dimension D>
std::array<float, D> Particle<D>::Position() {
  absl::MutexLock l(&mutex_);
  return position_;
}

template <Dimension D>
std::array<float, D> Particle<D>::Force() {
  absl::MutexLock l(&mutex_);
  return force_;
}

template <Dimension D>
void Particle<D>::ApplyForce(std::array<float, D>& force_to_apply) {
  absl::MutexLock l(&mutex_);
  for (int i = 0; i < D; i++) {
    force_[i] += force_to_apply[i];
  }
}

template class Particle<lgl::lib::k2Dimensions>;
template class Particle<lgl::lib::k3Dimensions>;

}  // namespace lib_v2
}  // namespace lgl