#include "particle.h"

#include "cube.h"

namespace lgl {
namespace lib {

template <Dimension NDimensions>
void Particle<NDimensions>::reset_values() {
  x = 0;
  f = 0;
  container_ = -1;
  index_ = 0;
  id_ = "";
  isAnchor_ = false;
}

template <Dimension NDimensions>
bool Particle<NDimensions>::isPositionInitialized() const noexcept {
  // TODO 'initialize' and compare against a big number instead of 0?
  return !std::all_of(X().begin(), X().end(), [](float x) { return x == 0.f; });
}

template <Dimension NDimensions>
bool Particle<NDimensions>::collisionCheck(
    const Particle<NDimensions>& vp) const {
  return x.distanceSquared(vp.x) <= sqr(radius() + vp.radius());
}

template <Dimension NDimensions>
void Particle<NDimensions>::copy(const Particle<NDimensions>& p) {
  if (*this == p) {
    return;
  }
  index_ = p.index_;
  radius_ = p.radius_;
  mass_ = p.mass_;
  x = p.x;
  f = p.f;
  mutex = p.mutex;
  isAnchor_ = p.isAnchor_;
}

template <Dimension NDimensions>
FloatType Particle<NDimensions>::separation2(
    const Particle<NDimensions>& p) const {
  return x.distanceSquared(p.x);
}

template <Dimension NDimensions>
FloatType Particle<NDimensions>::separation(
    const Particle<NDimensions>& p) const {
  return x.distance(p.x);
}

template <Dimension NDimensions>
void Particle<NDimensions>::printParticle(std::ostream& o) const {
  o << "Index: " << index_ << "\tId: " << id_ << '\t' << "M: " << mass_ << '\t'
    << "R: " << radius_ << "\tCont: " << container_ << '\n';
  o << "\tX: ";
  Particle<NDimensions>::printXCoords(o);
  o << '\n';
  o << "\tF: ";
  Particle<NDimensions>::printFCoords(o);
  o << '\n';
}

template <Dimension NDimensions>
void Particle<NDimensions>::printXCoords(std::ostream& o) const {
  o << x[0];
  for (unsigned int ii = 1; ii < NDimensions; ++ii) {
    o << " " << x[ii];
  }
}

template <Dimension NDimensions>
void Particle<NDimensions>::printFCoords(std::ostream& o) const {
  o << f[0];
  for (unsigned int ii = 1; ii < NDimensions; ++ii) {
    o << " " << f[ii];
  }
}

template <Dimension NDimensions>
void Particle<NDimensions>::printX(std::ostream& o) const {
  o << index_;
  for (unsigned int ii = 0; ii < NDimensions; ++ii) {
    o << " " << x[ii];
  }
  o << '\n';
}

template <Dimension NDimensions>
void Particle<NDimensions>::printF(std::ostream& o) const {
  o << id_;
  for (unsigned int ii = 0; ii < NDimensions; ++ii) {
    o << '\t' << f[ii];
  }
  o << '\n';
}

template <typename Particle>
FloatType euclideanDistance(const Particle& p1, const Particle& p2) {
  return euclideanDistance(p1.X().begin(), p1.X().end(), p2.X().begin());
}

template class Particle<k2Dimensions>;
template class Particle<k3Dimensions>;

}  // namespace lib
}  // namespace lgl