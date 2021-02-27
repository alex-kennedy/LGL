#include "io.h"

#include <memory>

#include "absl/status/statusor.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

namespace lgl {
namespace lib_v2 {

TEST(IOTest, ReadLGLFile) {
  absl::StatusOr<std::unique_ptr<LargeGraph>> graph_or =
      ReadLGLFile("/workspaces/LGL/test_output/1613031985_new_lgl.lgl");
  EXPECT_TRUE(graph_or.ok());
  LOG(INFO) << "graph loaded with " << graph_or.value()->NodeCount()
            << " nodes";
}

}  // namespace lib_v2
}  // namespace lgl
