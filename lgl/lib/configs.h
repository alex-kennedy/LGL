#ifndef _CONFIGS_H_
#define _CONFIGS_H_

//------------------------------------------------------

#include <algorithm>
#include <iterator>
#include <set>
#include <vector>

#include "aPthread.hpp"
#include "graph.hpp"
#include "cube.h"
#include "grid.hpp"
#include "gridSchedual.hpp"
#include "particle.h"
#include "particleContainer.hpp"
#include "particleContainerChaperone.hpp"
#include "particleStats.hpp"
#include "timeKeeper.hpp"
#include "voxel.hpp"
#include "voxelInteractionHandler.hpp"

//------------------------------------------------------

namespace lgl {
namespace lib {

typedef float prec_t;  // TODO(alexkennedy): Remove when replaced
const unsigned int DIMENSION = 2;

//------------------------------------------------------

const Dimension n_dimension_config = Dimension::k2Dimensions;

typedef Particle<k2Dimensions> Node;
typedef ParticleContainer<Node> NodeContainer;
typedef Grid<Node> Grid_t;
typedef FixedVec<FloatType, Dimension::k2Dimensions> FixedVec_p;
typedef FixedVec<long, Dimension::k2Dimensions> FixedVec_l;
typedef GridIter<Grid_t> GridIterator;
typedef ParticleContainerChaperone<Node> PCChaperone;
typedef ApthreadContainer ThreadContainer;
typedef Voxel<n_dimension_config> Voxel_t;
typedef TimeKeeper<FloatType> TimeKeeper_t;
template <typename Particle>
class ParticleInteractionHandler;
typedef ParticleInteractionHandler<Node> NodeInteractionHandler;
typedef VoxelInteractionHandler<Voxel_t, NodeInteractionHandler> VoxelHandler;
typedef GridSchedual_MTS<Grid_t> GridSchedual_t;
typedef ParticleStats<Node> ParticleStats_t;
typedef Graph<FloatType> Graph_t;
typedef vector<unsigned> LevelMap;
typedef vector<unsigned> ParentMap;
typedef vector<bool> PlacementStatus;
typedef std::vector<FloatType> EllipseFactors;

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

#endif
