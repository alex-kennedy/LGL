load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "threading",
    srcs = [
        "aPthread.hpp",
        "thread_pool.hpp",
    ],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)
