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
#ifndef _PARTICLE_HPP_
#define _PARTICLE_HPP_

//----------------------------------------------------

#include <algorithm>
#include <boost/atomic.hpp>
#include <iostream>
#include <string>

#include "aPthread.hpp"
#include "configs.h"
#include "fixedVec.hpp"
#include "fixedVecOperations.hpp"

namespace lgl {
namespace lib {

//----------------------------------------------------

enum Dimension { k2Dimensions = 2, k3Dimensions = 3 };
typedef float FloatType;

//----------------------------------------------------

template <Dimension NDimensions>
class Particle {
 private:
  typedef Particle<NDimensions> Particle_;
  unsigned int index_;  // Index of particle in array

 public:
  static const Dimension n_dimensions_ = NDimensions;
  typedef FloatType precision;
  typedef FixedVec<FloatType, NDimensions> vec_type;
  // using boost::atomic instead of std::atomic because the latter doesn't have
  // op+= or fetch_add et al. for floating-point specializations until C++20
  // TODO C++20: std::atomic
  typedef FixedVec<boost::atomic<FloatType>, NDimensions> atomic_vec_type;

 protected:
  long container_ = -1;  // This is the number of the container
                         // the particle resides in.
  FloatType radius_;     // Particle is spherical/circular
  FloatType mass_;
  FloatType dx_;  // The change in position per iteration

  Amutex mutex;
  std::string id_;
  bool isAnchor_;

 public:
  vec_type x;  // position in x,y,z
  // vec_type v; // velocity in x,y,z
  atomic_vec_type f;  // force in x,y,z

 public:
  Particle(const Particle_& p) { Particle_::copy(p); }
  Particle() { Particle_::resetValues(); }

  const vec_type& X() const { return x; }
  const atomic_vec_type& F() const { return f; }

  const std::string& id() const { return id_; }
  void id(const string& i) { id_ = i; }

  bool isAnchor() const { return isAnchor_; }
  void markAnchor() { isAnchor_ = true; }

  bool isPositionInitialized() const noexcept {
    // TODO 'initialize' and compare against a big number instead of 0?
    return !std::all_of(X().begin(), X().end(),
                        [](float x) { return x == 0.f; });
  }

  bool collisionCheck(const Particle_& vp) const {
    return x.distanceSquared(vp.x) <= sqr(radius() + vp.radius());
  }

  //  void incInteractionCtr( unsigned int i ) { interactionCtr_+=i; }
  // unsigned int interactionCtr() const { return interactionCtr_; }
  // void interactionCtr( unsigned int i ) { interactionCtr_=i; }

  void container(long c) { container_ = c; }
  long container() const { return container_; }

  int lock() { return mutex.lock(); }
  int unlock() { return mutex.unlock(); }

  void index(unsigned int i) { index_ = i; }
  unsigned int index() const { return index_; }

  void mass(FloatType m) { mass_ = m; }
  FloatType mass() { return mass_; }

  void radius(FloatType r) { radius_ = r; }
  FloatType radius() const { return radius_; }

  void add2F(const vec_type& f_) { f += f_; }

  void X(const vec_type& x_) { x = x_; }

  template <typename InputIterator>
  void X(InputIterator begin, InputIterator end) {
    std::copy(begin, end, x.begin());
  }

  // void V( const vec_type& v_ ) { v=v_; }
  void F(const vec_type& f_) { f = f_; }

  void resetValues() {
    // interactionCtr_=0;
    x = 0;
    f = 0;
    container_ = -1;
    index_ = 0;
    id_ = "";
    // v=0;
    isAnchor_ = false;
  }

  FloatType dx() const { return dx_; }
  void dx(FloatType d) { dx_ = d; }

  // FloatType magV() const { return v.magnitude(); }

  FloatType separation2(const Particle_& p) const {
    return x.distanceSquared(p.x);
  }

  FloatType separation(const Particle_& p) const { return x.distance(p.x); }

