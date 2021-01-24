#ifndef LGL_LIB_CUBE_H_
#define LGL_LIB_CUBE_H_

#include "fixedVec.hpp"

namespace lgl {
namespace lib {

// TODO(alexkennedy): Remove when more header files are split
enum Dimension { k2Dimensions = 2, k3Dimensions = 3 };
typedef float FloatType;

template <Dimension NDimensions>
class Cube {
 private:
  using VectorType = FixedVec<FloatType, NDimensions>;

 protected:
  VectorType origin_;
  FloatType radius_;  // edgeLength = 2 * radius

 public:
  Cube();

  Cube(const VectorType& p, FloatType r);

  Cube(const Cube<NDimensions>& c);

  FloatType radius() const;

  const VectorType& origin() const;

  void radius(FloatType r);

  void origin(const VectorType& p);

  void copy(const Cube<NDimensions>& c);

  bool check_inclusion(const VectorType& p) const;

  bool check_inclusion_fuzzy(const VectorType& p) const;

  void print(std::ostream& o = std::cout) const;

  Cube<NDimensions>& operator=(const Cube<NDimensions>& c);

  bool operator==(const Cube<NDimensions>& c) const;

  bool operator!=(const Cube<NDimensions>& c) const;
};

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_CUBE_H_