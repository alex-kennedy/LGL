#ifndef LGL_LIB_V2_PARTICLE_H_
#define LGL_LIB_V2_PARTICLE_H_

#include "external/com_google_absl/absl/container/fixed_array.h"
#include "external/com_google_absl/absl/synchronization/mutex.h"

namespace lgl {
namespace lib_v2 {

class Particle {
 public:
  // A particle is initialised only with the number of dimensions. Force and
  // position are initialised to 0.0.
  explicit Particle(unsigned int dimensions);

  Particle(const Particle& p) : force_(p.force_), position_(p.position_) {}

  // Returns the position of the particle.
  absl::FixedArray<float> Position();

  // Returns the force on the particle to be applied at the next integration.
  absl::FixedArray<float> Force();

  // Applies the given force to the particle, which will be accounted for the
  // next time Integrate is called.
  void ApplyForce(const absl::FixedArray<float>& force_to_apply);

 private:
  // D-dimensional vector representing the force on the particle in the current
  // iteration.
  absl::FixedArray<float> force_;

  // D-dimensional vector representing the current position of the particle.
  absl::FixedArray<float> position_;

  // Particle force and position calculations are parallelised, necessitating a
  // mutex.
  absl::Mutex mutex_;
};

}  // namespace lib_v2
}  // namespace lgl

#endif  // LGL_LIB_V2_PARTICLE_H_
