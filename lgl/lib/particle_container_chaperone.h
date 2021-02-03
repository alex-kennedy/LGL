#ifndef LGL_LIB_PARTICLE_CONTAINER_CHAPERONE_H_
#define LGL_LIB_PARTICLE_CONTAINER_CHAPERONE_H_

//----------------------------------------------------
// This class is a helper for particleContainer. It takes
// care of initializations and such of X, V, etc
//----------------------------------------------------

#include <string.h>  // for strdup

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "particle.h"
#include "particle_container.h"
#include "types.h"

namespace lgl {
namespace lib {

const unsigned int _X_FILE__ = 0;  // Position file
const unsigned int _V_FILE__ = 1;  // Velocity file
const unsigned int _M_FILE__ = 2;  // Mass file
const unsigned int _T_FILE__ = 2;  // Temprature file

template <Dimension D>
class ParticleContainerChaperone {
 private:
  using chaperone_type = ParticleContainerChaperone<D>;

 public:
  static const Dimension n_dimensions_ = D;
  typedef typename Particle<D>::vec_type vec_type;
  typedef typename ParticleContainer<D>::size_type size_type;

  ParticleContainer<D>& pc_;
  vec_type initVel_;
  vec_type vel_;
  vec_type initPos_;
  std::unordered_map<std::string, vec_type> positions_from_file_;
  FloatType radius_;
  FloatType initMass_;
  char* file_in[3];     //  X V M
  FloatType posRange_;  // Range for randomizing positions
  FloatType velRange_;  //   "    "      "       velocities
  bool randomPos_;
  bool randomVel_;
  ifstream streams_in[3];   // X V M
  ofstream streams_out[3];  // X V Temprature
  char* file_out[3];        // X V Temprature ( These are output files )
  bool file_out_flag[3];
  int level_;

  void openInFiles();

  void openOutFiles();

  void closeInFiles();

  void closeOutFiles();

  void orderingError(const char* file);

  void writeXout(const Particle<D>& p, const string& id);

  bool setXFromFile(Particle<D>& p, const string& id2check);

  bool setXFromAnchors(Particle<D>& p, const std::string& id2check) const;

  // bool setVFromFile(Particle<D>& p, string id2check = "");

  bool setMFromFile(Particle<D>& p, string id2check = "");

  vec_type& randomVec(vec_type& v, FloatType range);

  void initVals();

  // ParticleContainerChaperone() {}

  ParticleContainerChaperone(ParticleContainer<D>& pc__);

  void initAllParticles();

  void writeOutFiles();

  void initPos(const char* x) { file_in[_X_FILE__] = strdup(x); }

  void initMass(const char* m) { file_in[_M_FILE__] = strdup(m); }

  void initAnchors(const std::string& filepath);

  void initVel(const vec_type& v);

  void initPos(const vec_type& x);

  void initMass(FloatType m);

  void initRadius(FloatType r);

  void randomizePosRange(FloatType p);

  void randomizeVelRange(FloatType v);

  int level() const { return level_; }

  void level(int l) { level_ = l; }

  void velOutFile(const char* v);

  void posOutFile(const char* x);

  void tempOutFile(const char* t);

  const vec_type& initVel() { return initVel_; }
  const vec_type& initPos() { return initPos_; }
  FloatType initMass() { return initMass_; }
  FloatType initRadius() { return radius_; }

  ~ParticleContainerChaperone();

 private:
  typedef std::map<std::string, vec_type> AnchorPositions_t;
  AnchorPositions_t anchorPositions_;

  void readAnchors(std::istream& is);

  void readXin();

  static std::istream& readPos(std::istream& is, vec_type& pos) {
    for (size_type ii = 0; ii < D; ++ii) is >> pos[ii];
    return is;
  }
};

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_PARTICLE_CONTAINER_CHAPERONE_H_
