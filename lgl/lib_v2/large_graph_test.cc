#include "large_graph.h"

#include <memory>

#include "absl/algorithm/algorithm.h"
#include "absl/memory/memory.h"
#include "gtest/gtest.h"

namespace lgl {
namespace lib_v2 {
namespace {

TEST(LargeGraphTest, EmptyGraph) {
  std::unique_ptr<LargeGraph> graph = absl::make_unique<LargeGraph>();
  EXPECT_FALSE(graph->NameFromId(1).ok());
  EXPECT_FALSE(graph->IdFromName("empty!").ok());
  graph->shrink_to_fit();
}

TEST(LargeGraphTest, GraphNodesConstructCorrectly) {
  // Constructs the following graph
  //
  // A--B--D
  // | /
  // |/
  // C--E
  //
  std::unique_ptr<LargeGraph> graph = absl::make_unique<LargeGraph>();
  graph->AddEdge("A", "B");
  graph->AddEdge("A", "C");
  graph->AddEdge("A", "A");
  graph->AddEdge("B", "C");
  graph->AddEdge("C", "E");

  auto a_status = graph->IdFromName("A");
  auto e_status = graph->IdFromName("E");
  EXPECT_TRUE(a_status.ok());
  EXPECT_TRUE(e_status.ok());
  auto a_id = a_status.value();
  auto e_id = e_status.value();

  auto a_name = graph->NameFromId(a_id);
  auto e_name = graph->NameFromId(e_id);
  EXPECT_TRUE(a_name.ok());
  EXPECT_TRUE(e_name.ok());
  EXPECT_NE(a_id, e_id);

  EXPECT_EQ(4, graph->NodeCount());
}

}  // namespace
}  // namespace lib_v2
}  // namespace lgl
