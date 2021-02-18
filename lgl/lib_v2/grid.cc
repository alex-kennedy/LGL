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

template <Dimension D>
std::array<int, D> Grid<D>::VoxelAtPosition(std::array<float, D> position) {
  std::array<int, D> voxel;
  for (int i = 0; i < D; i++) {
    voxel[i] = position[i] / voxel_side_length_[i];
  }
  return voxel;
};

template <Dimension D>
void Grid<D>::UpdateVoxels() {
  for (auto it = voxel_map_.begin(); it != voxel_map_.end(); it++) {
    const std::array<int, D>& old_voxel = it->first;
    for (const auto particle : it->second) {
      const std::array<int, D>& new_voxel =
          VoxelAtPosition(particle->Position());
      if (new_voxel != old_voxel) {
        voxel_map_[old_voxel].erase(particle);
        voxel_map_[new_voxel].insert(particle);
      }
    }
  }
}

template <Dimension D>
void Grid<D>::RehashVoxels() {
  for (auto& voxel : voxel_map_) {
    voxel.second.rehash(0);
  }
}

template class Grid<lgl::lib::k2Dimensions>;
template class Grid<lgl::lib::k3Dimensions>;

}  // namespace lib_v2
}  // namespace lgl
