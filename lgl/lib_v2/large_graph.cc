#include "large_graph.h"

#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"

namespace lgl {
namespace lib_v2 {

int LargeGraph::AddNode(absl::string_view name) {
  if (node_name_to_node_id_.contains(name)) {
    return node_name_to_node_id_[name];
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

absl::StatusOr<absl::string_view> LargeGraph::NameFromId(int id) {
  if (id < 0 || static_cast<std::vector<std::string>::size_type>(id) >=
                    node_id_to_node_name_.size()) {
    return absl::InvalidArgumentError(absl::StrCat("Invalid id: ", id));
  }
  return node_id_to_node_name_[id];
}

absl::StatusOr<int> LargeGraph::IdFromName(absl::string_view name) {
  if (!node_name_to_node_id_.contains(name)) {
    return absl::InvalidArgumentError(absl::StrCat("Invalid name: ", name));
  }
  return node_name_to_node_id_[name];
}

void LargeGraph::AddEdge(absl::string_view source, absl::string_view target) {
  int source_id = AddNode(source);
  int target_id = AddNode(target);
  if (source_id == target_id) {
    return;
  }
  graph_[source_id].emplace(target_id);
  graph_[target_id].emplace(source_id);
}

int LargeGraph::NodeCount() { return node_id_to_node_name_.size(); }

}  // namespace lib_v2
}  // namespace lgl
