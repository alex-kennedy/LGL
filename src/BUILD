load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "lglbreakup",
    srcs = ["lglbreakup.C"],
    deps = [
        "//include",
    ],
)

cc_binary(
    name = "lglayout",
    srcs = ["lglayout.C"],
    deps = [
        "//include",
        "//include:calcFuncs",
    ],
)

cc_binary(
    name = "lglrebuild",
    srcs = ["lglrebuild.C"],
    deps = [
        "//include",
        "//include:calcFuncs",
    ],
)

cc_binary(
    name = "lglfileconvert",
    srcs = ["lglfileconvert.C"],
    deps = [
        "//include",
    ],
)
