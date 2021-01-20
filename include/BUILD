load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "include",
    srcs = [
        "aPthread.hpp",
        "calcFuncs.h",
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
    visibility = ["//visibility:public"],
)

cc_library(
    name = "calcFuncs",
    srcs = [
        "calcFuncs.C",
    ],
    hdrs = ["calcFuncs.h"],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
    deps = [":include"],
)
