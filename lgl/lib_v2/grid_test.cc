#include "grid.h"

#include <memory>

#include "absl/memory/memory.h"
#include "gtest/gtest.h"
#include "lgl/lib/types.h"

namespace lgl {
namespace lib_v2 {
namespace {

TEST(GridTest, TestSimpleInit) {
  const auto d = lgl::lib::k2Dimensions;
  std::unique_ptr<Grid<d>> grid = absl::make_unique<Grid<d>>();
}

}  // namespace
}  // namespace lib_v2
}  // namespace lgl