  void copy(const Particle_& p) {
    if (*this == p)
      return;  // TODO: really? compare objects (in this case indices) instead
               // of addresses?
    index_ = p.index_;
    radius_ = p.radius_;
    mass_ = p.mass_;
    x = p.x;
    f = p.f;
    // v=p.v;
    mutex = p.mutex;
    isAnchor_ = p.isAnchor_;
  }

  // Print specific particle info.
  void printParticle(std::ostream& o = std::cout) const;
  void printXCoords(std::ostream& o = std::cout) const;
  // void printVCoords(std::ostream& o=std::cout) const;
  void printFCoords(std::ostream& o = std::cout) const;
  void printX(std::ostream& o = std::cout) const;
  // void printV(std::ostream& o=std::cout) const;
  void printF(std::ostream& o = std::cout) const;
  void print(std::ostream& o = std::cout) const { Particle_::printParticle(o); }

  Particle_& operator=(const Particle_& p) {
    Particle_::copy(p);
    return *this;
  }

  bool operator==(const Particle_& p) const { return index_ == p.index_; }

  bool operator!=(const Particle_& p) const { return index_ != p.index_; }

  /*    friend std::ostream&  */
  /*      operator<< ( std::ostream& o , const Particle_& p ); */
};

//----------------------------------------------------

template <Dimension NDimensions>
void Particle<NDimensions>::printParticle(std::ostream& o) const {
  o << "Index: " << index_ << "\tId: " << id_ << '\t' << "M: " << mass_ << '\t'
    << "R: " << radius_ << "\tCont: " << container_ << '\n';
  ;
  o << "\tX: ";
  Particle_::printXCoords(o);
  o << '\n';
  // o << "\tV: "; Particle_::printVCoords(o); o << '\n';
  o << "\tF: ";
  Particle_::printFCoords(o);
  o << '\n';
}

//----------------------------------------------------

template <Dimension NDimensions>
void Particle<NDimensions>::printXCoords(std::ostream& o) const {
  o << x[0];
  for (unsigned int ii = 1; ii < NDimensions; ++ii) {
    o << " " << x[ii];
  }
}

//----------------------------------------------------

#if 0
template <Dimension NDimensions>
void Particle<FloatType, NDimensions>::printVCoords(std::ostream& o) const {
  o << v[0];
  for (unsigned int ii = 1; ii < NDimensions; ++ii) {
    o << " " << v[ii];
  }
}
#endif

//----------------------------------------------------

template <Dimension NDimensions>
void Particle<NDimensions>::printFCoords(std::ostream& o) const {
  o << f[0];
  for (unsigned int ii = 1; ii < NDimensions; ++ii) {
    o << " " << f[ii];
  }
}

//----------------------------------------------------

template <Dimension NDimensions>
void Particle<NDimensions>::printX(std::ostream& o) const {
  o << index_;
  for (unsigned int ii = 0; ii < NDimensions; ++ii) {
    o << " " << x[ii];
  }
  o << '\n';
}

//----------------------------------------------------

#if 0
template <Dimension NDimensions>
void Particle<NDimensions>::printV(std::ostream& o) const {
  o << id;
  for (unsigned int ii = 0; ii < NDimensions; ++ii) {
    o << " " << v[ii];
  }
  o << '\n';
}
#endif

//----------------------------------------------------

template <Dimension NDimensions>
void Particle<NDimensions>::printF(std::ostream& o) const {
  o << id_;
  for (unsigned int ii = 0; ii < NDimensions; ++ii) {
    o << '\t' << f[ii];
  }
  o << '\n';
}

//----------------------------------------------------

template <typename Particle>
typename Particle::precision euclideanDistance(const Particle& p1,
                                               const Particle& p2) {
  return euclideanDistance(p1.X().begin(), p1.X().end(), p2.X().begin());
}

//----------------------------------------------------

}  // namespace lib
}  // namespace lgl

#endif
