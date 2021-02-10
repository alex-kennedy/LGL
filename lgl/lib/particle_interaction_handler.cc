#include "particle_interaction_handler.h"

namespace lgl {
namespace lib {

template <Dimension D>
void ParticleInteractionHandler<D>::initVars() {
  timeStep_ = 0;
  springConstant_ = 0;
  eqDistance_ = 0;
  eqDistanceSquared_ = 0;
  id_ = 0;
  forceConstraint_ = 0;
}

template <Dimension D>
ParticleInteractionHandler<D>::ParticleInteractionHandler() {
  ParticleInteractionHandler<D>::initVars();
}

template <Dimension D>
ParticleInteractionHandler<D>::ParticleInteractionHandler(
    const ParticleInteractionHandler<D>& p) {
  ParticleInteractionHandler<D>::copy(p);
}

template <Dimension D>
void ParticleInteractionHandler<D>::copy(
    const ParticleInteractionHandler<D>& pi) {
  timeStep_ = pi.timeStep_;
  springConstant_ = pi.springConstant_;
  eqDistance_ = pi.eqDistance_;
  eqDistanceSquared_ = pi.eqDistanceSquared_;
  noiseAmplitude_ = pi.noiseAmplitude_;
  id_ = pi.id_;
}

template <Dimension D>
void ParticleInteractionHandler<D>::eqDistance(FloatType e) {
  eqDistance_ = e;
  eqDistanceSquared_ = sqr(e);
}

template <Dimension D>
void ParticleInteractionHandler<D>::enforceFLimit(Particle<D>& p1) const {
  p1.lock();
  for (unsigned int d = 0; d < D; ++d) {
    if (p1.f[d] > forceConstraint_) {
      p1.f[d] = forceConstraint_;
    } else if (p1.f[d] < -1.0 * forceConstraint_) {
      p1.f[d] = -1.0 * forceConstraint_;
    }
  }
  p1.unlock();
}

template <Dimension D>
void ParticleInteractionHandler<D>::addNoise(Particle<D>& p1) const {
  FloatType factor = noiseAmplitude_;
  typename Particle<D>::vec_type noise;
  constexpr FloatType divisor = RAND_MAX + 1.0;
  constexpr int rand_half = (RAND_MAX + 1u) / 2;
  for (unsigned d = 0; d < D; ++d) {
    if (std::rand() < rand_half) factor = -factor;
    noise[d] = factor * (((FloatType)std::rand()) / divisor);
  }
  // no need to lock because particle's force constituents are atomic
  // p1.lock();
  p1.add2F(noise);
  // p1.unlock();
}

template <Dimension D>
void ParticleInteractionHandler<D>::ellipseFactors(EllipseFactors ef) {
  ellipseFactors_.swap(ef);
  if (boost::algorithm::all_of_equal(ellipseFactors_,
                                     EllipseFactors::value_type(1)))
    ellipseFactors_.clear();  // just an optimization
  if (ellipseFactors_.empty()) return;
  if (ellipseFactors_.size() < D)
    ellipseFactors_.resize(D, ellipseFactors_.back());
}

template <Dimension D>
void ParticleInteractionHandler<D>::interaction(Particle<D>& p1,
                                                Particle<D>& p2) const {
  if (euclideanDistanceSquared(p1.X().begin(), p1.X().end(), p2.X().begin()) <
      eqDistanceSquared_) {
    springRepulsiveInteraction(p1, p2);
  }
}

template <Dimension D>
void ParticleInteractionHandler<D>::springRepulsiveInteraction(
    Particle<D>& p1, Particle<D>& p2) const {
  const bool p1anchor = p1.isAnchor(), p2anchor = p2.isAnchor();
  if (p1.collisionCheck(p2)) {
    if (!p1anchor) addNoise(p1);
    if (!p2anchor || p1anchor) addNoise(p2);
    return;
  }

  vec_type x1 = p1.X();
  vec_type x2 = p2.X();

  for (std::size_t i = 0; i < ellipseFactors_.size(); ++i) {
    const EllipseFactors::value_type f = ellipseFactors_[i];
    x1[i] *= f;
    x2[i] *= f;
  }

  const FloatType magx1x2 = x1.distance(x2);
  const FloatType sepFromIdeal = (magx1x2 - eqDistance_);
  const FloatType scale = -springConstant_ * sepFromIdeal / magx1x2;

  vec_type f_;
  vec_type fm1_;

  for (unsigned int ii = 0; ii < D; ++ii) {
    FloatType dx = (x1[ii] - x2[ii]);

    // results in a bad image for "the internet"
    // if (dx > eqDistance_) dx = 1.0 / dx;

    FloatType f = dx * scale;
    f_[ii] = f;
    fm1_[ii] = -f;
  }

  if (!p1anchor) {
    if (p2anchor) f_.scale(2);
    // no need to lock because particle's force constituents are atomic
    // p1.lock();
    p1.add2F(f_);
    // p1.unlock();
  }

  if (!p2anchor) {
    if (p1anchor) fm1_.scale(2);
    // no need to lock because particle's force constituents are atomic
    // p2.lock();
    p2.add2F(fm1_);
    // p2.unlock();
  }
}

template <Dimension D>
void ParticleInteractionHandler<D>::integrate(Particle<D>& p1) const {
  ParticleInteractionHandler<D>::integrateFirstOrder(p1);
}

template <Dimension D>
void ParticleInteractionHandler<D>::integrate(Particle<D>& p1,
                                              FloatType t) const {
  ParticleInteractionHandler<D>::integrateFirstOrder(p1, t);
}

template <Dimension D>
void ParticleInteractionHandler<D>::integrateFirstOrder(Particle<D>& p1,
                                                        FloatType t) const {
  for (unsigned int ii = 0; ii < D; ++ii) {
    FloatType finc = p1.f[ii] * t;
    if (finc < 0) {
      finc = -std::min<FloatType>((FloatType).05, abs(finc));
    } else {
      finc = std::min<FloatType>((FloatType).05, finc);
    }
    p1.x[ii] += finc;
  }
}

template <Dimension D>
void ParticleInteractionHandler<D>::integrateFirstOrder(Particle<D>& p1) const {
  ParticleInteractionHandler<D>::integrateFirstOrder(p1, timeStep_);
}

template <Dimension D>
void ParticleInteractionHandler<D>::print(std::ostream& o) const {
  o << "PIH:" << id_ << "\tT: " << timeStep_ << "\tk:" << springConstant_
    << "\ta: " << eqDistance_ << '\n';
}

template <Dimension D>
ParticleInteractionHandler<D>& ParticleInteractionHandler<D>::operator=(
    const ParticleInteractionHandler<D>& p) {
  ParticleInteractionHandler<D>::copy(p);
  return *this;
}

template <Dimension D>
bool ParticleInteractionHandler<D>::operator==(
    const ParticleInteractionHandler<D>& p) const {
  return id_ == p.id_;
}

template class ParticleInteractionHandler<k2Dimensions>;
template class ParticleInteractionHandler<k3Dimensions>;

}  // namespace lib
}  // namespace lgl
