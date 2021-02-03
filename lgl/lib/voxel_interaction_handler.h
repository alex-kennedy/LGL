#ifndef LGL_LIB_VOXEL_INTERACTION_HANDLER_H_
#define LGL_LIB_VOXEL_INTERACTION_HANDLER_H_

#include <iostream>

#include "particle_interaction_handler.h"
#include "types.h"
#include "voxel.h"

namespace lgl {
namespace lib {

template <Dimension D>
class VoxelInteractionHandler {
 private:
  typedef typename Voxel<D>::occupant_iterator occupant_iterator;

 protected:
  Voxel<D>* current;
  Voxel<D>* nbhr;
  ParticleInteractionHandler<D>* ih;
  int id_;
  Amutex mutex;

  void initVars();

 public:
  VoxelInteractionHandler();

  VoxelInteractionHandler(const VoxelInteractionHandler<D>& v);

  void copy(const VoxelInteractionHandler<D>& v);

  int lock() { return mutex.lock(); }
  int unlock() { return mutex.unlock(); }

  void interactionHandler(ParticleInteractionHandler<D>& i) { ih = &i; }
  ParticleInteractionHandler<D>& interactionHandler() const { return *ih; }

  void currentVoxel(Voxel<D>& v) { current = &v; }
  Voxel<D>& currentVoxel() { return *current; }

  void currentNbhrVoxel(Voxel<D>& v) { nbhr = &v; }
  Voxel<D>& currentNbhrVoxel() { return *nbhr; }

  void id(int i) { id_ = i; }
  int id() const { return id_; }

  void twoVoxelInteractions() const;

  void twoVoxelInteractions(Voxel<D>& v1, Voxel<D>& v2) const;

  void sameVoxelInteractions() const;

  void sameVoxelInteractions(Voxel<D>& v1) const;

  void print(std::ostream& o = std::cout) const;

  VoxelInteractionHandler<D>& operator=(const VoxelInteractionHandler<D>& v);

  bool operator==(const VoxelInteractionHandler<D>& v) const;

  ~VoxelInteractionHandler() {
    VoxelInteractionHandler<D>::initVars(); /* Don't delete anything */
  }
};

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_VOXEL_INTERACTION_HANDLER_H_
