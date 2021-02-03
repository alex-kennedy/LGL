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
#include "cube.h"

#include <iostream>

#include "fixed_vec.h"

namespace lgl {
namespace lib {

template <Dimension NDimensions>
Cube<NDimensions>::Cube() : origin_(0) {
  radius_ = 0;
}

template <Dimension NDimensions>
Cube<NDimensions>::Cube(const VectorType& p, FloatType r)
    : origin_(p), radius_(r) {}

template <Dimension NDimensions>
Cube<NDimensions>::Cube(const Cube<NDimensions>& c) {
  Cube<NDimensions>::copy(c);
}

template <Dimension NDimensions>
FloatType Cube<NDimensions>::radius() const {
  return radius_;
}

template <Dimension NDimensions>
const typename Cube<NDimensions>::VectorType& Cube<NDimensions>::origin()
    const {
  return origin_;
}

template <Dimension NDimensions>
void Cube<NDimensions>::radius(FloatType r) {
  radius_ = r;
}

template <Dimension NDimensions>
void Cube<NDimensions>::origin(const Cube<NDimensions>::VectorType& p) {
  origin_ = p;
}

template <Dimension NDimensions>
void Cube<NDimensions>::copy(const Cube<NDimensions>& c) {
  radius_ = c.radius_;
  origin_ = c.origin_;
}

template <Dimension NDimensions>
bool Cube<NDimensions>::check_inclusion(
    const Cube<NDimensions>::VectorType& p) const {
  for (int i = 0; i < NDimensions; ++i) {
    if (p[i] < (origin_[i] - radius_) || p[i] > (origin_[i] + radius_)) {
      return 0;
    }
  }
  return 1;
}

template <Dimension NDimensions>
bool Cube<NDimensions>::check_inclusion_fuzzy(
    const Cube<NDimensions>::VectorType& p) const {
  for (int i = 0; i < NDimensions; ++i) {
    if (p[i] < (origin_[i] - radius_ - .001) ||
        p[i] > (origin_[i] + radius_ + .001)) {
      return 0;
    }
  }
  return 1;
}

template <Dimension NDimensions>
void Cube<NDimensions>::print(std::ostream& o) const {
  o << "Rad: " << radius_ << "\tAt: ";
  origin_.print(o);
}

template <Dimension NDimensions>
Cube<NDimensions>& Cube<NDimensions>::operator=(const Cube<NDimensions>& c) {
  Cube<NDimensions>::copy(c);
  return *this;
}

template <Dimension NDimensions>
bool Cube<NDimensions>::operator==(const Cube<NDimensions>& c) const {
  return (radius_ == c.radius_ && origin_ == c.origin_);
}

template <Dimension NDimensions>
bool Cube<NDimensions>::operator!=(const Cube<NDimensions>& c) const {
  return !(*this == c);
}

template class Cube<k2Dimensions>;
template class Cube<k3Dimensions>;

}  // namespace lib
}  // namespace lgl
