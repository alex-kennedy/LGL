#include "particle.h"

#include "absl/container/fixed_array.h"
#include "absl/synchronization/mutex.h"

namespace lgl {
namespace lib_v2 {

absl::FixedArray<float> Particle::Position() {
  absl::MutexLock l(&mutex_);
  return position_;
}

absl::FixedArray<float> Particle::Force() {
  absl::MutexLock l(&mutex_);
  return force_;
}

void Particle::ApplyForce(absl::FixedArray<float>& force_to_apply) {
  absl::MutexLock l(&mutex_);
  for (unsigned int i = 0; i < force_.size(); i++) {
    force_[i] += force_to_apply[i];
  }
}

}  // namespace lib_v2
}  // namespace lgl