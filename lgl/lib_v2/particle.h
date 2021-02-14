#ifndef LGL_LIB_V2_PARTICLE_H_
#define LGL_LIB_V2_PARTICLE_H_

#include <array>

#include "absl/synchronization/mutex.h"
#include "lgl/lib/types.h"

using lgl::lib::Dimension;

namespace lgl {
namespace lib_v2 {

template <Dimension D>
class Particle {
 private:
  // D-dimensional vector representing the force on the particle in the current
  // iteration.
  std::array<float, D> force_ = {};

  // D-dimensional vector representing the current position of the particle.
  std::array<float, D> position_ = {};

  // Particle force and position calculations are parallelised, necessitating a
  // mutex.
  absl::Mutex mutex_;

 public:
  // Returns the position of the particle.
  std::array<float, D> Position();

  // Returns the force on the particle to be applied at the next integration.
  std::array<float, D> Force();

  // Applies the given force to the particle, which will be accounted for the
  // next time Integrate is called.
  void ApplyForce(std::array<float, D>& force_to_apply);
};

}  // namespace lib_v2
}  // namespace lgl

#endif  // LGL_LIB_V2_PARTICLE_H_