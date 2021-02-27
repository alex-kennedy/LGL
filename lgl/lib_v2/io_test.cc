#include "io.h"

#include <memory>

#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "glog/logging.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lgl {
namespace lib_v2 {

TEST(IOTest, ReadGraphLGL) {
  absl::StatusOr<std::unique_ptr<LargeGraph>> graph_or =
      ReadGraphLGL("/workspaces/LGL/test_output/1613031985_new_lgl.lgl");
  EXPECT_TRUE(graph_or.ok());
  LOG(INFO) << "graph loaded with " << graph_or.value()->NodeCount()
            << " nodes";
}

TEST(IOTest, ReadGraphNCOL) {
  absl::StatusOr<std::unique_ptr<LargeGraph>> graph_or =
      ReadGraphNCOL("/workspaces/LGL/lgl/t/edges.ncol");
  if (!graph_or.ok()) {
    LOG(ERROR) << graph_or.status();
  }
  EXPECT_TRUE(graph_or.ok());
}

}  // namespace lib_v2
}  // namespace lgl
