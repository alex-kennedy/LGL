#ifndef LGL_LGL_LIB_V2_LAYOUT_RUNNER_H_
#define LGL_LGL_LIB_V2_LAYOUT_RUNNER_H_

#include <memory>

#include "glog/logging.h"
#include "lgl/lib_v2/config.h"
#include "lgl/lib_v2/grid.h"
#include "lgl/lib_v2/io.h"
#include "lgl/lib_v2/large_graph.h"

namespace lgl {
namespace lib_v2 {

class LayoutRunner {
 public:
  explicit LayoutRunner(const LGLConfig &config) : config_(config) {}

  absl::Status Run();

 private:
  const LGLConfig config_;

  std::unique_ptr<LargeGraph> graph_;
  std::unique_ptr<Grid> grid_;

  absl::Status CheckConfig();

  absl::Status Init();

  absl::Status Write();
};

}  // namespace lib_v2
}  // namespace lgl

#endif  // LGL_LIB_V2_LAYOUT_RUNNER_H_
