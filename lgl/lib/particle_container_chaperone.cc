#include "particle_container_chaperone.h"

#include "particle.h"
#include "types.h"

namespace lgl {
namespace lib {

template <Dimension D>
void ParticleContainerChaperone<D>::openInFiles() {
  for (int i = 0; i < 3; ++i) {
    if (file_in[i] != 0) {
      streams_in[i].open(file_in[i]);
      if (!streams_in[i]) {
        cerr << " Could not open Input File: " << file_in[i] << endl;
        std::exit(EXIT_FAILURE);
      }
    }
  }
}

template <Dimension D>
void ParticleContainerChaperone<D>::openOutFiles() {
  for (int i = 0; i < 3; ++i) {
    if (file_out[i] != 0) {
      streams_out[i].open(file_out[i]);
      if (!streams_out[i]) {
        cerr << " Could not open Out File: " << file_out[i] << endl;
        std::exit(EXIT_FAILURE);
      }
    }
  }
}

template <Dimension D>
void ParticleContainerChaperone<D>::closeInFiles() {
  for (int i = 0; i < 3; ++i) {
    if (file_in[i] != 0) {
      streams_in[i].close();
    }
  }
}

template <Dimension D>
void ParticleContainerChaperone<D>::closeOutFiles() {
  for (int i = 0; i < 3; ++i) {
    if (file_out[i] != 0) {
      streams_out[i].close();
    }
  }
}

template <Dimension D>
void ParticleContainerChaperone<D>::orderingError(const char* file) {
  cerr << "The ordering in the initialization file " << file << " is off.\n";
  std::exit(EXIT_FAILURE);
}

template <Dimension D>
void ParticleContainerChaperone<D>::writeXout(const Particle<D>& p,
                                              const string& id) {
  streams_out[_X_FILE__] << id << " ";
  p.printXCoords(streams_out[_X_FILE__]);
  streams_out[_X_FILE__] << '\n';
}

template <Dimension D>
bool ParticleContainerChaperone<D>::setXFromFile(Particle<D>& p,
                                                 const string& id2check) {
  // assuming id2check isn't empty for simplicity of this optimized
  // implementation
  assert(!id2check.empty());
  const auto it = positions_from_file_.find(id2check);
  if (it != positions_from_file_.end()) {
    p.X(it->second);
    return true;
  }
  return false;
}

template <Dimension D>
bool ParticleContainerChaperone<D>::setXFromAnchors(
    Particle<D>& p, const std::string& id2check) const {
  typename AnchorPositions_t::const_iterator const it =
      anchorPositions_.find(id2check);
  if (it == anchorPositions_.end()) return false;
  p.X(it->second);
  p.markAnchor();
  return true;
}

template <Dimension D>
bool ParticleContainerChaperone<D>::setMFromFile(Particle<D>& p,
                                                 string id2check) {
  string id = "";
  if (!streams_in[_M_FILE__].eof() && streams_in[_M_FILE__] >> id) {
    if (id2check != "" && id2check != id) {
      chaperone_type::orderingError(file_in[_M_FILE__]);
    }
    streams_in[_M_FILE__] >> initMass_;
    p.mass(initMass_);
    return 1;
  } else {
    return 0;
  }
}

template <Dimension D>
typename ParticleContainerChaperone<D>::vec_type&
ParticleContainerChaperone<D>::randomVec(vec_type& v, FloatType range) {
  for (unsigned int jj = 0; jj < D; ++jj) {
    v[jj] = static_cast<FloatType>(std::rand() / (RAND_MAX + 1.0)) * range;
  }
  return v;
}

template <Dimension D>
void ParticleContainerChaperone<D>::initVals() {
  for (unsigned int jj = 0; jj < 3; ++jj) {
    file_in[jj] = 0;
    file_out[jj] = 0;
    file_out_flag[jj] = 0;
  }
  vel_ = 0;
  initVel_ = 0;
  initPos_ = 0;
  radius_ = 0;
  initMass_ = 0;
  posRange_ = 0;
  velRange_ = 0;
  randomPos_ = 0;
  randomVel_ = 0;
}

template <Dimension D>
ParticleContainerChaperone<D>::ParticleContainerChaperone(
    ParticleContainer<D>& pc__)
    : pc_(pc__) {
  chaperone_type::initVals();
}

template <Dimension D>
void ParticleContainerChaperone<D>::initAllParticles() {
  chaperone_type::openInFiles();
  readXin();
  size_type nodeCount = pc_.size();
  for (size_type ii = 0; ii < nodeCount; ++ii) {
    const string& id = pc_.ids[ii];
    if (file_in[_X_FILE__]) {
      chaperone_type::setXFromFile(pc_[ii], id);
    } else if (!chaperone_type::setXFromAnchors(pc_[ii], id)) {
      if (randomPos_) {
        pc_[ii].X(chaperone_type::randomVec(initPos_, posRange_));
      } else {
        pc_[ii].X(initPos_);
      }
    }
    if (file_in[_M_FILE__]) {
      chaperone_type::setMFromFile(pc_[ii], id);
    } else {
      pc_[ii].mass(initMass_);
    }
    pc_[ii].radius(radius_);
  }
  chaperone_type::closeInFiles();
}

template <Dimension D>
void ParticleContainerChaperone<D>::writeOutFiles() {
  chaperone_type::openOutFiles();
  size_type nodeCount = pc_.size();
  for (size_type ii = 0; ii < nodeCount; ++ii) {
    string id = pc_.ids[ii];
    if (file_out_flag[_X_FILE__] != 0) {
      chaperone_type::writeXout(pc_[ii], id);
    }
  }
  chaperone_type::closeOutFiles();
}

template <Dimension D>
void ParticleContainerChaperone<D>::initAnchors(const std::string& filepath) {
  std::ifstream f(filepath.c_str());
  if (!f)
    throw std::runtime_error("Failed to open " + filepath + " for reading");
  readAnchors(f);
}

template <Dimension D>
void ParticleContainerChaperone<D>::initVel(const vec_type& v) {
  initVel_ = v;
  randomVel_ = 0;
}

template <Dimension D>
void ParticleContainerChaperone<D>::initPos(const vec_type& x) {
  initPos_ = x;
  randomPos_ = 0;
}

template <Dimension D>
void ParticleContainerChaperone<D>::initMass(FloatType m) {
  initMass_ = m;
}

template <Dimension D>
void ParticleContainerChaperone<D>::initRadius(FloatType r) {
  radius_ = r;
}

template <Dimension D>
void ParticleContainerChaperone<D>::randomizePosRange(FloatType p) {
  posRange_ = p;
  randomPos_ = 1;
}

template <Dimension D>
void ParticleContainerChaperone<D>::randomizeVelRange(FloatType v) {
  velRange_ = v;
  randomVel_ = 1;
}

template <Dimension D>
void ParticleContainerChaperone<D>::velOutFile(const char* v) {
  file_out[_V_FILE__] = strdup(v);
  file_out_flag[_V_FILE__] = 1;
}

template <Dimension D>
void ParticleContainerChaperone<D>::posOutFile(const char* x) {
  file_out[_X_FILE__] = strdup(x);
  file_out_flag[_X_FILE__] = 1;
}

template <Dimension D>
void ParticleContainerChaperone<D>::tempOutFile(const char* t) {
  file_out[_T_FILE__] = strdup(t);
  file_out_flag[_T_FILE__] = 1;
}

template <Dimension D>
ParticleContainerChaperone<D>::~ParticleContainerChaperone() {
  for (int i = 0; i < 3; ++i) {
    delete file_in[i];
    delete file_out[i];
  }
}

template <Dimension D>
void ParticleContainerChaperone<D>::readAnchors(std::istream& is) {
  std::string id;
  vec_type pos;
  while (is >> id) {
    if (!readPos(is, pos))
      throw std::domain_error("Anchor position input failed for node '" + id +
                              '\'');
    if (!anchorPositions_.insert(std::make_pair(id, pos)).second)
      throw std::domain_error("Anchor '" + id +
                              "' has already been specified!");
  }
  if (!is.eof())
    throw std::domain_error("Anchor file input failed around node '" + id +
                            '\'');
}

template <Dimension D>
void ParticleContainerChaperone<D>::readXin() {
  auto& is = streams_in[_X_FILE__];
  std::string id;
  vec_type pos;
  while (is >> id) {
    if (!readPos(is, pos))
      throw std::domain_error("Initial position input failed for node '" + id +
                              '\'');
    if (!positions_from_file_.insert({std::move(id), pos}).second)
      throw std::domain_error("Node '" + id +
                              "' has already been specified earlier in the "
                              "positions input file!");
  }
  if (!is.eof())
    throw std::domain_error(
        "Initial positions file input failed around node '" + id + '\'');
}

template class ParticleContainerChaperone<k2Dimensions>;
template class ParticleContainerChaperone<k3Dimensions>;

}  // namespace lib
}  // namespace lgl
