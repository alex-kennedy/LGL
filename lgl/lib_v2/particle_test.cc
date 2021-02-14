#include "particle.h"

#include <array>

#include "gtest/gtest.h"
#include "lgl/lib/types.h"

namespace lgl {
namespace lib_v2 {
namespace {

TEST(ParticleTest, PositionAndForceInitialiseToZero) {
  const lgl::lib::Dimension n_dimensions = lgl::lib::k2Dimensions;
  Particle<n_dimensions> p;
  auto position = p.Position();
  auto force = p.Force();
  for (int i = 0; i < n_dimensions; i++) {
    EXPECT_EQ(position[i], 0.0f);
    EXPECT_EQ(force[i], 0.0f);
  }
}

TEST(ParticleTest, ForceAppliesCorrectly) {
  const lgl::lib::Dimension n_dimensions = lgl::lib::k3Dimensions;
  Particle<n_dimensions> p;

  std::array<float, n_dimensions> f1 = {1.0, 2.0, 0.0};
  std::array<float, n_dimensions> f2 = {4.0, -6.0, 1.0};
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