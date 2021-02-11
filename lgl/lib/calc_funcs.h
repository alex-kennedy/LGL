// This file has functions which the threads
// could call.

#ifndef LGL_LIB_CALC_FUNCS_H_
#define LGL_LIB_CALC_FUNCS_H_

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/breadth_first_search.hpp"
#include "boost/graph/kruskal_min_spanning_tree.hpp"
#include "boost/graph/visitors.hpp"
#include "boost/property_map/property_map.hpp"
#include "configs.h"
#include "fixed_vec.h"
#include "grid.h"
#include "lgl/lib/particle_interaction_handler.h"
#include "particle.h"
#include "particle_container.h"
#include "particle_container_chaperone.h"
#include "sphere.h"
#include "types.h"
#include "voxel.h"
#include "voxel_interaction_handler.h"

namespace lgl {
namespace lib {

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>
    out_graph;
typedef std::vector<FloatType> EllipseFactors;

struct ThreadArgs {
  NodeContainer* nodes;
  VoxelHandler* voxelHandler;
  NodeInteractionHandler* nodeHandler;
  FixedVec_l* voxelList;
  ParticleStats_t* stats;
  long voxelListSize;
  Grid_t* grid;
  FloatType eqDistance;
  EllipseFactors ellipseFactors;
  GridIterator* gridIterator;
  long threadCount;
  long whichThread;
  FloatType nbhdRadius;
  FloatType casualSpringConstant;
  FloatType specialSpringConstant;
  Graph<FloatType>* full_graph;
  Graph<FloatType>* layout_graph;
  LevelMap* levels;
  ParentMap* parents;
  unsigned int currentLevel;
};

void* calcInteractions(void* arg);
void* integrateParticles(void* arg);
void* onlyEdgeInteractions(void* arg);
void* collectEdgeStats(void* arg);

int generateMSTFromNodes(NodeContainer& nodes, ThreadArgs* args, long p);

FloatType collectOutput(ThreadArgs* args, PCChaperone& chaperone);
void beginSimulation(ThreadContainer& threads, FloatType cutOffPrecision,
                     TimeKeeper& timer, ThreadArgs* threadArgs,
                     PCChaperone& chaperone, unsigned int totalLevels, bool b,
                     FloatType placementDistance, FloatType placementRadius,
                     bool placeLeafsClose, bool silentOutput);

void adjustWeightsBasedOnChildrenCount(NodeContainer& nodes);
void solidifyLargeEdges(NodeContainer& nodes);

FixedVec_p calcCenterOfMass(Graph<FloatType>& g, NodeContainer& nodes,
                            LevelMap& levels, unsigned int currentLevel);

void initializeCurrentLayer(Graph<FloatType>& g, NodeContainer& nodes,
                            LevelMap& levels, ParentMap& p, Grid_t& grid,
                            unsigned int currentLevel, Graph<FloatType>& full,
                            FloatType placementDistance,
                            FloatType placementRadius, bool placeLeafsClose);

long activeEdgeCount(Node& n);

FloatType activateNextLayerOfEdges(NodeContainer& n, unsigned int currentLevel,
                                   ThreadArgs* threadArgs);
FloatType getBufferDistance(Node& n);
void printOutput(long i, FloatType d, long ll, std::ostream& o);

void layerNPlacement(NodeContainer& nodes, Grid_t& grid, out_graph& g,
                     FixedVec_p& cm, unsigned int currentLevel,
                     ParentMap& parents, Graph<FloatType>& full, LevelMap& lm,
                     FloatType placementDistance, FloatType placementRadius,
                     bool placeLeafsClose);
void generatePlacementVector(FixedVec_p& d, const FixedVec_p& parentNode,
                             const FixedVec_p& parentParentNode,
                             const FixedVec_p& cm);
FloatType placementFormula(FloatType placementDistance, int vertices2place,
                           int dimension);

void gridPrepAndInit(NodeContainer& nc, Grid_t& g, FloatType voxelLength);
bool doesVertexHaveAnyChildren(Graph<FloatType>& G,
                               Graph<FloatType>::vertex_descriptor v,
                               out_graph& g, ParentMap& parents);

EllipseFactors parseEllipseFactors(const std::string& optionStr);

// Attempts to initialize any particle positions that are not initialized yet,
// by way of interpolation from its neighbors which have initialized positions
// already, if any. This is done by picking the "center point" of those
// neighbors' positions to be used as the initial position for a particle that
// wasn't initialized before. The process continues until either all particles
// have initial positions properly set, or no further progress can be made (due
// to isolated and totally uninitialized islands). Also, the progress of the
// stages and the final accomplishment is printed to stdout.
void interpolateUninitializedPositions(PCChaperone& chaperone,
                                       const Graph<FloatType>::boost_graph& g,
                                       bool remove_disconnected_nodes);

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_CALC_FUNCS_H_
