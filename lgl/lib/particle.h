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
#ifndef LGL_LIB_PARTICLE_H_
#define LGL_LIB_PARTICLE_H_

#include <algorithm>
#include <boost/atomic.hpp>
#include <iostream>
#include <string>

#include "cube.h"
#include "fixedVec.hpp"
#include "fixedVecOperations.hpp"
#include "pthread_wrapper.h"

namespace lgl {
namespace lib {

template <Dimension NDimensions>
class Particle {
 private:
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
  // This is the number of the container the particle resides in.
  long container_ = -1;

  // Particle is spherical/circular.
  FloatType radius_;

  FloatType mass_;

  // The change in position per iteration.
  FloatType dx_;

  Amutex mutex;

  std::string id_;

  bool isAnchor_;

 public:
  // Position of the particle in x,y(,z), depending on the dimensions.
  FixedVec<FloatType, NDimensions> x;

  // Force in x,y(,z)
  atomic_vec_type f;

 public:
  Particle(const Particle<NDimensions>& p) { Particle<NDimensions>::copy(p); }

  Particle() { Particle<NDimensions>::reset_values(); }

  const FixedVec<FloatType, NDimensions>& X() const { return x; }

  const atomic_vec_type& F() const { return f; }

  const std::string& id() const { return id_; }

  void id(const string& i) { id_ = i; }

  bool isAnchor() const { return isAnchor_; }

  void markAnchor() { isAnchor_ = true; }

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

  void add2F(const FixedVec<FloatType, NDimensions>& f_) { f += f_; }

  void X(const FixedVec<FloatType, NDimensions>& x_) { x = x_; }

  template <typename InputIterator>
  void X(InputIterator begin, InputIterator end) {
    std::copy(begin, end, x.begin());
  }

  void F(const FixedVec<FloatType, NDimensions>& f_) { f = f_; }

  FloatType dx() const { return dx_; }
  void dx(FloatType d) { dx_ = d; }

  Particle<NDimensions>& operator=(const Particle<NDimensions>& p) {
    Particle<NDimensions>::copy(p);
    return *this;
  }

  // TODO: Equality by index seems dangerous. Should consider checking by
  // address.
  bool operator==(const Particle<NDimensions>& p) const {
    return index_ == p.index_;
  }

  bool operator!=(const Particle<NDimensions>& p) const {
    return index_ != p.index_;
  }

  bool collisionCheck(const Particle<NDimensions>& vp) const;

  bool isPositionInitialized() const noexcept;

  void reset_values();

  void copy(const Particle<NDimensions>& p);

  FloatType separation2(const Particle<NDimensions>& p) const;

  FloatType separation(const Particle<NDimensions>& p) const;

  // Print specific particle info.
  void printParticle(std::ostream& o = std::cout) const;
  void printXCoords(std::ostream& o = std::cout) const;
  void printFCoords(std::ostream& o = std::cout) const;
  void printX(std::ostream& o = std::cout) const;
  void printF(std::ostream& o = std::cout) const;
  void print(std::ostream& o = std::cout) const {
    Particle<NDimensions>::printParticle(o);
  }
};

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_PARTICLE_H_
