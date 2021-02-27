#ifndef LGL_LIB_V2_IO_H_
#define LGL_LIB_V2_IO_H_

#include <memory>

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "lgl/lib_v2/large_graph.h"

namespace lgl {
namespace lib_v2 {

// Reads a graph, inferring its format from the file extension.
absl::StatusOr<std::unique_ptr<LargeGraph>> ReadGraph(
    absl::string_view file_name);

// Reads a graph using the LGL format. A line beginning with '# ' is connected
// to the following lines. In the following example, node1 is connected to
// node2 and node3, and node2 is connected to node3.
// # node1
// node2
// node3
// # node2
// node3
absl::StatusOr<std::unique_ptr<LargeGraph>> ReadGraphLGL(
    absl::string_view file_name);

// Reads a graph in the NCOL format. It is a simple list of edges, where each
// line contains two nodes separated by a space. In the following example,
// node1 is connected to node2 and node3.
// node1 node2
// node1 node3
absl::StatusOr<std::unique_ptr<LargeGraph>> ReadGraphNCOL(
    absl::string_view file_name);

// TODO
absl::Status WriteLGL(LargeGraph& graph);

}  // namespace lib_v2
}  // namespace lgl

#endif  // LGL_LIB_V2_IO_H_
