#include "lgl/lib_v2/grid.h"

#include "external/com_google_absl/absl/container/fixed_array.h"

namespace lgl {
namespace lib_v2 {

Grid::Grid(unsigned int dimensions)
    : dimensions_(dimensions),
      voxel_count_(absl::FixedArray<int>(dimensions)),
      voxel_side_length_(absl::FixedArray<int>(dimensions)) {}

void Grid::Init(const LargeGraph& graph) {
  particles_ = std::vector<Particle>(graph.NodeCount(), Particle(dimensions_));
  auto is_initialised = std::vector<bool>(graph.NodeCount(), 0);

  // TODO(alex-kennedy): Initialise particle positions randomly, or from a file

  // TODO(alex-kennedy): Initialise voxels and place the particles in their
  // appropriate spot.
}

absl::FixedArray<int> Grid::VoxelAtPosition(absl::FixedArray<float> position) {
  absl::FixedArray<int> voxel(dimensions_);
  for (unsigned int i = 0; i < dimensions_; i++) {
    voxel[i] = position[i] / voxel_side_length_[i];
  }
  return voxel;
}

void Grid::UpdateVoxels() {
  for (auto it = voxel_map_.begin(); it != voxel_map_.end(); it++) {
    const absl::FixedArray<int>& old_voxel = it->first;
    for (const auto particle : it->second) {
      const absl::FixedArray<int>& new_voxel =
          VoxelAtPosition(particle->Position());
      if (new_voxel != old_voxel) {
        voxel_map_[old_voxel].erase(particle);
        voxel_map_[new_voxel].insert(particle);
      }
    }
  }
}

void Grid::RehashVoxels() {
  for (auto& voxel : voxel_map_) {
    voxel.second.rehash(0);
  }
}

}  // namespace lib_v2
}  // namespace lgl
