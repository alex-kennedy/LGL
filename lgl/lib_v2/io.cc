#include "io.h"

#include <memory>

#include "absl/status/statusor.h"
#include "glog/logging.h"
#include "third_party/fast_cpp_csv_parser/csv.h"

namespace lgl {
namespace lib_v2 {

absl::StatusOr<std::unique_ptr<LargeGraph>> ReadLGLFile(
    absl::string_view file_name) {
  try {
    io::LineReader reader(file_name.data());
    std::unique_ptr<LargeGraph> graph = absl::make_unique<LargeGraph>();

    absl::string_view line;
    absl::string_view source;
    while (char* line_ptr = reader.next_line()) {
      line = absl::string_view(line_ptr);

      if (line.empty()) {
        return absl::InvalidArgumentError(
            "empty line found before end of file");
      }

      if (line.length() > 2 && line.substr(0, 2).compare("# ") == 0) {
        source = std::string(line.substr(2).data());
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

}  // namespace lib_v2
}  // namespace lgl
