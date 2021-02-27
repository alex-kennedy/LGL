#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/string_view.h"
#include "glog/logging.h"

ABSL_FLAG(
    std::string, positions, "",
    "Path to a CSV file specifying initial positions of each node. Each line "
    "should be other form 'node,x,y[,z], depending on the dimensionality.");

ABSL_FLAG(std::string, mass_file, "",
          "Path to a CSV file specifying the mass of each node. If not "
          "specified, equal masses are used for all particles.");
ABSL_FLAG(float, mass, 1.0,
          "Mass of each node. If mass_file is given, this value is "
          "overridden for any nodes in the file.");

ABSL_FLAG(std::string, anchors, "", "Path to anchors file.");

ABSL_FLAG(int, threads, 1, "Number of threads to spawn. For maximum, use -1.");

ABSL_FLAG(int, max_iter, 250000, "Maximum number of iterations.");

ABSL_FLAG(
    float, interaction_radius, 1.0,
    "The neighborhood radius for each particle. It defines the interaction "
    "range for casual (generally repulsive) interactions.");

ABSL_FLAG(float, time_step, 0.001, "Time step for each iteration.");

ABSL_FLAG(float, node_radius, 0.01,
          "The size of each node. Prevents force from going to infinity if "
          "nodes get too close to each other.");

ABSL_FLAG(float, outer_radius, -1.0,
          "The radius of the outer boundary. Determined based on the "
          "number of nodes and dimensionality unless it is set here.");

ABSL_FLAG(int, write_interval, 0,
          "Dumps intermediate coordinates at this interval.");

ABSL_FLAG(std::string, root_node, "", "Manually specify the root node.");

ABSL_FLAG(bool, write_levels, false, "Write the edge level map.");

ABSL_FLAG(bool, write_mst, false, "Write the minimum spanning tree.");

ABSL_FLAG(bool, use_original_weights, false, "Use original weights.");

ABSL_FLAG(bool, layout_tree_only, false,
          "Run the layout algorithm for the minimum spanning tree only. Can "
          "be helpful for large graphs.");

ABSL_FLAG(float, equilibrium_distance, 0.5, "Equilibrium distance.");

// TODO(alex-kennedy): Establish a format for this and parse it to a vector
// directly.
ABSL_FLAG(std::string, ellipse_factors, "", "Ellipse factors.");

ABSL_FLAG(float, placement_distance, -1.0,
          "Placement distance is the distance you want the next level to be "
          "placed with respect to the previous level. If this float value "
          "is not given a formula calculates the placement distance.");

ABSL_FLAG(float, placement_radius, 0.1, "A measure of placement density.");

ABSL_FLAG(bool, place_leaves_close, false,
          "Place the leafs close by. This applies to trees more than graphs. "
          "Setting this option will place the child vertices very near the "
          "parent vertex if all of its children have none themselves.");

ABSL_FLAG(int, log_threshold, 1,
          "Minimum threshold for logging. The numbers of severity levels INFO, "
          "WARNING, ERROR, and FATAL are 0, 1, 2, and 3, respectively. "
          "Defaults to 1, which is WARNING");

int main(int argc, char* argv[]) {
  std::vector<char*> positional_args = absl::ParseCommandLine(argc, argv);

  google::InitGoogleLogging(argv[0]);
  google::SetStderrLogging(absl::GetFlag(FLAGS_log_threshold));

  // There should be one positional argument, the graph file to load
  absl::string_view graph_file;
  if (positional_args.size() == 1) {
    LOG(ERROR) << "No graph file given. Exiting.";
    return 1;
  } else if (positional_args.size() > 2) {
    LOG(ERROR) << "Too many positional arguments. Exiting.";
    return 1;
  } else {
    graph_file = positional_args[1];
  }
  LOG(INFO) << "Laying out graph: " << graph_file;

  google::ShutdownGoogleLogging();
}
