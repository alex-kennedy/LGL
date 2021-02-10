#ifndef LGL_LIB_PARTICLE_INTERACTION_HANDLER_H_
#define LGL_LIB_PARTICLE_INTERACTION_HANDLER_H_

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "boost/algorithm/cxx11/all_of.hpp"
#include "particle.h"
#include "particle_stats.h"
#include "types.h"

namespace lgl {
namespace lib {

typedef std::vector<FloatType> EllipseFactors;

template <Dimension D>
class ParticleInteractionHandler {
 public:
  typedef typename Particle<D>::vec_type vec_type;

 protected:
  FloatType timeStep_;
  FloatType springConstant_;
  FloatType eqDistance_;
  FloatType eqDistanceSquared_ = 0;  // just for optimization
  FloatType forceConstraint_;
  FloatType noiseAmplitude_;
  EllipseFactors ellipseFactors_;
  int id_;

  void initVars();

 public:
  ParticleInteractionHandler();
  ParticleInteractionHandler(const ParticleInteractionHandler<D>& p);

  void copy(const ParticleInteractionHandler<D>& pi);

  int id() const { return id_; }
  void id(int i) { id_ = i; }

  FloatType timeStep() const { return timeStep_; }
  void timeStep(FloatType t) { timeStep_ = t; }

  FloatType springConstant() const { return springConstant_; }
  void springConstant(FloatType k) { springConstant_ = k; }

  FloatType eqDistance() const { return eqDistance_; }
  void eqDistance(FloatType e);

  void forceLimit(FloatType v) { forceConstraint_ = v; }
  FloatType forceLimit() const { return forceConstraint_; }

  void enforceFLimit(Particle<D>& p1) const;

  void noiseAmplitude(FloatType n) { noiseAmplitude_ = n; }
  FloatType noiseAmplitude() const { return noiseAmplitude_; }

  void addNoise(Particle<D>& p1) const;

  void ellipseFactors(EllipseFactors ef);

  void interaction(Particle<D>& p1, Particle<D>& p2) const;

  void springRepulsiveInteraction(Particle<D>& p1, Particle<D>& p2) const;

  void integrate(Particle<D>& p1) const;

  void integrate(Particle<D>& p1, FloatType t) const;

  void integrateFirstOrder(Particle<D>& p1, FloatType t) const;

  void integrateFirstOrder(Particle<D>& p1) const;

  void print(std::ostream& o = std::cout) const;

  ParticleInteractionHandler<D>& operator=(
      const ParticleInteractionHandler<D>& p);

  bool operator==(const ParticleInteractionHandler<D>& p) const;
};

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_PARTICLE_INTERACTION_HANDLER_H_
