#ifndef LGL_LGL_LIB_V2_LAYOUT_RUNNER_H_
#define LGL_LGL_LIB_V2_LAYOUT_RUNNER_H_

#include <memory>

#include "lgl/lib_v2/config.h"
#include "lgl/lib_v2/large_graph.h"
#include "lgl/lib_v2/grid.h"

namespace lgl {
namespace lib_v2 {

class LayoutRunner {
 public:
  explicit LayoutRunner(const LGLConfig &config) : config_(config) {}

  absl::Status Init();

  absl::Status Run();

  absl::Status Write();

 private:
  const LGLConfig config_;

  LargeGraph graph_;
  std::unique_ptr<Grid> grid_;

  absl::Status ConfigIsValid();
};

}  // namespace lib_v2
}  // namespace lgl

#endif  // LGL_LIB_V2_LAYOUT_RUNNER_H_
