#include "voxel_interaction_handler.h"

#include "types.h"

namespace lgl {
namespace lib {

template <Dimension D>
void VoxelInteractionHandler<D>::initVars() {
  current = 0;
  nbhr = 0;
  ih = 0;
  id_ = 0;
}

template <Dimension D>
VoxelInteractionHandler<D>::VoxelInteractionHandler() {
  VoxelInteractionHandler<D>::initVars();
}

template <Dimension D>
VoxelInteractionHandler<D>::VoxelInteractionHandler(
    const VoxelInteractionHandler<D>& v) {
  VoxelInteractionHandler<D>::copy(v);
}

template <Dimension D>
void VoxelInteractionHandler<D>::copy(const VoxelInteractionHandler<D>& v) {
  current = v.current;
  nbhr = v.nbhr;
  ih = v.ih;
  id_ = v.id_;
}

template <Dimension D>
void VoxelInteractionHandler<D>::twoVoxelInteractions() const {
  VoxelInteractionHandler<D>::twoVoxelInteractions(*current, *nbhr);
}

template <Dimension D>
void VoxelInteractionHandler<D>::twoVoxelInteractions(Voxel<D>& v1,
                                                      Voxel<D>& v2) const {
  // This doesn't do an occupancy check since that should have been done
  // already.
  if (v1.index() == v2.index()) {
    return VoxelInteractionHandler<D>::sameVoxelInteractions(v1);
  } else {
    for (occupant_iterator ii = v1.begin(); ii != v1.end(); ++ii) {
      for (occupant_iterator jj = v2.begin(); jj != v2.end(); ++jj) {
        ih->interaction(*(*ii), *(*jj));
      }
    }
  }
}

template <Dimension D>
void VoxelInteractionHandler<D>::sameVoxelInteractions() const {
  VoxelInteractionHandler<D>::sameVoxelInteractions(*current);
}

template <Dimension D>
void VoxelInteractionHandler<D>::sameVoxelInteractions(Voxel<D>& v1) const {
  if (v1.occupancy() < 2) {
    return;
  }
  for (occupant_iterator ii = v1.begin(); ii != v1.end(); ++ii) {
    occupant_iterator current = ii;
    for (occupant_iterator jj = ++current; jj != v1.end(); ++jj) {
      ih->interaction(*(*ii), *(*jj));
    }
  }
}

template <Dimension D>
void VoxelInteractionHandler<D>::print(std::ostream& o) const {
  o << "VIH:" << id_ << "\tInteractionHandler:\n";
  ih->print(o);
}

template <Dimension D>
VoxelInteractionHandler<D>& VoxelInteractionHandler<D>::operator=(
    const VoxelInteractionHandler<D>& v) {
  VoxelInteractionHandler<D>::copy(v);
  return *this;
}

template <Dimension D>
bool VoxelInteractionHandler<D>::operator==(
    const VoxelInteractionHandler<D>& v) const {
  return id_ == v.id_;
}

template class VoxelInteractionHandler<k2Dimensions>;
template class VoxelInteractionHandler<k3Dimensions>;

}  // namespace lib
}  // namespace lgl
