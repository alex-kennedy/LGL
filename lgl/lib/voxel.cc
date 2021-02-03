#include "voxel.h"

#include "cube.h"

namespace lgl {
namespace lib {

template <Dimension D>
void Voxel<D>::copy(const Voxel<D>& v) {
  Cube<D>::operator=(v);
  mutex = v.mutex;
  index_ = v.index_;
  interactionCtr_ = v.interactionCtr_;
  occupants = v.occupants;
}

template <Dimension D>
void Voxel<D>::print(std::ostream& o) const {
  o << "Vox: " << index_ << "\tOcc: " << occupants.size()
    << "\tCtr: " << interactionCtr_ << '\t';
  Cube<D>::print(o);
}

template <Dimension D>
const Voxel<D>& Voxel<D>::operator=(const Voxel<D>& v) {
  Voxel<D>::copy(v);
  return *this;
}

template class Voxel<k2Dimensions>;
template class Voxel<k3Dimensions>;

}  // namespace lib
}  // namespace lgl
