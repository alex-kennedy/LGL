load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "lglbreakup",
    srcs = [
        "lglbreakup.cc",
    ],
    deps = [
        "//lgl/lib",
        "@boost//:graph",
    ],
)

cc_binary(
    name = "lglayout",
    srcs = ["lglayout.cc"],
    deps = [
        "//lgl/lib",
    ],
)

cc_binary(
    name = "lglrebuild",
    srcs = ["lglrebuild.cc"],
    deps = [
        "//lgl/lib",
    ],
)

cc_binary(
    name = "lglfileconvert",
    srcs = ["lglfileconvert.cc"],
    deps = [
        "//lgl/lib",
    ],
)
