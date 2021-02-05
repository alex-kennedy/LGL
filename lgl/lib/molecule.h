#ifndef LGL_LIB_MOLECULE_H_
#define LGL_LIB_MOLECULE_H_

#include <boost/limits.hpp>
#include <boost/random/uniform_on_sphere.hpp>
#include <boost/tokenizer.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "types.h"
#include "fixed_vec.h"
#include "sphere.h"

namespace lgl {
namespace lib {

template <Dimension D>
class Molecule {
 public:
  typedef Sphere particle_type;
  typedef typename std::vector<particle_type> particle_holder;
  typedef typename particle_holder::size_type size_type;
  typedef typename particle_holder::iterator iterator;
  typedef typename particle_holder::const_iterator const_iterator;
  typedef typename std::vector<FloatType> vec_type;

 private:
  particle_holder particles;
  std::string id;
  vec_type maxs;
  vec_type mins;

  void resetMinsMaxs();

  void init(size_type d);

  void min_max_test(const particle_type& s);

  void translateParticle(size_type ii, const vec_type& m);

 public:
  // CONSTRUCTORS
  Molecule(size_type d = 2) { init(d); }
  Molecule(size_type d, const std::string& i) : id(i) { init(d); }
  Molecule(const Molecule<D>& m) { operator=(m); }

  // ACCESSORS
  const particle_holder& atoms() const { return particles; }
  const_iterator atoms_begin() const { return particles.begin(); }
  const_iterator atoms_end() const { return particles.end(); }
  iterator atoms_begin() { return particles.begin(); }
  iterator atoms_end() { return particles.end(); }

  const std::string& ID() const { return id; }

  size_type size() const { return particles.size(); }
  size_type dimension() const { return mins.size(); }

  const vec_type& max() const { return maxs; }
  const vec_type& min() const { return mins; }

  bool inRange(const Molecule<D>& m) const;

  // MUTATORS

  void addMolecule(const Molecule& other);

  void translateMolecule(const vec_type& m);

  void clear();

  void ID(const std::string& i) { id = i; }

  void push_back(const particle_type& s);

  void resize(size_type ii) { particles.resize(ii); }
  void reserve(size_type ii) { particles.reserve(ii); }

  void print(std::ostream& o = std::cout) const;

  // OPERATORS
  Molecule<D>& operator=(const Molecule<D>& m);

  const particle_type& operator[](size_type ii) const { return particles[ii]; }
};

template <Dimension D>
bool molecular_size_based_test(const Molecule<D>& m1, const Molecule<D>& m2);

template <typename Molecule>
void moveMolecule(Molecule& m, const typename Molecule::vec_type& v) {
  typedef typename Molecule::vec_type vec_type;
  vec_type x = simpleAverageMoleculePosition(m);
  scale(x.begin(), x.end(), -1.0);
  vec_type vv(v);
  translate(vv.begin(), vv.end(), x.begin());
  m.translateMolecule(vv);
}

template <typename Molecule>
FloatType radiusOfMolecule(const Molecule& m) {
  typedef const typename Molecule::vec_type& vec_ref;
  typename Molecule::vec_type center = simpleAverageMoleculePosition(m);
  vec_ref min = m.min();
  vec_ref max = m.max();
  return .5 * euclideanDistance(min.begin(), min.end(), max.begin());
}

template <typename Molecule>
Molecule readMoleculeFromCoordFile(const char* file, FloatType radius);

template <typename Molecule>
typename Molecule::vec_type simpleAverageMoleculePosition(const Molecule& m) {
  typedef const typename Molecule::vec_type& vec_ref;
  vec_ref min = m.min();
  vec_ref max = m.max();
  typename Molecule::vec_type mean(min.size());
  for (typename Molecule::size_type ii = 0; ii < min.size(); ++ii) {
    mean[ii] = (min[ii] + max[ii]) * .5;
  }
  return mean;
}

template <typename Iterator>
void printMolecules(std::ostream& o, Iterator begin, Iterator end) {
  for (; begin != end; ++begin) {
    begin->print(o);
  }
}

template <typename Molecule, typename Sphere>
bool isMoleculeRoughlyInSphere(const Molecule& m, const Sphere& s) {
  typename Molecule::vec_type x1 = simpleAverageMoleculePosition(m);
  FloatType r1 = radiusOfMolecule(m);
  Sphere s1(x1, r1);
  return doIntersect(s, s1);
}

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_MOLECULE_H_
