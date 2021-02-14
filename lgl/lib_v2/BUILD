load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "large_graph",
    srcs = ["large_graph.cc"],
    hdrs = ["large_graph.h"],
    deps = [
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "large_graph_test",
    srcs = ["large_graph_test.cc"],
    deps = [
        ":large_graph",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "grid",
    srcs = ["grid.cc"],
    hdrs = ["grid.h"],
    deps = [
        ":large_graph",
        ":particle",
        "//lgl/lib",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/container:flat_hash_set",
    ],
)

cc_test(
    name = "grid_test",
    srcs = ["grid_test.cc"],
    deps = [
        ":grid",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "particle",
    srcs = ["particle.cc"],
    hdrs = ["particle.h"],
    deps = [
        "//lgl/lib",
        "@com_google_absl//absl/synchronization",
    ],
)

cc_test(
    name = "particle_test",
    srcs = ["particle_test.cc"],
    deps = [
        ":particle",
        "@gtest//:gtest_main",
    ],
)
