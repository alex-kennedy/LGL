#ifndef LGL_LIB_V2_GRID_H_
#define LGL_LIB_V2_GRID_H_

#include <array>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "large_graph.h"
#include "lgl/lib/types.h"
#include "particle.h"

using lgl::lib::Dimension;

namespace lgl {
namespace lib_v2 {

template <Dimension D>
class Grid {
 private:
  // The particles in the grid.
  std::vector<Particle<D>> particles_;

  // The grid space is segmented into voxels. Each voxel maintains a set of the
  // particles present within it at any time.
  absl::flat_hash_map<std::array<int, D>, absl::flat_hash_set<Particle<D>*>>
      voxel_map_;

  // The number of voxels in each dimension.
  std::array<int, D> voxel_count_;

  // The length edge of a voxel in each dimension.
  std::array<int, D> voxel_side_length_;

 public:
  Grid(){};

  // First, the positions of the particles are initialised, either from a file,
  // or randomly. The voxels are constructed based on those locations, and the
  // particles are assigned to their correct voxels.
  void InitGrid(LargeGraph& graph);

  // Gets the key for the voxel which contains the given position.
  std::array<int, D> VoxelAtPosition(std::array<float, D> position);

  // Moves all particles to their correct voxel.
  void UpdateVoxels();

  // Rehashes all the voxel sets. This might help keep memory costs if voxels
  // become much less crowded.
  void RehashVoxels();
};

}  // namespace lib_v2
}  // namespace lgl

#endif  // LGL_LIB_V2_GRID_H_
