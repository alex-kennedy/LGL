load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Boost support
git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "1e3a69bf2d5cd10c34b74f066054cd335d033d71",
    remote = "https://github.com/nelhage/rules_boost",
    shallow_since = "1591047380 -0700",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()

# googletest support
git_repository(
    name = "gtest",
    commit = "703bd9caab50b139428cea1aaff9974ebee5742e",  # v1.10.x
    remote = "https://github.com/google/googletest",
    shallow_since = "1570114335 -0400",
)

# Abseil support
http_archive(
    name = "com_google_absl",
    sha256 = "38f5fc5399a8b88538b55b81c6d22b4622a41b7c1f568d0d883aaa63fb5d49c5",
    strip_prefix = "abseil-cpp-b343ac3a649f5713380cdbeee79d2ee6f94ddd93",
    urls = ["https://github.com/abseil/abseil-cpp/archive/b343ac3a649f5713380cdbeee79d2ee6f94ddd93.zip"],
)

# Fast C++ CSV Parser
new_git_repository(
    name = "fast_cpp_csv_parser",
    build_file = "@//third_party/fast_cpp_csv_parser:BUILD.bazel",
    commit = "75600d0b77448e6c410893830df0aec1dbacf8e3",
    remote = "https://github.com/ben-strasser/fast-cpp-csv-parser.git",
    shallow_since = "1609681465 +0100",
)

# Google-style C++ logging
http_archive(
    name = "com_github_google_glog",
    # Manually disable gflags so we can use Abseil's flags library
    build_file_content = """
licenses(['notice'])
load(':bazel/glog.bzl', 'glog_library')
glog_library(with_gflags = 0)
        """,
    sha256 = "62efeb57ff70db9ea2129a16d0f908941e355d09d6d83c9f7b18557c0a7ab59e",
    strip_prefix = "glog-d516278b1cd33cd148e8989aec488b6049a4ca0b",
    urls = ["https://github.com/google/glog/archive/d516278b1cd33cd148e8989aec488b6049a4ca0b.zip"],
)
