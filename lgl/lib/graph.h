#ifndef LGL_LIB_GRAPH_H_
#define LGL_LIB_GRAPH_H_

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/breadth_first_search.hpp"
#include "boost/graph/kruskal_min_spanning_tree.hpp"
#include "boost/graph/visitors.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/limits.hpp"
#include "boost/property_map/property_map.hpp"
#include "boost/tokenizer.hpp"
#include "fixed_vec.h"
#include "mutual_map.h"

namespace lgl {
namespace lib {

namespace GraphDetail {
template <typename T>
inline T mmax(T x, T y) {
  return (x > y) ? x : y;
}
}  // namespace GraphDetail

template <typename Weight>
class Graph {
 public:
  typedef typename std::pair<unsigned int, unsigned int> Edge;
  typedef MutualMap<std::string, int> vertex_index_map;
  typedef typename boost::adjacency_list<
      boost::setS, boost::vecS, boost::undirectedS,
      boost::property<boost::vertex_name_t, int>,
      boost::property<boost::edge_weight_t, Weight>>
      boost_graph;
  typedef typename boost_graph::edge_descriptor edge_descriptor;
  typedef typename boost_graph::vertex_descriptor vertex_descriptor;
  typedef typename boost_graph::edge_iterator edge_iterator;
  typedef typename boost::property_map<boost_graph, boost::edge_weight_t>::type
      weight_map;
  typedef typename boost_graph::vertices_size_type vertices_size_type;
  typedef typename boost_graph::edges_size_type edges_size_type;
  typedef typename boost_graph::vertex_iterator vertex_iterator;
  typedef typename boost_graph::out_edge_iterator out_edge_iterator;
  typedef Weight weight_type;

 private:
  boost_graph G;
  vertex_index_map mutmap;
  bool weight;

 public:
  Graph();

  Graph(const Graph<Weight>& g);

  // MUTATORS
  void vertexIdMap(const vertex_index_map& m);

  void boostGraph(const boost_graph& g);

  boost_graph& boostGraph() { return G; }

  void clear();

  void weights(const weight_map& w);

  void removeEdge(const edge_descriptor& e);

  void eraseEdge(const edge_descriptor& e);

  // ACCESSORS
  typename boost_graph::vertices_size_type vertexCount() const;

  typename boost_graph::edges_size_type edgeCount() const;

  const vertex_index_map& vertexIdMap() const;

  const boost_graph& boostGraph() const { return G; }

  weight_map weights() { return get(boost::edge_weight, G); }

  weight_type getWeight(edge_descriptor e) const {
    return get(boost::edge_weight, G)[e];
  }

  bool hasWeights() const { return weight; }

  void hasWeights(bool b) { weight = b; }

  std::string idFromIndex(int i) const { return mutmap.findFormer(i); }
  int indexFromId(std::string& s) const { return mutmap.findLatter(s); }

  bool doesEdgeExist(vertex_descriptor u, vertex_descriptor v) const;

  void addEdge(edge_descriptor e, weight_type w);

  void addEdge(edge_descriptor e) { addEdge(source(e, G), target(e, G)); }

  void addEdge(vertex_descriptor v1, vertex_descriptor v2, weight_type w) {
    weight = true;
    add_edge(v1, v2, w, G);
  }

  void addEdge(vertex_descriptor v1, vertex_descriptor v2) {
    add_edge(v1, v2, G);
  }

  void print(std::ostream& o = std::cout) const;

  // OPERATORS
  Graph<Weight>& operator=(const Graph<Weight>& g);

  ~Graph() {}
};

template <typename Graph>
void remap(Graph& g);

// TODO(alex-kennedy): Fix or remove
// template <typename Graph>
// typename Graph::weight_type weightBasedOnNegativeOfEdgeCount(
//     const Graph& g, const typename Graph::Edge& e);

template <typename Graph>
typename Graph::weight_type actualWeight(const Graph& g,
                                         const typename Graph::Edge& e);

template <typename Graph>
void generateWeightMapFromNegativeAdjacentVertexCount(Graph& g);

// TODO(alex-kennedy): Fix or remove
// template <typename Graph>
// std::pair<typename Graph::vertex_descriptor, typename
// Graph::vertices_size_type> getVertexWMostEdges(Graph& g);

template <typename Graph>
void setMSTFromGraph(Graph& g, Graph& mst);

// Determines the number of levels two vertices
template <typename LevelMap, typename vertex>
class dos_recorder : public boost::default_bfs_visitor {
 private:
  LevelMap& m;
  vertex v;
  bool found;
  int level;

