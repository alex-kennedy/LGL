load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "lglbreakup",
    srcs = ["lglbreakup.cc"],
    deps = [
        "//include",
    ],
)

cc_binary(
    name = "lglayout",
    srcs = ["lglayout.cc"],
    deps = [
        "//include",
        "//include:calcFuncs",
    ],
)

cc_binary(
    name = "lglrebuild",
    srcs = ["lglrebuild.cc"],
    deps = [
        "//include",
        "//include:calcFuncs",
    ],
)

cc_binary(
    name = "lglfileconvert",
    srcs = ["lglfileconvert.cc"],
    deps = [
        "//include",
    ],
)
