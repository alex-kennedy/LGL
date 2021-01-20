cc_library(
  name = "include",
  srcs = [
    "configs.h",
    "aPthread.hpp",
    "graph.hpp",
    "fixedVecOperations.hpp",
    "mutualMap.hpp",
    "grid.hpp",
    "fixedVec.hpp",
    "voxel.hpp",
    "cube.hpp",
    "gridSchedual.hpp",
    "particle.hpp",
    "particleContainer.hpp",
    "particleContainerChaperone.hpp",
    "particleStats.hpp",
    "timeKeeper.hpp",
    "voxelInteractionHandler.hpp",
    "calcFuncs.h",
    "sphere.hpp",
    "particleInteractionHandler.hpp",
    "thread_pool.hpp",
    "molecule.hpp",
    "edLookupTable.hpp",
  ],
  visibility = ["//visibility:public"]
)

cc_library(
  name = "calcFuncs",
  srcs = [
    "calcFuncs.C",
  ],
  deps = [":include"],
  hdrs = ["calcFuncs.h"],
  linkopts = ["-pthread"],
  visibility = ["//visibility:public"]
)