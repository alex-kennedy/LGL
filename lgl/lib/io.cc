#include "io.h"

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <fstream>
#include <iostream>

#include "graph.hpp"
#include "types.h"

namespace lgl {
namespace lib {

template <typename Graph>
struct vertex_id_compare {
  const Graph& g;
  vertex_id_compare(const Graph& g1) : g(g1) {}
  bool operator()(const typename Graph::vertex_descriptor v1,
                  const typename Graph::vertex_descriptor v2) const {
    return g.idFromIndex(v1) < g.idFromIndex(v2);
  }
};

template <typename Graph>
void writeLGL(const Graph& g, const char* file) {
  using namespace boost;
  const typename Graph::boost_graph& bg = g.boostGraph();
  const typename Graph::vertex_index_map& ids = g.vertexIdMap();
  typedef typename Graph::weight_type W;
  typename Graph::vertex_iterator vi, viend;
  typename Graph::vertex_descriptor v1, v2;
  typename Graph::out_edge_iterator ee1, ee2;

  if (g.edgeCount() == 0) {
    return;
  }

  std::ofstream out(file);
  if (!out) {
    std::cerr << "writeLGL: Open of " << file << " failed.\n";
    exit(EXIT_FAILURE);
  }

  // Sort based on ids
  vertex_id_compare<Graph> vid(g);
  std::vector<typename Graph::vertex_descriptor> vertices(g.vertexCount());
  int ii = 0;
  for (boost::tie(vi, viend) = boost::vertices(bg); vi != viend; ++vi, ++ii)
    vertices[ii] = *vi;
  sort(vertices.begin(), vertices.end(), vid);

  for (unsigned ii = 0; ii < vertices.size(); ++ii) {
    v1 = vertices[ii];
    tie(ee1, ee2) = out_edges(v1, bg);
    if (ee1 == ee2) {
      continue;
    }
    std::string lines("# " + ids.findFormer(v1) + '\n');
    bool entry = false;
    while (ee1 != ee2) {
      v2 = target(*ee1, bg);
      if (g.idFromIndex(v1) < g.idFromIndex(v2)) {
        entry = true;
        lines += ids.findFormer(v2);
        if (g.hasWeights()) {
          lines += " " + boost::lexical_cast<std::string, W>(g.getWeight(*ee1));
        }
        lines += '\n';
      }
      ++ee1;
    }
    if (entry) {
      out << lines;
    }
  }
}
template void writeLGL(const Graph<FloatType>& g, const char* file);

template <typename Graph>
void writeNCOL(const Graph& g, const char* file) {
  using namespace boost;
  const typename Graph::boost_graph& bg = g.boostGraph();
  const typename Graph::vertex_index_map& ids = g.vertexIdMap();
  typedef typename Graph::weight_type W;
  typename Graph::vertex_iterator vi, viend;
  typename Graph::vertex_descriptor v1, v2;
  typename Graph::out_edge_iterator ee1, ee2;

  if (g.edgeCount() == 0) {
    return;
  }

  std::ofstream out(file);
  if (!out) {
    std::cerr << "writeLGL: Open of " << file << " failed.\n";
    exit(EXIT_FAILURE);
  }

  // Sort based on ids
  vertex_id_compare<Graph> vid(g);
  std::vector<typename Graph::vertex_descriptor> vertices(g.vertexCount());
  int ii = 0;
  for (boost::tie(vi, viend) = boost::vertices(bg); vi != viend; ++vi, ++ii)
    vertices[ii] = *vi;
  sort(vertices.begin(), vertices.end(), vid);

  for (unsigned ii = 0; ii < vertices.size(); ++ii) {
    v1 = vertices[ii];
    tie(ee1, ee2) = out_edges(v1, bg);
    if (ee1 == ee2) {
      continue;
    }
    std::string lines;
    bool entry = false;
    while (ee1 != ee2) {
      v2 = target(*ee1, bg);
      if (g.idFromIndex(v1) < g.idFromIndex(v2)) {
        entry = true;
        lines += ids.findFormer(v1) + " ";
        lines += ids.findFormer(v2);
        if (g.hasWeights()) {
          lines += " " + boost::lexical_cast<std::string, W>(g.getWeight(*ee1));
        }
        lines += '\n';
      }
      ++ee1;
    }
    if (entry) {
      out << lines;
    }
  }
}
template void writeNCOL(const Graph<FloatType>& g, const char* file);

template <typename Graph>
void readLGL(Graph& g, const char* file) {
  readLGL(g, file, std::numeric_limits<typename Graph::weight_type>::max());
}
template void readLGL(Graph<FloatType>& g, const char* file);

template <typename Graph>
void readLGL(Graph& g, const char* file, typename Graph::weight_type cutoff) {
  using namespace boost;
  typedef typename Graph::boost_graph BG;
  typedef typename Graph::vertex_index_map VIM;
  typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
  typename Graph::weight_type
      w{};  // value-initializing to avoid non-legit warning on line 804: ‘w’
            // may be used uninitialized in this function
            // [-Wmaybe-uninitialized]

  std::ifstream in(file);
  if (!in) {
    std::cerr << "readLGL: Open of " << file << " failed.\n";
    exit(EXIT_FAILURE);
  }

  boost::char_separator<char> sep(" ");
  VIM idmap;
  BG bg;
  int index = 0;
  int u = 0, v = 0;

  bool hasweight = false;
  std::string head(256, ' ');

  while (true) {
    std::string id1(256, ' ');
    std::string id2(256, ' ');
    char l[256];
    if (in.eof()) {
      break;
    }
    in.getline(l, 256);
    std::string line(l);
    // cout << "LINE: " << line << endl;
    if (line == "") {
      break;
    }
    tokenizer tokens(line, sep);
    tokenizer::iterator tok_iter = tokens.begin();
    id1 = *tok_iter++;
    if (tok_iter != tokens.end()) {
      id2 = *tok_iter;
    }
    // cout << "IDS: " << id1 << " " << id2 << endl;
    if (id1 == "#") {
      head = id2;
      continue;
    } else {
      if (id2[0] != ' ') {
        w = atof(id2.c_str());
        hasweight = true;
      }
      if (hasweight && w > cutoff) {
        continue;
      }
      if (idmap.doesMapExist(head)) {
        u = idmap.findLatter(head);
      } else {
        idmap.createMap(head, index);
        u = index++;
      }
      if (idmap.doesMapExist(id1)) {
        v = idmap.findLatter(id1);
      } else {
        idmap.createMap(id1, index);
        v = index++;
      }
    }
    if (!hasweight) {
      add_edge(u, v, bg);
    } else {
      // Take the lowest edge if it is redundant
      int nv = num_vertices(bg);
      typename Graph::edge_descriptor eee;
      bool found = false;
      if (nv > u && nv > v) {
        tie(eee, found) = edge(u, v, bg);
      }
      if (found) {
        w = std::min(w, get(boost::edge_weight, bg)[eee]);
        get(boost::edge_weight, bg)[eee] = w;
      } else {
        add_edge(u, v, w, bg);
      }
    }
    // cout << "READ: ADDED " << u << " " << v << " " << w << endl;
  }

  g.boostGraph(bg);
  if (hasweight) {
    g.hasWeights(true);
  }
  g.vertexIdMap(idmap);
}
template void readLGL(Graph<FloatType>& g, const char* file,
                      typename Graph<FloatType>::weight_type cutoff);

template <typename Graph>
void readNCOL(Graph& g, const char* file) {
  typedef typename Graph::boost_graph BG;
  typedef typename Graph::vertex_index_map VIM;

  std::ifstream in(file);
  if (!in) {
    std::cerr << "readNCOL: Open of " << file << " failed.\n";
    exit(EXIT_FAILURE);
  }

  VIM idmap;
  BG bg;
  int index = 0;

  std::string id1(256, ' ');
  std::string id2(256, ' ');

  in >> id1;
  while (id1 != "" && !in.eof()) {
    int u, v;
    if (idmap.doesMapExist(id1)) {
      u = idmap.findLatter(id1);
    } else {
      idmap.createMap(id1, index);
      u = index++;
    }
    in >> id2;
    if (idmap.doesMapExist(id2)) {
      v = idmap.findLatter(id2);
    } else {
      idmap.createMap(id2, index);
      v = index++;
    }
    add_edge(u, v, bg);
    in >> id1;
  }

  g.boostGraph(bg);
  g.vertexIdMap(idmap);
}
template void readNCOL(Graph<FloatType>& g, const char* file);

template <typename Graph, typename LevelMap>
void writeLevelMap2File(const Graph& g, const LevelMap& levels,
                        const char* file) {
  const typename Graph::boost_graph& bg = g.boostGraph();
  const typename Graph::vertex_index_map& ids = g.vertexIdMap();
  typedef typename LevelMap::value_type level_type;
  typedef typename Graph::vertex_descriptor vertex_descriptor;
  typename Graph::edge_iterator ee1, ee2;

  std::ofstream out(file);
  if (!out) {
    std::cerr << "writeLevelMap2File: Open of " << file << " failed.\n";
    exit(EXIT_FAILURE);
  }

  for (tie(ee1, ee2) = edges(bg); ee1 != ee2; ++ee1) {
    vertex_descriptor v1 = source(*ee1, bg);
    vertex_descriptor v2 = target(*ee1, bg);
    level_type l1 = levels[v1];
    level_type l2 = levels[v2];
    level_type l = GraphDetail::mmax<level_type>(l1, l2);
    out << ids.findFormer(v1) << " " << ids.findFormer(v2) << " " << l << '\n';
  }
}
template void writeLevelMap2File(const Graph<FloatType>& g,
                                 const std::vector<unsigned>& levels,
                                 const char* file);

template <typename Graph>
void readLGL_weightMin(Graph& g, const char* file,
                       typename Graph::weight_type cutoff) {
  using namespace boost;
  typedef typename Graph::boost_graph BG;
  typedef typename Graph::vertex_index_map VIM;
  typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
  typename Graph::weight_type w;

  std::ifstream in(file);
  if (!in) {
    std::cerr << "readLGL: Open of " << file << " failed.\n";
    exit(EXIT_FAILURE);
  }

  boost::char_separator<char> sep(" ");
  VIM idmap;
  BG bg;
  int index = 0;
  int u = 0, v = 0;

  bool hasweight = false;
  std::string head(256, ' ');

  while (true) {
    std::string id1(256, ' ');
    std::string id2(256, ' ');
    char l[256];
    if (in.eof()) {
      break;
    }
    in.getline(l, 256);
    std::string line(l);
    // cout << "LINE: " << line << endl;
    if (line == "") {
      break;
    }
    tokenizer tokens(line, sep);
    tokenizer::iterator tok_iter = tokens.begin();
    id1 = *tok_iter++;
    if (tok_iter != tokens.end()) {
      id2 = *tok_iter;
    }
    // cout << "IDS: " << id1 << " " << id2 << endl;
    if (id1 == "#") {
      head = id2;
      continue;
    } else {
      if (id2[0] != ' ') {
        w = atof(id2.c_str());
        hasweight = true;
      }
      if (hasweight && w <= cutoff) {
        continue;
      }
      if (idmap.doesMapExist(head)) {
        u = idmap.findLatter(head);
      } else {
        idmap.createMap(head, index);
        u = index++;
      }
      if (idmap.doesMapExist(id1)) {
        v = idmap.findLatter(id1);
      } else {
        idmap.createMap(id1, index);
        v = index++;
      }
    }
    if (!hasweight) {
      add_edge(u, v, bg);
    } else {
      // Take the lowest edge if it is redundant
      int nv = num_vertices(bg);
      typename Graph::edge_descriptor eee;
      bool found = false;
      if (nv > u && nv > v) {
        tie(eee, found) = edge(u, v, bg);
      }
      if (found) {
        w = std::min(w, get(boost::edge_weight, bg)[eee]);
        get(boost::edge_weight, bg)[eee] = w;
      } else {
        add_edge(u, v, w, bg);
      }
    }
    // cout << "READ: ADDED " << u << " " << v << " " << w << endl;
  }

  g.boostGraph(bg);
  if (hasweight) {
    g.hasWeights(true);
  }
  g.vertexIdMap(idmap);
}
template void readLGL_weightMin(Graph<FloatType>& g, const char* file,
                                typename Graph<FloatType>::weight_type cutoff);

}  // namespace lib
}  // namespace lgl
