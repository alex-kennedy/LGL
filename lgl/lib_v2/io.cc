#include "io.h"

#include <memory>

#include "absl/status/statusor.h"
#include "glog/logging.h"
#include "third_party/fast_cpp_csv_parser/csv.h"

using absl::StatusOr;
using absl::string_view;
using std::string;
using std::unique_ptr;

namespace lgl {
namespace lib_v2 {

StatusOr<unique_ptr<LargeGraph>> ReadGraph(string_view file_name) {
  if (file_name.length() > 3 &&
      file_name.substr(file_name.length() - 3).compare("lgl") == 0) {
    return ReadGraphLGL(file_name);
  } else if (file_name.length() > 4 &&
             file_name.substr(file_name.length() - 4).compare("ncol") == 0) {
    return ReadGraphNCOL(file_name);
  } else {
    return absl::NotFoundError(
        absl::StrCat("file extension not recognised for file: ", file_name));
  }
}

StatusOr<unique_ptr<LargeGraph>> ReadGraphLGL(string_view file_name) {
  try {
    io::LineReader reader(file_name.data());
    unique_ptr<LargeGraph> graph = absl::make_unique<LargeGraph>();

    string_view line;
    string_view source;
    while (char* line_ptr = reader.next_line()) {
      line = string_view(line_ptr);
      if (line.empty()) {
        return absl::InvalidArgumentError(
            "empty line found before end of file");
      }

      if (line.length() > 2 && line.substr(0, 2).compare("# ") == 0) {
        source = string(line.substr(2).data());
        if (source.empty()) {
          return absl::InvalidArgumentError(
              absl::StrCat("invalid source node: ", line));
        }
      } else {
        graph->AddEdge(source, line);
      }

      if (source.empty()) {
        return absl::InvalidArgumentError(
            "target node found before a source node");
      }
    }
    return graph;
  } catch (io::error::base& e) {
    return absl::UnknownError(e.what());
  }
}

StatusOr<unique_ptr<LargeGraph>> ReadGraphNCOL(string_view file_name) {
  try {
    io::CSVReader<2, io::trim_chars<' '>, io::no_quote_escape<' '>,
                  io::throw_on_overflow, io::no_comment>
        reader(file_name.data());
    unique_ptr<LargeGraph> graph = absl::make_unique<LargeGraph>();
    string source;
    string target;
    while (reader.read_row(source, target)) {
      graph->AddEdge(source, target);
    }
    return graph;
  } catch (io::error::base& e) {
    return absl::UnknownError(e.what());
  }
}

}  // namespace lib_v2
}  // namespace lgl
