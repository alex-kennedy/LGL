#include "lgl/lib_v2/io.h"

#include <glog/logging.h>

#include <memory>

#include "external/com_google_absl/absl/status/statusor.h"
#include "external/com_google_absl/absl/strings/string_view.h"
#include "external/com_google_googletest/googlemock/include/gmock/gmock.h"
#include "external/com_google_googletest/googletest/include/gtest/gtest.h"

namespace lgl {
namespace lib_v2 {

TEST(IOTest, ReadGraphLGL) {
  absl::StatusOr<std::unique_ptr<LargeGraph>> graph_or =
      // ReadGraphLGL("/workspaces/LGL/test_output/1613031985_new_lgl.lgl");
      ReadGraphLGL("/workspaces/LGL/test_output/1611191021_new_lgl.lgl");
  EXPECT_TRUE(graph_or.ok());
  LOG(INFO) << "graph loaded with " << graph_or.value()->NodeCount()
            << " nodes";
}

TEST(IOTest, ReadGraphNCOL) {
  absl::StatusOr<std::unique_ptr<LargeGraph>> graph_or =
      ReadGraphNCOL("/workspaces/LGL/lgl/t/edges.ncol");
  if (!graph_or.ok()) {
    FAIL() << graph_or.status();
  }
}

}  // namespace lib_v2
}  // namespace lgl
