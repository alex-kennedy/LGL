#include "graph.h"

#include <iostream>

#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/breadth_first_search.hpp"
#include "boost/graph/visitors.hpp"
#include "boost/property_map/property_map.hpp"
#include "boost/tokenizer.hpp"
#include "fixed_vec.h"
#include "mutual_map.h"

namespace lgl {
namespace lib {

template <typename Weight>
Graph<Weight>::Graph() : weight(false) {}

template <typename Weight>
Graph<Weight>::Graph(const Graph<Weight>& g) {
  Graph<Weight>::operator=(g);
}

template <typename Weight>
void Graph<Weight>::vertexIdMap(const vertex_index_map& m) {
  mutmap = m;
}

template <typename Weight>
void Graph<Weight>::boostGraph(const Graph<Weight>::boost_graph& g) {
  G = g;
}

template <typename Weight>
void Graph<Weight>::clear() {
  G.clear();
  mutmap.clear();
}

template <typename Weight>
void Graph<Weight>::weights(const weight_map& w) {
  edge_iterator e, eend;
  for (tie(e, eend) = edges(G); e != eend; ++e) {
    get(boost::edge_weight, G)[*e] = w[*e];
  }
  weight = true;
}

template <typename Weight>
void Graph<Weight>::removeEdge(const Graph<Weight>::edge_descriptor& e) {
  remove_edge(e, G);
}

template <typename Weight>
void Graph<Weight>::eraseEdge(const Graph<Weight>::edge_descriptor& e) {
  removeEdge(e);
  if (mutmap.doesMapExist(source(e, G))) mutmap.removeMap(source(e, G));
  if (mutmap.doesMapExist(target(e, G))) mutmap.removeMap(target(e, G));
}

template <typename Weight>
typename Graph<Weight>::vertices_size_type Graph<Weight>::vertexCount() const {
  return num_vertices(G);
}

template <typename Weight>
typename Graph<Weight>::edges_size_type Graph<Weight>::edgeCount() const {
  return num_edges(G);
}

template <typename Weight>
const typename Graph<Weight>::vertex_index_map& Graph<Weight>::vertexIdMap()
    const {
  return mutmap;
}

template <typename Weight>
bool Graph<Weight>::doesEdgeExist(Graph<Weight>::vertex_descriptor u,
                                  Graph<Weight>::vertex_descriptor v) const {
  if (num_edges(G) == 0) {
    return false;
  }
  if (num_vertices(G) <= u) {
    return false;
  }
  if (num_vertices(G) <= v) {
    return false;
  }
  Graph<Weight>::edge_descriptor e;
  bool found;
  tie(e, found) = edge(u, v, G);
  return found;
}

template <typename Weight>
void Graph<Weight>::addEdge(Graph<Weight>::edge_descriptor e,
                            Graph<Weight>::weight_type w) {
  addEdge(source(e, G), target(e, G), w);
}

template <typename Weight>
void Graph<Weight>::print(std::ostream& o) const {
  edge_iterator e, eend;
  for (tie(e, eend) = edges(G); e != eend; ++e) {
    // o << "(" << source(*e,G) << "," << target(*e,G) << ")";
    o << "(" << idFromIndex(source(*e, G)) << "," << idFromIndex(target(*e, G))
      << ")" << std::flush;
    if (weight) {
      o << " = " << getWeight(*e);
    }
    o << '\n';
  }
}

template <typename Weight>
Graph<Weight>& Graph<Weight>::operator=(const Graph<Weight>& g) {
  G = g.boostGraph();
  weight = g.hasWeights();
  mutmap = g.vertexIdMap();
  return *this;
}

template class Graph<FloatType>;

template <typename Graph>
void remap(Graph& g) {
  const typename Graph::boost_graph& bg = g.boostGraph();
  typename Graph::boost_graph ng;
  typename Graph::vertex_index_map nids;
  typename Graph::vertex_descriptor v1, v2, v1new, v2new;
  typename Graph::edge_iterator ee1, ee2;

  int index = 0;
  std::vector<int> marked(num_vertices(bg), -1);
  for (tie(ee1, ee2) = edges(bg); ee1 != ee2; ++ee1) {
    v1 = source(*ee1, bg);
    if (marked[v1] == -1) {
      nids.createMap(g.idFromIndex(v1), index);
      v1new = index;
      marked[v1] = index;
      ++index;
    } else {
      v1new = nids.findLatter(g.idFromIndex(v1));
    }
    v2 = target(*ee1, bg);
    if (marked[v2] == -1) {
      nids.createMap(g.idFromIndex(v2), index);
      v2new = index;
      marked[v2] = index;
      ++index;
    } else {
      v2new = nids.findLatter(g.idFromIndex(v2));
    }
    if (!g.hasWeights())
      add_edge(v1new, v2new, ng);
    else
      add_edge(v1new, v2new, g.getWeight(*ee1), ng);
  }
  g.boostGraph(ng);
  g.vertexIdMap(nids);
}
template void remap(Graph<FloatType>& g);

/*
template <typename Graph>
typename Graph::weight_type weightBasedOnNegativeOfEdgeCount(
    const Graph& g, const typename Graph::Edge& e) {
  typename Graph::out_edge_iterator ee1, ee2;
  typename Graph::vertex_descriptor v1, v2;

  v1 = boost::source(g.boostGraph(), e);
  v2 = boost::target(g.boostGraph(), e);

  // Get edge count of each weight
  tie(ee1, ee2) = out_edges(*v1, g.boostGraph());
  typename Graph::weight_type w = -1 * distance(ee1, ee2);

  tie(ee1, ee2) = out_edges(*v2, g.boostGraph());
  w += -1 * distance(ee1, ee2);

  return w;
}
template typename Graph<FloatType>::weight_type
weightBasedOnNegativeOfEdgeCount(const Graph<FloatType>& g,
                                 const typename Graph<FloatType>::Edge& e);
*/

template <typename Graph>
typename Graph::weight_type actualWeight(const Graph& g,
                                         const typename Graph::Edge& e) {
  return g.getWeight(e);
}
typename Graph<FloatType>::weight_type actualWeight(
    const Graph<FloatType>& g, const typename Graph<FloatType>::Edge& e);

template <typename Graph>
void generateWeightMapFromNegativeAdjacentVertexCount(Graph& g) {
  typename Graph::edge_iterator e, eend;
  typename Graph::vertex_iterator v, vend;
  typename Graph::out_edge_iterator ee1, ee2;
  std::map<typename Graph::vertex_descriptor, typename Graph::weight_type>
      weights;
  for (std::tie(v, vend) = vertices(g.boostGraph()); v != vend; ++v) {
    tie(ee1, ee2) = out_edges(*v, g.boostGraph());
    weights[*v] = -1 * distance(ee1, ee2);
  }
  for (std::tie(e, eend) = edges(g.boostGraph()); e != eend; ++e) {
    get(boost::edge_weight, g.boostGraph())[*e] =
        // This makes the weight positive
        // boost::numeric_limits< typename Graph::weight_type >::max() +
        weights[source(*e, g.boostGraph())] +
        weights[target(*e, g.boostGraph())];
  }
  g.hasWeights(true);
}
template void generateWeightMapFromNegativeAdjacentVertexCount(
    Graph<FloatType>&);

/*
template <typename Graph>
std::pair<typename Graph::vertex_descriptor, typename Graph::vertices_size_type>
getVertexWMostEdges(Graph& g) {
  typename Graph::vertex_iterator v, vend;
  typename Graph::out_edge_iterator ee1, ee2;
  std::pair<typename Graph::vertex_descriptor,
            typename Graph::vertices_size_type>
      p;
  typename Graph::vertices_size_type count, most = 0;
  for (tie(v, vend) = vertices(g.boostGraph()); v != vend; ++v) {
    tie(ee1, ee2) = out_edges(*v, g.boostGraph());
    count = distance(ee1, ee2);
    if (count > most) {
      p.first = *v;
      most = p.second = count;
    }
  }
  return p;
}
template std::pair<typename Graph<FloatType>::vertex_descriptor,
                   typename Graph<FloatType>::vertices_size_type>
getVertexWMostEdges(Graph<FloatType>&);
*/

template <typename Graph>
void setMSTFromGraph(Graph& g, Graph& mst) {
  std::vector<typename Graph::edge_descriptor> new_edges;
  typename Graph::edge_descriptor e;
  typename Graph::boost_graph& mst_graph = mst.boostGraph();
  const typename Graph::boost_graph& old_graph = g.boostGraph();
  typename Graph::weight_map old_weights = g.weights();
  kruskal_minimum_spanning_tree(old_graph, std::back_inserter(new_edges));
  bool inserted;
  for (unsigned int ii = 0; ii < new_edges.size(); ++ii) {
    tie(e, inserted) = add_edge(source(new_edges[ii], old_graph),
                                target(new_edges[ii], old_graph), mst_graph);
    get(boost::edge_weight, mst_graph)[e] = old_weights[new_edges[ii]];
  }
  mst.hasWeights(true);
  mst.vertexIdMap(g.vertexIdMap());
}
template void setMSTFromGraph(Graph<FloatType>&, Graph<FloatType>&);

template <typename LevelMap, typename vertex>
dos_recorder<LevelMap, vertex>::dos_recorder(LevelMap& mm, vertex vv)
    : m(mm), v(vv), found(false), level(1) {}

template <typename LevelMap, typename vertex>
template <typename Edge, typename BoostGraph>
void dos_recorder<LevelMap, vertex>::tree_edge(Edge e, const BoostGraph& bg) {
  if (found) {
    return;
  }
  level = m[boost::source(e, bg)] + 1;
  if (boost::target(e, bg) == v) {
    found = true;
  }
  m[boost::target(e, bg)] = level;
}

template <typename Graph>
int degreesOfSeparation(const Graph& g, typename Graph::vertex_descriptor u,
                        typename Graph::vertex_descriptor v) {
  const typename Graph::boost_graph& mst_bg = g.boostGraph();
  std::vector<int> lm(boost::num_vertices(mst_bg), 0);
  breadth_first_search(mst_bg, u, boost::visitor(record_dos(lm, v)));
  return lm[v];
}

template int degreesOfSeparation(const Graph<FloatType>&,
                                 typename Graph<FloatType>::vertex_descriptor,
                                 typename Graph<FloatType>::vertex_descriptor);

// template <typename Graph, typename LevelMap>
// void fillMissingEdgesFromModel(Graph& g, const Graph& model, const LevelMap&
// lm,
//                                int level) {
//   using namespace boost;
//   typename Graph::boost_graph &bg_g = g.boostGraph(),
//                               bg_model = model.boostGraph();
//   typename Graph::vertex_iterator v, vend;
//   typename Graph::out_edge_iterator ee1, ee2;
//   // Go through each vertex of the given graph and look for ones
//   // at the current level
//   for (tie(v, vend) = vertices(bg_model); v != vend; ++v) {
//     if (lm[*v] != level) {
//       continue;
//     }
//     // The only edges that should need repopulating are ones originating
//     // from the current level
//     for (tie(ee1, ee2) = out_edges(*v, bg_model); ee1 != ee2; ++ee1) {
//       // Add any vertices (making an edge) that are lower than the current
//       // level
//       if (lm[target(*ee1, bg_model)] <= level)
//         add_edge(source(*ee1, bg_model), target(*ee1, bg_model), bg_g);
//     }
//   }
// }
// template void fillMissingEdgesFromModel(Graph<FloatType>&,
//                                         const Graph<FloatType>&,
//                                         const std::vector<unsigned>&, int);

template <typename Graph, typename LevelMap, typename ParentMap>
std::pair<typename Graph::vertex_descriptor, int> generateLevelsFromGraph(
    const Graph& g, LevelMap& levels, ParentMap& parents,
    typename Graph::vertex_descriptor* rootPtr, Graph& mst,
    bool useOriginalWeights) {
  Graph newGraph(g);
  // First issue is to weight the nodes if desired
  if (!useOriginalWeights)
    generateWeightMapFromNegativeAdjacentVertexCount(newGraph);
  // Now to make a tree based on these weights
  setMSTFromGraph(newGraph, mst);
  // Now to determine the root node
  typename Graph::vertex_iterator v, vend;
  std::tie(v, vend) = vertices(mst.boostGraph());
  typename Graph::vertex_descriptor root, v1, v2;

  if (!rootPtr) {
    // linear time algorithm to find the root by Ying Wang (yw1984@stanford.edu)

    int i, k, n = mst.vertexCount();
    std::vector<typename Graph::vertex_descriptor> queue(n + 1);
    int *ccount, *d;
    long long tot = 0, min, *a;
    typename Graph::out_edge_iterator ee1, ee2;

    int p, q;
    queue[p = q = 0] = *v;
    a = new long long[n + 1];
    ccount = new int[n + 1];
    d = new int[n + 1];
    for (i = 0; i <= n; i++) ccount[i] = 1, d[i] = -1, a[i] = 0;
    d[*v] = 0;
    const typename Graph::boost_graph& bg = mst.boostGraph();

    for (; p <= q; p++) {
      v1 = queue[p];
      std::tie(ee1, ee2) = out_edges(v1, bg);
      for (; ee1 != ee2; ee1++) {
        v2 = target(*ee1, bg);
        if (d[v2] == -1) queue[++q] = v2, tot += (d[v2] = d[v1] + 1);
      }
    }

    a[*v] = min = tot;
    root = *v;

    for (k = q; k >= 0; k--) {
      v1 = queue[k];
      std::tie(ee1, ee2) = out_edges(v1, bg);
      for (; ee1 != ee2; ee1++) {
        v2 = target(*ee1, bg);
        if (d[v2] > d[v1]) {
          ccount[v1] += ccount[v2];
        }
      }
    }

    for (k = 0; k <= q; k++) {
      v1 = queue[k];
      std::tie(ee1, ee2) = out_edges(v1, bg);
      for (; ee1 != ee2; ee1++) {
        v2 = target(*ee1, bg);
        if (d[v2] > d[v1]) {
          a[v2] = a[v1] - ccount[v2] + (n - ccount[v2]);
          if (a[v2] < min) root = v2, min = a[v2];
        }
      }
    }

    delete[] a;
    delete[] d;
    delete[] ccount;
    std::cerr << "root finding done!" << std::endl;
  } else {
    root = *rootPtr;
  }
  // The node with the most edges is at root
  // tie( root , count ) = getVertexWMostEdges( mst );
  // Generate a breadth first level map
  setLevelMapFromMST(mst, levels, parents, root);
  int maxL = 0;
  for (typename LevelMap::const_iterator i = levels.begin(); i != levels.end();
       ++i) {
    maxL = (int)GraphDetail::mmax<long>(*i, maxL);
  }
  // Return the descriptor to the one that started it all
  // with the total level count
  return std::make_pair(root, maxL);
}
template std::pair<typename Graph<FloatType>::vertex_descriptor, int>
generateLevelsFromGraph(const Graph<FloatType>&, std::vector<unsigned>&,
                        std::vector<unsigned>&,
                        typename Graph<FloatType>::vertex_descriptor*,
                        Graph<FloatType>&, bool);

template <typename Graph, typename LevelMap>
void addNextLevelFromMap(Graph& g, const Graph& model, const LevelMap& lm,
                         unsigned int level) {
  typename Graph::boost_graph& g_bg = g.boostGraph();
  const typename Graph::boost_graph& model_bg = model.boostGraph();
  typename Graph::vertex_iterator v, vend;
  typename Graph::out_edge_iterator ee1, ee2;
  // Go through each vertex of model and look for vertices
  // with the previous level.
  for (std::tie(v, vend) = vertices(model_bg); v != vend; ++v) {
    if (lm[*v] == level) {
      // cout << "CURRENT: " << model.idFromIndex(*v) << endl;
      for (std::tie(ee1, ee2) = out_edges(*v, model_bg); ee1 != ee2; ++ee1) {
        // Add any vertices (making an edge) that are at the current level
        // or below
        if (lm[target(*ee1, model_bg)] <= level) {
          // 	  cout <<  model.idFromIndex(source( *ee1 , model_bg )) << " "
          // 	       <<  model.idFromIndex(target( *ee1 , model_bg )) << endl;
          add_edge(source(*ee1, model_bg), target(*ee1, model_bg), g_bg);
        }
      }
    }
  }
}
template void addNextLevelFromMap(Graph<FloatType>&, const Graph<FloatType>&,
                                  const std::vector<unsigned>&, unsigned int);

// template <typename Graph, typename LevelMap, typename ParentMap>
// std::pair<typename Graph::vertex_descriptor, int>
// generateLevelsFromGraphProper(
//     const Graph& g, LevelMap& levels, ParentMap& parents,
//     typename Graph::vertex_descriptor* rootPtr) {
//   // Now to determine the root node
//   typename Graph::vertex_iterator v, vend;
//   tie(v, vend) = vertices(g.boostGraph());
//   typename Graph::vertex_descriptor root;
//   if (!rootPtr) {
//     // Init mins and find root if none was given
//     int min = sumDOS(g, *v);
//     root = *v;
//     int ctr = 1;
//     std::cerr << std::setw(8) << ctr << std::flush;
//     for (; v != vend; ++v) {
//       int sum = sumDOS(g, *v);
//       if (sum < min) {
//         min = sum;
//         root = *v;
//       }
//       std::cerr << "\b\b\b\b\b\b\b\b" << std::setw(8) << ++ctr << std::flush;
//     }
//   } else {
//     root = *rootPtr;
//   }
//   // The node with the most edges is at root
//   // tie( root , count ) = getVertexWMostEdges( mst );
//   // Generate a breadth first level map
//   setLevelMapFromMST(g, levels, parents, root);
//   int maxL = 0;
//   for (typename LevelMap::const_iterator i = levels.begin(); i !=
//   levels.end();
//        ++i) {
//     maxL = (int)GraphDetail::mmax<long>(*i, maxL);
//   }
//   // Return the descriptor to the one that started it all
//   // with the total level count
//   return std::make_pair(root, maxL);
// }
// template std::pair<typename Graph<FloatType>::vertex_descriptor, int>
// generateLevelsFromGraphProper(const Graph<FloatType>&,
// std::vector<unsigned>&,
//                               std::vector<unsigned>&,
//                               typename Graph<FloatType>::vertex_descriptor*);

}  // namespace lib
}  // namespace lgl