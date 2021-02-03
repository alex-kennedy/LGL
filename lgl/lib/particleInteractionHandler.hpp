/*
 *
 *  Copyright (c) 2002 Alex Adai, All Rights Reserved.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *  MA 02111-1307 USA
 *
 */
#ifndef LGL_LIB_PARTICLE_INTERACTION_HANDLER_HPP_
#define LGL_LIB_PARTICLE_INTERACTION_HANDLER_HPP_

//---------------------------------------------

#include <boost/algorithm/cxx11/all_of.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "particle.h"
#include "particleStats.hpp"
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

#if 0  // unused function, if becomes used in the future it needs to change
  // to account for Particle::F()[ii] being atomic
  // This encourages pass throughs by giving a boost to whatever
  // direction particles were going in the first place
  void handleCollision(Particle<D>& p1, Particle<D>& p2) const {
    const vec_type& f1 = p1.F();
    const vec_type& f2 = p2.F();
    const FloatType mag1 = p1.F().magnitude();
    FloatType mag1m1 = 0;
    if (mag1 > .01) {
      mag1m1 = 1.0 / mag1;
    } else {
      mag1m1 = .1;
    }
    const FloatType mag2 = p2.F().magnitude();
    FloatType mag2m1 = 0;
    if (mag2 > .01) {
      mag2m1 = 1.0 / mag2;
    } else {
      mag2m1 = .1;
    }

    vec_type f1_;
    vec_type f2_;

    for (unsigned int ii = 0; ii < D; ++ii) {
      f1_[ii] = -.1 * (f1[ii] * mag1m1);
      f2_[ii] = -.1 * (f2[ii] * mag2m1);
    }

    p1.lock();
    p1.f += f1_;
    p1.unlock();

    p2.lock();
    p2.f += f2_;
    p2.unlock();
  }
#endif

  void initVars() {
    timeStep_ = 0;
    springConstant_ = 0;
    eqDistance_ = 0;
    eqDistanceSquared_ = 0;
    id_ = 0;
    forceConstraint_ = 0;
  }

 public:
  ParticleInteractionHandler() { ParticleInteractionHandler<D>::initVars(); }
  ParticleInteractionHandler(const ParticleInteractionHandler<D>& p) {
    ParticleInteractionHandler<D>::copy(p);
  }

  void copy(const ParticleInteractionHandler<D>& pi) {
    timeStep_ = pi.timeStep_;
    springConstant_ = pi.springConstant_;
    eqDistance_ = pi.eqDistance_;
    eqDistanceSquared_ = pi.eqDistanceSquared_;
    noiseAmplitude_ = pi.noiseAmplitude_;
    id_ = pi.id_;
  }

  int id() const { return id_; }
  void id(int i) { id_ = i; }

  FloatType timeStep() const { return timeStep_; }
  void timeStep(FloatType t) { timeStep_ = t; }

  FloatType springConstant() const { return springConstant_; }
  void springConstant(FloatType k) { springConstant_ = k; }

  FloatType eqDistance() const { return eqDistance_; }
  void eqDistance(FloatType e) {
    eqDistance_ = e;
    eqDistanceSquared_ = sqr(e);
  }

  void forceLimit(FloatType v) { forceConstraint_ = v; }
  FloatType forceLimit() const { return forceConstraint_; }

  void enforceFLimit(Particle<D>& p1) const {
    p1.lock();
    for (unsigned int d = 0; d < D; ++d) {
      if (p1.f[d] > forceConstraint_) {
        p1.f[d] = forceConstraint_;
      } else if (p1.f[d] < -1.0 * forceConstraint_) {
        p1.f[d] = -1.0 * forceConstraint_;
      }
    }
    p1.unlock();
    // p1.print(cout);
  }

  void noiseAmplitude(FloatType n) { noiseAmplitude_ = n; }
  FloatType noiseAmplitude() const { return noiseAmplitude_; }

  void addNoise(Particle<D>& p1) const {
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

  void ellipseFactors(EllipseFactors ef) {
    ellipseFactors_.swap(ef);
    if (boost::algorithm::all_of_equal(ellipseFactors_,
                                       EllipseFactors::value_type(1)))
      ellipseFactors_.clear();  // just an optimization
    if (ellipseFactors_.empty()) return;
    if (ellipseFactors_.size() < D)
      ellipseFactors_.resize(D, ellipseFactors_.back());
  }

  void interaction(Particle<D>& p1, Particle<D>& p2) const {
    if (euclideanDistanceSquared(p1.X().begin(), p1.X().end(), p2.X().begin()) <
        eqDistanceSquared_) {
      springRepulsiveInteraction(p1, p2);
    }
  }

  void springRepulsiveInteraction(Particle<D>& p1, Particle<D>& p2) const {
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
#if 0  // results in a bad image for "the internet"
      if (dx > eqDistance_) dx = 1.0 / dx;
#endif
      FloatType f = dx * scale;
      f_[ii] = f;
      fm1_[ii] = -f;
    }

    //     cout << "d: " << magx1x2 << "\tsep: " << sepFromIdeal << "\tSc: "
    // 	 << scale << "\tEQ: " << eqDistance_ << endl;
    //     cout << "B4" << endl;
    //     p1.print();
    //     p2.print();

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

    //    cout << "AFTER" << endl;
    //    p1.print();
    //    p2.print();
  }

  void integrate(Particle<D>& p1) const {
    ParticleInteractionHandler<D>::integrateFirstOrder(p1);
  }

  void integrate(Particle<D>& p1, FloatType t) const {
    ParticleInteractionHandler<D>::integrateFirstOrder(p1, t);
  }

  void integrateFirstOrder(Particle<D>& p1, FloatType t) const {
    for (unsigned int ii = 0; ii < D; ++ii) {
      FloatType finc = p1.f[ii] * t;
      if (finc < 0) {
        finc = -min<FloatType>((FloatType).05, abs(finc));
      } else {
        finc = min<FloatType>((FloatType).05, finc);
      }
      p1.x[ii] += finc;
    }
  }

  void integrateFirstOrder(Particle<D>& p1) const {
    ParticleInteractionHandler<D>::integrateFirstOrder(p1, timeStep_);
  }

  void print(std::ostream& o = std::cout) const {
    o << "PIH:" << id_ << "\tT: " << timeStep_ << "\tk:" << springConstant_
      << "\ta: " << eqDistance_ << '\n';
  }

  ParticleInteractionHandler<D>& operator=(
      const ParticleInteractionHandler<D>& p) {
    ParticleInteractionHandler<D>::copy(p);
    return *this;
  }

  bool operator==(const ParticleInteractionHandler<D>& p) const {
    return id_ == p.id_;
  }
};

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_PARTICLE_INTERACTION_HANDLER_HPP_
