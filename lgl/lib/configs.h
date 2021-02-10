#ifndef LGL_LIB_CONFIGS_H_
#define LGL_LIB_CONFIGS_H_

//------------------------------------------------------

#include <algorithm>
#include <iterator>
#include <set>
#include <vector>

#include "graph.h"
#include "grid.h"
#include "grid_schedule.h"
#include "particle.h"
#include "particle_container.h"
#include "particle_container_chaperone.h"
#include "particle_interaction_handler.h"
#include "particle_stats.h"
#include "pthread_wrapper.h"
#include "thread_pool.h"
#include "time_keeper.h"
#include "types.h"
#include "voxel.h"
#include "voxel_interaction_handler.h"

//------------------------------------------------------

namespace lgl {
namespace lib {

typedef float prec_t;  // TODO(alexkennedy): Remove when replaced
const unsigned int DIMENSION = 2;

//------------------------------------------------------

const Dimension n_dimensions = k2Dimensions;

typedef Particle<n_dimensions> Node;
typedef ParticleContainer<n_dimensions> NodeContainer;
typedef Grid<Node> Grid_t;
typedef FixedVec<FloatType, n_dimensions> FixedVec_p;
typedef FixedVec<long, n_dimensions> FixedVec_l;
typedef GridIter<Grid_t> GridIterator;
typedef ParticleContainerChaperone<n_dimensions> PCChaperone;
typedef ApthreadContainer ThreadContainer;
typedef Voxel<n_dimensions> Voxel_t;
typedef ParticleInteractionHandler<n_dimensions> NodeInteractionHandler;
typedef VoxelInteractionHandler<n_dimensions> VoxelHandler;
typedef GridSchedule_MTS<Grid_t> GridSchedule_t;
typedef ParticleStats<Node> ParticleStats_t;
typedef Graph<FloatType> Graph_t;
typedef std::vector<unsigned> LevelMap;
typedef std::vector<unsigned> ParentMap;
typedef std::vector<bool> PlacementStatus;

//------------------------------------------------------

// Time step for the force calcs
const FloatType PART_TIME_STEP = .001;

// The program stops and writes to file
// at this count. (Default value).
const int MAXITER = 250000;

// This automatically dums xcoords to file
// at these multiples.
const int WRITE_INTERVAL = 0;  // Off by default

// The range that make casual interactions
// applicable
const FloatType INTERACTION_RADIUS = 1.0;

// The default value of k in F=-kx for casual interactions
const FloatType DEFAULT_SPRING_CONSTANT = 10.0;

// This is an initial force term for the
// drag. The drag probably increases with
// each time step though.
const FloatType INIT_RESISTANCE = 1.0;

// This is the scale factor for all the spring constants
// (Scales col1 of the relational data)
const FloatType SPRING_CONSTANT_FACTOR = 1.0;

// This is the size of each node. It is important to
// have a size since if a particle gets really really
// really close the unit vector -> infinite
// const FloatType NODE_SIZE = BEST_EQ_DISTANCE*.25;
const FloatType NODE_SIZE = .01;

// If this = 0 the input data has no connection strengths.
const int DEFAULT_WEIGHT_FORMAT = 1;

// This is the default mass size of each node.
const FloatType DEFAULT_NODE_MASS = 1.0;

//------------------------------------------------

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_CONFIGS_H_
