#include "molecule.h"

#include "cube.h"

namespace lgl {
namespace lib {

template <Dimension D>
void Molecule<D>::resetMinsMaxs() {
  for (size_type ii = 0; ii < dimension(); ++ii) {
    maxs[ii] = -1e10;
    mins[ii] = 1e10;
  }
}

template <Dimension D>
void Molecule<D>::init(Molecule<D>::size_type d) {
  mins.clear();
  maxs.clear();
  mins.resize(d);
  maxs.resize(d);
  resetMinsMaxs();
}

template <Dimension D>
void Molecule<D>::min_max_test(const Molecule<D>::particle_type& s) {
  const vec_type& v = s.location();
  // s.print();
  for (size_type ii = 0; ii < dimension(); ++ii) {
    FloatType upper = v[ii] + s.radius();
    FloatType lower = v[ii] - s.radius();
    if (lower < mins[ii]) {
      mins[ii] = lower;
    }
    if (upper > maxs[ii]) {
      maxs[ii] = upper;
    }
  }
}

template <Dimension D>
void Molecule<D>::translateParticle(Molecule<D>::size_type ii,
                                    const Molecule<D>::vec_type& m) {
  vec_type& l = particles[ii].location();
  translate(l.begin(), l.end(), m.begin());
}

template <Dimension D>
bool Molecule<D>::inRange(const Molecule<D>& m) const {
  // Look for overlap in maxs and mins
  const vec_type& mins2 = m.min();
  const vec_type& maxs2 = m.max();
  for (size_type ii = 0; ii < mins2.size(); ++ii)
    if ((maxs2[ii] < mins[ii]) || (mins2[ii] > maxs[ii])) return false;
  return true;
}

template <Dimension D>
void Molecule<D>::addMolecule(const Molecule<D>& other) {
  if (other.dimension() != dimension()) {
  }
  // Combine the ids
  id += "|" + other.ID();
  // Add all the particles from the other molecule
  for (size_type ii = 0; ii < other.size(); ++ii) {
    push_back(other[ii]);
  }
}

template <Dimension D>
void Molecule<D>::translateMolecule(const Molecule<D>::vec_type& m) {
  for (size_type ii = 0; ii < particles.size(); ++ii) {
    translateParticle(ii, m);
  }
  resetMinsMaxs();
  for (size_type ii = 0; ii < particles.size(); ++ii) {
    min_max_test(particles[ii]);
  }
}

template <Dimension D>
void Molecule<D>::clear() {
  Molecule<D>::size_type d = dimension();
  particles.clear();
  id.clear();
  init(d);
}

template <Dimension D>
void Molecule<D>::push_back(const Molecule<D>::particle_type& s) {
  min_max_test(s);
  particles.push_back(s);
}

template <Dimension D>
void Molecule<D>::print(std::ostream& o) const {
  o << "Molecule " << id << '\n';
  o << "Mins: ";
  copy(mins.begin(), mins.end(), std::ostream_iterator<FloatType>(o, " "));
  o << "\nMaxs: ";
  copy(maxs.begin(), maxs.end(), std::ostream_iterator<FloatType>(o, " "));
  o << '\n';
  for (size_type ii = 0; ii < particles.size(); ++ii) {
    particles[ii].print(o);
  }
}

template <Dimension D>
Molecule<D>& Molecule<D>::operator=(const Molecule<D>& m) {
  particles = m.atoms();
  mins = m.min();
  maxs = m.max();
  id = m.ID();
  return *this;
}

template class Molecule<k2Dimensions>;
template class Molecule<k3Dimensions>;

template <Dimension D>
bool molecular_size_based_test(const Molecule<D>& m1, const Molecule<D>& m2) {
  return m1.size() < m2.size();
}
template bool molecular_size_based_test<k2Dimensions>(
    const Molecule<k2Dimensions>& m1, const Molecule<k2Dimensions>& m2);
template bool molecular_size_based_test<k3Dimensions>(
    const Molecule<k3Dimensions>& m1, const Molecule<k3Dimensions>& m2);

template <typename Molecule>
Molecule readMoleculeFromCoordFile(const char* file, FloatType radius) {
  typedef typename boost::tokenizer<boost::char_separator<char>> tokenizer;
  typename tokenizer::iterator tok_iter, tok_beg, tok_end;
  typename Molecule::particle_holder particle_holder;
  typedef typename Molecule::size_type size_type;
  typedef typename Molecule::particle_type particle_type;
  typedef typename Molecule::vec_type vec_type;

  std::ifstream in(file);
  if (!in) {
    std::cerr << "readMoleculeFromCoordFile: Open of " << file << " Failed\n";
    exit(EXIT_FAILURE);
  }

  boost::char_separator<char> sep(" ");
  size_type dimension = 0;
  Molecule m(3);

  while (!in.eof()) {
    char l[256];
    in.getline(l, 256);
    std::string line(l);
    if (line == "") {
      break;
    }
    tokenizer tokens(line, sep);
    tok_beg = tokens.begin();
    tok_end = tokens.end();
    if (dimension == 0) {
      dimension = std::distance(tok_beg, tok_end) - 1;
      Molecule mm(dimension, std::string(file));
      m = mm;
    }
    std::string id(*tok_beg);
    vec_type coords;
    while (++tok_beg != tok_end) {
      coords.push_back((FloatType)atof((*tok_beg).c_str()));
    }
    particle_type s(id, coords, radius);
    m.push_back(s);
  }

  return m;
}

template Molecule<k2Dimensions> readMoleculeFromCoordFile(const char* file,
                                                          FloatType radius);
template Molecule<k3Dimensions> readMoleculeFromCoordFile(const char* file,
                                                          FloatType radius);

}  // namespace lib
}  // namespace lgl
