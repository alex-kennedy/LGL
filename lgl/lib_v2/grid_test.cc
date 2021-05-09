#include "lgl/lib_v2/grid.h"

#include <memory>

#include "external/com_google_absl/absl/memory/memory.h"
#include "external/com_google_googletest/googletest/include/gtest/gtest.h"

namespace lgl {
namespace lib_v2 {
namespace {

TEST(GridTest, TestSimpleInit) {
  std::unique_ptr<Grid> grid = absl::make_unique<Grid>(3);
}

}  // namespace
}  // namespace lib_v2
}  // namespace lgl
