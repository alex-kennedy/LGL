load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "lglbreakup",
    srcs = [
        "lglbreakup.cc",
    ],
    deps = [
        "//src:configs",
        "//src/utils",
    ],
)

cc_binary(
    name = "lglayout",
    srcs = ["lglayout.cc"],
    deps = [
        "//src:configs",
        "//src/geometry",
        "//src/utils",
    ],
)

cc_binary(
    name = "lglrebuild",
    srcs = ["lglrebuild.cc"],
    deps = [
        "//src/geometry",
        "//src/utils",
    ],
)

cc_binary(
    name = "lglfileconvert",
    srcs = ["lglfileconvert.cc"],
    deps = [
        "//src:configs",
        "//src/geometry",
        "//src/utils",
    ],
)
