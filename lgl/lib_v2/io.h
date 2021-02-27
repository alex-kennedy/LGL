#ifndef LGL_LIB_V2_IO_H_
#define LGL_LIB_V2_IO_H_

#include <memory>

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "lgl/lib_v2/large_graph.h"

namespace lgl {
namespace lib_v2 {

absl::StatusOr<std::unique_ptr<LargeGraph>> ReadLGLFile(
    absl::string_view file_name);

absl::Status WriteLGL(LargeGraph& graph);

}  // namespace lib_v2
}  // namespace lgl

#endif  // LGL_LIB_V2_IO_H_
