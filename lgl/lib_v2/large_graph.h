#ifndef LGL_LIB_V2_LARGE_GRAPH_H_
#define LGL_LIB_V2_LARGE_GRAPH_H_

#include <string>
#include <vector>

#include "external/com_google_absl/absl/container/flat_hash_map.h"
#include "external/com_google_absl/absl/container/flat_hash_set.h"
#include "external/com_google_absl/absl/status/statusor.h"
#include "external/com_google_absl/absl/strings/string_view.h"

namespace lgl {
namespace lib_v2 {

class LargeGraph {
 public:
  LargeGraph() {}

  // Gets the name of a node from its ID.
  absl::StatusOr<absl::string_view> NameFromId(uint id) const;

  // Gets the ID of a node from its name.
  absl::StatusOr<int> IdFromName(const absl::string_view name) const;

  // Returns the number of nodes in the graph.
  int NodeCount() const;

  // Adds a bi-directional edge from source to target. Adding an edge that
  // already exists makes no change to the graph. Self-links are ignored. New
  // nodes are added automatically.
  void AddEdge(absl::string_view source, absl::string_view target);

  // Possibly shrinks the vector data members.
  void shrink_to_fit();

 private:
  // Maintains a mapping a of id, used internally, to the name of the node, used
  // in the input data file.
  std::vector<std::string> node_id_to_node_name_;
  absl::flat_hash_map<std::string, uint> node_name_to_node_id_;

  // A bi-directional edge list which encodes the edges of the graph.
  std::vector<absl::flat_hash_set<uint>> graph_;

  // Adds a node by name, creating an ID for it if one does not exist. Returns
  // the ID of the newly created node, or ID of the existing node.
  int AddNode(absl::string_view name);
};

}  // namespace lib_v2
}  // namespace lgl

#endif  // LGL_LIB_V2_LARGE_GRAPH_H_
