#include "voxel.h"

#include "cube.h"

namespace lgl {
namespace lib {

template <Dimension NDimensions>
void Voxel<NDimensions>::copy(const Voxel<NDimensions>& v) {
  Cube_::operator=(v);
  mutex = v.mutex;
  index_ = v.index_;
  interactionCtr_ = v.interactionCtr_;
  occupants = v.occupants;
}

template <Dimension NDimensions>
void Voxel<NDimensions>::print(std::ostream& o) const {
  o << "Vox: " << index_ << "\tOcc: " << occupants.size()
    << "\tCtr: " << interactionCtr_ << '\t';
  Cube<NDimensions>::print(o);
}

template <Dimension NDimensions>
const Voxel<NDimensions>& Voxel<NDimensions>::operator=(
    const Voxel<NDimensions>& v) {
  Voxel<NDimensions>::copy(v);
  return *this;
}

template class Voxel<k2Dimensions>;
template class Voxel<k3Dimensions>;

}  // namespace lib
}  // namespace lgl
