#include "particle.h"

#include "types.h"

namespace lgl {
namespace lib {

template <Dimension D>
void Particle<D>::reset_values() {
  x = 0;
  f = 0;
  container_ = -1;
  index_ = 0;
  id_ = "";
  isAnchor_ = false;
}

template <Dimension D>
bool Particle<D>::isPositionInitialized() const noexcept {
  // TODO 'initialize' and compare against a big number instead of 0?
  return !std::all_of(X().begin(), X().end(), [](float x) { return x == 0.f; });
}

template <Dimension D>
bool Particle<D>::collisionCheck(const Particle<D>& vp) const {
  return x.distanceSquared(vp.x) <= sqr(radius() + vp.radius());
}

template <Dimension D>
void Particle<D>::copy(const Particle<D>& p) {
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

template <Dimension D>
FloatType Particle<D>::separation2(const Particle<D>& p) const {
  return x.distanceSquared(p.x);
}

template <Dimension D>
FloatType Particle<D>::separation(const Particle<D>& p) const {
  return x.distance(p.x);
}

template <Dimension D>
void Particle<D>::printParticle(std::ostream& o) const {
  o << "Index: " << index_ << "\tId: " << id_ << '\t' << "M: " << mass_ << '\t'
    << "R: " << radius_ << "\tCont: " << container_ << '\n';
  o << "\tX: ";
  Particle<D>::printXCoords(o);
  o << '\n';
  o << "\tF: ";
  Particle<D>::printFCoords(o);
  o << '\n';
}

template <Dimension D>
void Particle<D>::printXCoords(std::ostream& o) const {
  o << x[0];
  for (unsigned int ii = 1; ii < D; ++ii) {
    o << " " << x[ii];
  }
}

template <Dimension D>
void Particle<D>::printFCoords(std::ostream& o) const {
  o << f[0];
  for (unsigned int ii = 1; ii < D; ++ii) {
    o << " " << f[ii];
  }
}

template <Dimension D>
void Particle<D>::printX(std::ostream& o) const {
  o << index_;
  for (unsigned int ii = 0; ii < D; ++ii) {
    o << " " << x[ii];
  }
  o << '\n';
}

template <Dimension D>
void Particle<D>::printF(std::ostream& o) const {
  o << id_;
  for (unsigned int ii = 0; ii < D; ++ii) {
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