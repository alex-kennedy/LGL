load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "lib_v2",
    visibility = ["//lgl:__subpackages__"],
    deps = [
        "config",
        ":grid",
        ":io",
        ":large_graph",
        ":layout_runner",
        ":particle",
    ],
)

cc_library(
    name = "config",
    hdrs = ["config.h"],
)

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
    name = "layout_runner",
    srcs = ["layout_runner.cc"],
    hdrs = ["layout_runner.h"],
    deps = [
        ":large_graph",
        ":config",
        ":grid",
    ],
)

cc_library(
    name = "grid",
    srcs = ["grid.cc"],
    hdrs = ["grid.h"],
    deps = [
        ":large_graph",
        ":particle",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/container:flat_hash_set",
    ],
)

cc_test(
    name = "grid_test",
    srcs = ["grid_test.cc"],
    deps = [
        ":grid",
        "@com_google_absl//absl/memory",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "particle",
    srcs = ["particle.cc"],
    hdrs = ["particle.h"],
    deps = [
        "@com_google_absl//absl/container:fixed_array",
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

cc_library(
    name = "io",
    srcs = ["io.cc"],
    hdrs = ["io.h"],
    deps = [
        ":large_graph",
        "@com_github_google_glog//:glog",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@fast_cpp_csv_parser",
    ],
)

cc_test(
    name = "io_test",
    srcs = ["io_test.cc"],
    deps = [
        ":io",
        "@gtest//:gtest_main",
    ],
)
