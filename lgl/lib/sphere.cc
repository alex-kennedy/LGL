#include "sphere.h"

#include "types.h"

namespace lgl {
namespace lib {

void Sphere::print(std::ostream& o) const {
  o << "ID: " << ID() << "\tR: " << radius_ << "\tX: ";
  std::copy(x.begin(), x.end(), std::ostream_iterator<FloatType>(o, " "));
  o << '\n';
}

void Sphere::printBasic(std::ostream& o) const {
  o << ID() << " ";
  std::copy(x.begin(), x.end(), std::ostream_iterator<FloatType>(o, " "));
  o << '\n';
}

Sphere& Sphere::operator=(const Sphere& s) {
  id = s.ID();
  x = s.location();
  radius_ = s.radius();
  return *this;
}

}  // namespace lib
}  // namespace lgl
