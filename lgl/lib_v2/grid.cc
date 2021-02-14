#include "grid.h"

#include "lgl/lib/types.h"

namespace lgl {
namespace lib_v2 {

template <Dimension D>
void Grid<D>::InitGrid(LargeGraph& graph) {
  particles_ = std::vector<Particle<D>>(graph.NodeCount());
  std::vector<bool> is_initialised(graph.NodeCount(), 0);

  // TODO(alex-kennedy): Initialise particle positions randomly, or from a file

  // TODO(alex-kennedy): Initialise voxels and place the particles in their
  // appropriate spot.
}

}  // namespace lib_v2
}  // namespace lgl
