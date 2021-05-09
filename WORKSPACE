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
    name = "com_google_googletest",
    commit = "703bd9caab50b139428cea1aaff9974ebee5742e",  # v1.10.x
    remote = "https://github.com/google/googletest",
    shallow_since = "1570114335 -0400",
)

# Abseil support
http_archive(
    name = "com_google_absl",
    sha256 = "441db7c09a0565376ecacf0085b2d4c2bbedde6115d7773551bc116212c2a8d6",
    strip_prefix = "abseil-cpp-20210324.1",
    urls = ["https://github.com/abseil/abseil-cpp/archive/refs/tags/20210324.1.tar.gz"],
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
    sha256 = "f28359aeba12f30d73d9e4711ef356dc842886968112162bc73002645139c39c",
    strip_prefix = "glog-0.4.0",
    urls = ["https://github.com/google/glog/archive/refs/tags/v0.4.0.tar.gz"],
)