 public:
  dos_recorder(LevelMap& mm, vertex vv);

  template <typename Edge, typename BoostGraph>
  void tree_edge(Edge e, const BoostGraph& bg);
};

template <typename LevelMap, typename Vertex>
dos_recorder<LevelMap, Vertex> record_dos(LevelMap& m, Vertex v) {
  return dos_recorder<LevelMap, Vertex>(m, v);
}

template <typename Graph>
int degreesOfSeparation(const Graph& g, typename Graph::vertex_descriptor u,
                        typename Graph::vertex_descriptor v);

// Records the number of levels from one vertex to
// any other vertex (Minimum spanning distance)
template <typename LevelMap, typename ParentMap>
class level_recorder : public boost::default_bfs_visitor {
 public:
  level_recorder(LevelMap& dist, ParentMap& parents) : d(dist), p(parents) {}
  template <typename Edge, typename BoostGraph>
  void tree_edge(Edge e, const BoostGraph& bg) {
    d[boost::target(e, bg)] = d[boost::source(e, bg)] + 1;
    p[boost::target(e, bg)] = boost::source(e, bg);
  }

 private:
  LevelMap& d;
  ParentMap& p;
};

template <typename LevelMap, typename ParentMap>
level_recorder<LevelMap, ParentMap> record_levels(LevelMap& m, ParentMap& p) {
  return level_recorder<LevelMap, ParentMap>(m, p);
};

template <typename Graph, typename LevelMap, typename ParentMap>
void setLevelMapFromMST(const Graph& mst, LevelMap& lm, ParentMap& parents,
                        typename Graph::vertex_descriptor root) {
  const typename Graph::boost_graph& mst_bg = mst.boostGraph();
  lm.clear();
  lm.resize(num_vertices(mst_bg));
  parents.clear();
  parents.resize(num_vertices(mst_bg));
  lm[root] = 0;        // Source or root Node
  parents[root] = -1;  // Root has no parent
  breadth_first_search(mst_bg, root,
                       boost::visitor(record_levels(lm, parents)));
}

template <typename LevelMap>
class level_recorder2 : public boost::default_bfs_visitor {
 public:
  level_recorder2(LevelMap& dist) : d(dist) {}
  template <typename Edge, typename BoostGraph>
  void tree_edge(Edge e, const BoostGraph& bg) {
    d[boost::target(e, bg)] = d[boost::source(e, bg)] + 1;
  }

 private:
  LevelMap& d;
};

template <typename LevelMap>
level_recorder2<LevelMap> record_levels2(LevelMap& m) {
  return level_recorder2<LevelMap>(m);
};

template <typename Graph>
int sumDOS(const Graph& g, typename Graph::vertex_descriptor root) {
  std::vector<int> lm;
  lm.resize(g.vertexCount());
  lm[root] = 0;  // Source or root Node
  breadth_first_search(g.boostGraph(), root,
                       boost::visitor(record_levels2(lm)));
  return sum<typename std::vector<int>::iterator, int>(lm.begin(), lm.end());
}

//--------------------------------------------------
// Start at the center of the mst (root), and do a depth
// first search in g for nodes missing from the mst.
//--------------------------------------------------
template <typename Graph, typename LevelMap>
void addNextLevelFromMap(Graph& g, const Graph& model, const LevelMap& lm,
                         unsigned int level);

//--------------------------------------------------
// Take any edges that are in the model and apply them
// to g if g has those vertices. It repopulates the
// missing edges only and does not add any new vertices.
//--------------------------------------------------
// TODO(alex-kennedy): Fix or remove
/*
template <typename Graph, typename LevelMap>
void fillMissingEdgesFromModel(Graph& g, const Graph& model, const LevelMap& lm,
                               int level);
*/

template <typename Graph, typename LevelMap, typename ParentMap>
std::pair<typename Graph::vertex_descriptor, int> generateLevelsFromGraph(
    const Graph& g, LevelMap& levels, ParentMap& parents,
    typename Graph::vertex_descriptor* rootPtr, Graph& mst,
    bool useOriginalWeights);

// TODO(alex-kennedy): Fix or remove
// template <typename Graph, typename LevelMap, typename ParentMap>
// std::pair<typename Graph::vertex_descriptor, int>
// generateLevelsFromGraphProper(
//     const Graph& g, LevelMap& levels, ParentMap& parents,
//     typename Graph::vertex_descriptor* rootPtr);

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_GRAPH_H_