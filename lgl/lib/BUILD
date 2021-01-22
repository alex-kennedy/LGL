load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "lib",
    srcs = [
        "aPthread.hpp",
        "calcFuncs.cc",
        "configs.h",
        "cube.hpp",
        "edLookupTable.hpp",
        "fixedVec.hpp",
        "fixedVecOperations.hpp",
        "graph.hpp",
        "grid.hpp",
        "gridSchedual.hpp",
        "molecule.hpp",
        "mutualMap.hpp",
        "particle.hpp",
        "particleContainer.hpp",
        "particleContainerChaperone.hpp",
        "particleInteractionHandler.hpp",
        "particleStats.hpp",
        "sphere.hpp",
        "thread_pool.hpp",
        "timeKeeper.hpp",
        "voxel.hpp",
        "voxelInteractionHandler.hpp",
    ],
    hdrs = [
        "calcFuncs.h",
    ],
    linkopts = ["-pthread"],
    visibility = ["//lgl:__subpackages__"],
)
