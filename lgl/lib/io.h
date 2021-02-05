#ifndef LGL_LIB_IO_H_
#define LGL_LIB_IO_H_

namespace lgl {
namespace lib {

template <typename Graph>
void writeLGL(const Graph& g, const char* file);

template <typename Graph>
void writeNCOL(const Graph& g, const char* file);

template <typename Graph>
void readLGL(Graph& g, const char* file);

template <typename Graph>
void readLGL(Graph& g, const char* file, typename Graph::weight_type cutoff);

template <typename Graph>
void readNCOL(Graph& g, const char* file);

template <typename Graph, typename LevelMap>
void writeLevelMap2File(const Graph& g, const LevelMap& levels,
                        const char* file);

template <typename Graph>
void readLGL_weightMin(Graph& g, const char* file,
                       typename Graph::weight_type cutoff);

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_IO_H_
