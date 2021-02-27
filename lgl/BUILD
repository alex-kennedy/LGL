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

cc_binary(
    name = "layout",
    srcs = ["layout.cc"],
    deps = [
        "//lgl/lib_v2",
        "@com_github_google_glog//:glog",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings",
    ],
)
