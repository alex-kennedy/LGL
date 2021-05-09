#include "lgl/lib_v2/particle.h"

#include "external/com_google_absl/absl/container/fixed_array.h"
#include "gtest/gtest.h"

namespace lgl {
namespace lib_v2 {
namespace {

TEST(ParticleTest, PositionAndForceInitialiseToZero) {
  int n_dimensions = 3;
  Particle p(n_dimensions);
  auto position = p.Position();
  auto force = p.Force();
  for (int i = 0; i < n_dimensions; i++) {
    EXPECT_EQ(position[i], 0.0f);
    EXPECT_EQ(force[i], 0.0f);
  }
}

TEST(ParticleTest, ForceAppliesCorrectly) {
  int n_dimensions = 3;
  Particle p(n_dimensions);

  absl::FixedArray<float> f1 = {1.0, 2.0, 0.0};
  absl::FixedArray<float> f2 = {4.0, -6.0, 1.0};
  p.ApplyForce(f1);
  p.ApplyForce(f2);

  auto force = p.Force();
  EXPECT_EQ(force[0], 5.0);
  EXPECT_EQ(force[1], -4.0);
  EXPECT_EQ(force[2], 1.0);
}

}  // namespace
}  // namespace lib_v2
}  // namespace lgl
