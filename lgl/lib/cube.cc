#include "cube.h"

#include <iostream>

#include "fixed_vec.h"

namespace lgl {
namespace lib {

template <Dimension D>
Cube<D>::Cube() : origin_(0) {
  radius_ = 0;
}

template <Dimension D>
Cube<D>::Cube(const VectorType& p, FloatType r) : origin_(p), radius_(r) {}

template <Dimension D>
Cube<D>::Cube(const Cube<D>& c) {
  Cube<D>::copy(c);
}

template <Dimension D>
FloatType Cube<D>::radius() const {
  return radius_;
}

template <Dimension D>
const typename Cube<D>::VectorType& Cube<D>::origin() const {
  return origin_;
}

template <Dimension D>
void Cube<D>::radius(FloatType r) {
  radius_ = r;
}

template <Dimension D>
void Cube<D>::origin(const Cube<D>::VectorType& p) {
  origin_ = p;
}

template <Dimension D>
void Cube<D>::copy(const Cube<D>& c) {
  radius_ = c.radius_;
  origin_ = c.origin_;
}

template <Dimension D>
bool Cube<D>::check_inclusion(const Cube<D>::VectorType& p) const {
  for (int i = 0; i < D; ++i) {
    if (p[i] < (origin_[i] - radius_) || p[i] > (origin_[i] + radius_)) {
      return 0;
    }
  }
  return 1;
}

template <Dimension D>
bool Cube<D>::check_inclusion_fuzzy(const Cube<D>::VectorType& p) const {
  for (int i = 0; i < D; ++i) {
    if (p[i] < (origin_[i] - radius_ - .001) ||
        p[i] > (origin_[i] + radius_ + .001)) {
      return 0;
    }
  }
  return 1;
}

template <Dimension D>
void Cube<D>::print(std::ostream& o) const {
  o << "Rad: " << radius_ << "\tAt: ";
  origin_.print(o);
}

template <Dimension D>
Cube<D>& Cube<D>::operator=(const Cube<D>& c) {
  Cube<D>::copy(c);
  return *this;
}

template <Dimension D>
bool Cube<D>::operator==(const Cube<D>& c) const {
  return (radius_ == c.radius_ && origin_ == c.origin_);
}

template <Dimension D>
bool Cube<D>::operator!=(const Cube<D>& c) const {
  return !(*this == c);
}

template class Cube<k2Dimensions>;
template class Cube<k3Dimensions>;

}  // namespace lib
}  // namespace lgl
