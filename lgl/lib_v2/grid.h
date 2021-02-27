#ifndef LGL_LIB_V2_GRID_H_
#define LGL_LIB_V2_GRID_H_

#include <array>

#include "absl/container/fixed_array.h"
#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "large_graph.h"
#include "particle.h"

namespace lgl {
namespace lib_v2 {

class Grid {
 private:
  // Number of dimensions of the grid.
  unsigned int dimensions_;

  // The particles in the grid.
  std::vector<Particle> particles_;

  // The number of voxels in each dimension.
  absl::FixedArray<int> voxel_count_;

  // The length edge of a voxel in each dimension.
  absl::FixedArray<int> voxel_side_length_;

  // The grid space is segmented into voxels. Each voxel maintains a set of the
  // particles present within it at any time.
  absl::flat_hash_map<absl::FixedArray<int>, absl::flat_hash_set<Particle*>>
      voxel_map_;

 public:
  Grid(unsigned int dimensions)
      : dimensions_(dimensions),
        voxel_count_(absl::FixedArray<int>(dimensions)),
        voxel_side_length_(absl::FixedArray<int>(dimensions)){};

  // First, the positions of the particles are initialised, either from a file,
  // or randomly. The voxels are constructed based on those locations, and the
  // particles are assigned to their correct voxels.
  void InitGrid(LargeGraph& graph);

  // Gets the key for the voxel which contains the given position.
  absl::FixedArray<int> VoxelAtPosition(absl::FixedArray<float> position);

  // Moves all particles to their correct voxel.
  void UpdateVoxels();

  // Rehashes all the voxel sets. This might help keep memory costs if voxels
  // become much less crowded.
  void RehashVoxels();
};

}  // namespace lib_v2
}  // namespace lgl

#endif  // LGL_LIB_V2_GRID_H_
