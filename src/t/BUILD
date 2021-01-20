load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "genTestGraphAndCoords",
    srcs = ["genTestGraphAndCoords.cpp"],
)

cc_binary(
    name = "voxelInteractionTest",
    srcs = ["voxelInteractionTest.cpp"],
    deps = [
        "//include",
    ],
)
