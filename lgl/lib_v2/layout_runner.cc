#include "lgl/lib_v2/layout_runner.h"

#include <utility>

#include "external/com_google_absl/absl/status/status.h"

namespace lgl {
namespace lib_v2 {

absl::Status LayoutRunner::Run() {
  absl::Status config_status = CheckConfig();
  if (!config_status.ok()) {
    return config_status;
  }

  absl::Status init_status = Init();
  if (!init_status.ok()) {
    return init_status;
  }

  return absl::OkStatus();
}

absl::Status LayoutRunner::CheckConfig() {
  if (!(config_.dimensions == 2 || config_.dimensions == 3)) {
    return absl::InvalidArgumentError(absl::StrCat(
        "Dimensionality (--d) should be 2 or 3, not ", config_.dimensions));
  }

  if (config_.graph_path == "") {
    return absl::InvalidArgumentError("graph_path must be set.");
  }

  return absl::OkStatus();
}

absl::Status LayoutRunner::Init() {
  auto graph_or = ReadGraph(config_.graph_path);
  if (!graph_or.ok()) {
    return graph_or.status();
  }
  graph_ = std::move(graph_or.value());


  grid_ = absl::make_unique<Grid>(config_.dimensions);
  grid_->Init(*graph_);

  return absl::OkStatus();
}

absl::Status LayoutRunner::Write() { return absl::OkStatus(); }

}  // namespace lib_v2
}  // namespace lgl
