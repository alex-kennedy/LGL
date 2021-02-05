#ifndef LGL_LIB_SPHERE_H_
#define LGL_LIB_SPHERE_H_

#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <iterator>
#include <string>
#include <vector>

#include "fixed_vec.h"

namespace lgl {
namespace lib {

class Sphere {
 public:
  typedef std::vector<FloatType> vec_type;

 private:
  std::string id;
  vec_type x;  // Location in Space
  FloatType radius_;

 public:
  Sphere() : x(2, 0) {}
  Sphere(const std::string& i, const vec_type& v, FloatType r = 1)
      : id(i), x(v), radius_(r) {}
  Sphere(const vec_type& v, FloatType r = 1) : x(v), radius_(r) {}

  int dimension() const { return x.size(); }
  const vec_type& location() const { return x; }
  vec_type& location() { return x; }
  FloatType radius() const { return radius_; }
  void radius(FloatType r) { radius_ = r; }
  void ID(const std::string& s) { id = s; }
  const std::string& ID() const { return id; }

  template <typename iterator>
  void location(iterator b, iterator e) {
    x.assign(b, e);
  }

  void print(std::ostream& o = std::cout) const;

  void printBasic(std::ostream& o = std::cout) const;

  Sphere& operator=(const Sphere& s);
};

template <typename Vectype>
inline void seriesOfPointsOnSphere(Sphere& s, int count, Vectype& v) {
  v.resize(count);
  for (int ii = 0; ii < count; ++ii) randomPointOnSurface(s, v.at(ii));
}

inline bool doIntersect(const Sphere& s1, const Sphere& s2) {
  typedef typename Sphere::vec_type vec_type;
  const vec_type& x1 = s1.location();
  const vec_type& x2 = s2.location();
  FloatType min = s1.radius() + s2.radius();
  FloatType d = euclideanDistance(x1.begin(), x1.end(), x2.begin());
  return d < min;
}

template <typename Vectype>
inline void uniform_on_sphere_vec(Vectype& v, int dimension) {
  // Assume radius = 1
  typedef typename Vectype::value_type value_type;
  const value_type PI = 3.141592654;
  v.resize(dimension);
  if (dimension == 2) {
    value_type theta = rand() / (RAND_MAX + 1.0) * 2.0 * PI;
    v.at(0) = cos(theta);  // X
    v.at(1) = sin(theta);  // Y
  } else if (dimension == 3) {
    value_type theta = 2.0 * PI * rand() / (RAND_MAX + 1.0);
    value_type phi = acos(1.0 - 2.0 * rand() / (RAND_MAX + 1.0));
    v.at(0) = cos(theta) * sin(phi);  // X
    v.at(1) = sin(theta) * sin(phi);  // Y
    v.at(2) = cos(phi);               // Z
  } else {
    std::cerr << "Unsupported dimension: must be 2 or 3\n";
    exit(EXIT_FAILURE);
  }
}

inline void randomPointOnSurface(const Sphere& s,
                                 typename Sphere::vec_type& r) {
#if 1
  uniform_on_sphere_vec(r, s.dimension());
#else
  // TODO use this instead of the hand-made code?
  typedef typename boost::hellekalek1995 FloatType;
  FloatType rr(++randomPointOnSurfaceCall___);
  boost::uniform_on_sphere<> usph(s.dimension());
  std::vector<double> v = usph(rr);
  std::copy(v.begin(), v.end(), std::ostream_iterator<double>(std::cerr, " "));
  std::cerr << '\n';
  typename Sphere::vec_type r(v.begin(), v.end());
#endif
  // Scale the vec to match the radius of the sphere
  scale(r.begin(), r.end(), s.radius());
  // Recenter for the sphere
  translate(r.begin(), r.end(), s.location().begin());
}

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_SPHERE_HPP_
