#ifndef LGL_LIB_V2_CONFIG_H_
#define LGL_LIB_V2_CONFIG_H_

#include <string>

namespace lgl {
namespace lib_v2 {

struct LGLConfig {
  // Files paths for running the simulation.
  std::string graph_path;
  std::string positions_path;
  std::string mass_path;
  std::string anchors_path;

  int dimensions;
  float mass;
  int threads;
  int max_iter;
  float interaction_radius;
  float node_radius;
  float outer_radius;
  int write_interval;
  std::string root_node;
  bool write_levels;
  bool use_original_weights;
  bool layout_tree_only;
  float equilibrium_distance;
  std::string ellipse_factors;
  float placement_distance;
  float placement_radius;
  bool place_leaves_close;
};

}  // namespace lib_v2
}  // namespace lgl

#endif  // LGL_LIB_V2_CONFIG_H_
