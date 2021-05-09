#include "lgl/lib_v2/large_graph.h"

#include <string>

#include "external/com_google_absl/absl/status/statusor.h"
#include "external/com_google_absl/absl/strings/str_cat.h"
#include "external/com_google_absl/absl/strings/string_view.h"

namespace lgl {
namespace lib_v2 {

absl::StatusOr<absl::string_view> LargeGraph::NameFromId(uint id) const {
  if (id >= node_id_to_node_name_.size()) {
    return absl::InvalidArgumentError(absl::StrCat("Invalid id: ", id));
  }
  return node_id_to_node_name_[id];
}

absl::StatusOr<int> LargeGraph::IdFromName(const absl::string_view name) const {
  auto result = node_name_to_node_id_.find(name);
  if (result == node_name_to_node_id_.end()) {
    return absl::InvalidArgumentError(absl::StrCat("Invalid name: ", name));
  }
  return result->second;
}

int LargeGraph::NodeCount() const { return node_id_to_node_name_.size(); }

void LargeGraph::AddEdge(absl::string_view source, absl::string_view target) {
  int source_id = AddNode(source);
  int target_id = AddNode(target);
  if (source_id == target_id) {
    return;
  }
  graph_[source_id].insert(target_id);
  graph_[target_id].insert(source_id);
}

int LargeGraph::AddNode(absl::string_view name) {
  auto result = node_name_to_node_id_.find(name);
  if (result != node_name_to_node_id_.end()) {
    return result->second;
  } else {
    node_id_to_node_name_.emplace_back(name);
    graph_.emplace_back();
    int id = node_id_to_node_name_.size() - 1;
    node_name_to_node_id_[name] = id;
    return id;
  }
}

void LargeGraph::shrink_to_fit() {
  node_id_to_node_name_.shrink_to_fit();
  graph_.shrink_to_fit();
}

}  // namespace lib_v2
}  // namespace lgl
