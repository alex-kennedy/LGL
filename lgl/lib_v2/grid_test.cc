#include "grid.h"

#include <memory>

#include "absl/memory/memory.h"
#include "gtest/gtest.h"

namespace lgl {
namespace lib_v2 {
namespace {

TEST(GridTest, TestSimpleInit) {
  std::unique_ptr<Grid> grid = absl::make_unique<Grid>(3);
}

}  // namespace
}  // namespace lib_v2
}  // namespace lgl
