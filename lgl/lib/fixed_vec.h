#ifndef LGL_LIB_FIXED_VEC_H_
#define LGL_LIB_FIXED_VEC_H_

#include <cmath>
#include <iostream>
#include <utility>

#include "types.h"

namespace lgl {
namespace lib {

template <typename T>
inline auto sqr(T t) -> decltype(t * t) {
  return t * t;
}

template <typename T, Dimension D>
class FixedVec {
 public:
  using FloatType = decltype(
      std::declval<T&>() + std::declval<T&>());  // this handles atomics as well
                                                 // as raw arithmetic types
  typedef FloatType precision;
  typedef unsigned int size_type;
  typedef T* iterator;
  typedef const T* const_iterator;

 private:
  typedef FixedVec<T, D> Vec_;

  T orig[D];  // The origin

 public:
  // CONSTRUCTORS
  FixedVec() {}
  FixedVec(FloatType v) { Vec_::constant(v); }
  FixedVec(const Vec_& p) { Vec_::copy(p); }

  // ACCESSORS
  size_type size() const { return D; }
  iterator begin() { return orig; }
  const_iterator begin() const { return orig; }
  iterator end() { return &orig[0] + D; }
  const_iterator end() const { return &orig[0] + D; }

  // MUTATORS
  template <typename U>
  void copy(const FixedVec<U, D>& p) {
    for (size_type ii = 0; ii < D; ++ii) orig[ii] = p[ii];
  }

  // OPERATIONS
  void translate(const Vec_& p) {
    for (size_type ii = 0; ii < D; ++ii) orig[ii] += p[ii];
  }

  void translate(FloatType x) {
    for (size_type ii = 0; ii < D; ++ii) orig[ii] += x;
  }

  void subtract(FloatType x) {
    for (size_type ii = 0; ii < D; ++ii) orig[ii] -= x;
  }

  void scale(const Vec_& p) {
    for (size_type ii = 0; ii < D; ++ii) orig[ii] *= p[ii];
  }

  void scale(FloatType x) {
    for (size_type ii = 0; ii < D; ++ii) orig[ii] *= x;
  }

  void constant(FloatType x) {
    for (size_type ii = 0; ii < D; ++ii) orig[ii] = x;
  }

  FloatType magnitudeSquared() const {
    FloatType mag = 0;
    for (size_type ii = 0; ii < D; ++ii) mag += sqr((*this)[ii]);
    return mag;
  }

  FloatType magnitude() const { return sqrt(Vec_::magnitudeSquared()); }

  FloatType distanceSquared(const Vec_& p) const {
    FloatType distance = 0;
    for (size_type ii = 0; ii < D; ++ii) {
      FloatType dx = orig[ii] - p[ii];
      distance += sqr(dx);
    }
    return distance;
  }

  FloatType distance(const Vec_& p) const {
    return sqrt(Vec_::distanceSquared(p));
  }

  FloatType sum() const {
    FloatType sum = 0;
    for (size_type ii = 0; ii < D; ++ii) sum += orig[ii];
    return sum;
  }

  FloatType average() const { return this->sum() / static_cast<FloatType>(D); }

  FloatType product() const {
    FloatType product = 1;
    for (size_type ii = 0; ii < D; ++ii) product *= orig[ii];
    return product;
  }

  FloatType dotProduct(const Vec_& v) const {
    FloatType product = 0;
    for (size_type ii = 0; ii < D; ++ii) product += orig[ii] * v[ii];
    return product;
  }

  // STREAMS
  std::istream& loadFromStream(std::istream& i = std::cin) {
    for (size_type ii = 0; ii < D; ++ii) {
      i >> orig[ii];
    }
    return i;
  }

  std::ostream& print(std::ostream& o = std::cout) const {
    o << orig[0];
    for (size_type ii = 1; ii < D; ++ii) o << " " << orig[ii];
    o << '\n';
    return o;
  }

  // OPERATORS
  Vec_& operator=(const Vec_& p2) {
    Vec_::copy(p2);
    return *this;
  }

  template <typename U>
  Vec_& operator=(const FixedVec<U, D>& p2) {
    Vec_::copy(p2);
    return *this;
  }

  void operator-=(const Vec_& p2) {
    for (size_type ii = 0; ii < D; ++ii) orig[ii] -= p2[ii];
  }

  template <typename U>
  void operator+=(const FixedVec<U, D>& p2) {
    for (size_type ii = 0; ii < D; ++ii) orig[ii] += p2[ii];
  }

  Vec_& operator=(FloatType p) {
    Vec_::constant(p);
    return *this;
  }

  bool operator==(const Vec_& p2) const {
    for (size_type ii = 0; ii < D; ++ii) {
      if (orig[ii] != p2[ii]) return 0;
    }
    return 1;
  }

  bool operator!=(const Vec_& p2) const { return !(*this == p2); }

  T& operator[](size_type i) { return orig[i]; }

  FloatType operator[](size_type i) const { return orig[i]; }
};

template <typename Iterator1, typename precision>
precision sum(Iterator1 begin1, Iterator1 end1) {
  precision sum = 0;
  for (; begin1 != end1; ++begin1) {
    sum += *begin1;
  }
  return sum;
}

template <typename Iterator1>
double magnitude(Iterator1 begin, Iterator1 end) {
  double sum(0);
  for (; begin != end; ++begin) {
    sum += *begin * (*begin);
  }
  return sqrt(sum);
}

template <typename Iterator>
double euclideanDistanceSquared(Iterator begin1, Iterator end1,
                                Iterator begin2) {
  double d = 0;
  for (; begin1 != end1; ++begin1, ++begin2) {
    double dx = *begin2 - *begin1;
    d += dx * dx;
  }
  return d;
}

template <typename Iterator>
double euclideanDistance(Iterator begin1, Iterator end1, Iterator begin2) {
  return sqrt(euclideanDistanceSquared(begin1, end1, begin2));
}

template <typename ForwardIterator>
void translate(ForwardIterator begin, ForwardIterator end, double t) {
  for (; begin != end; ++begin) *begin += t;
}

template <typename Iterator1, typename Iterator2>
void translate(Iterator1 begin1, Iterator1 end1, Iterator2 begin2) {
  for (; begin1 != end1; ++begin1, ++begin2) {
    (*begin1) += (*begin2);
  }
}

template <typename Iterator1, typename precision>
void scale(Iterator1 begin, Iterator1 end, precision f) {
  for (; begin != end; ++begin) {
    *begin *= f;
  }
}

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_FIXEDVEC_H_