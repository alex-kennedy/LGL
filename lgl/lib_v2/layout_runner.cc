#include "lgl/lib_v2/layout_runner.h"

#include "absl/status/status.h"

namespace lgl {
namespace lib_v2 {

absl::Status LayoutRunner::Init() { return absl::OkStatus(); }

absl::Status LayoutRunner::Run() { return absl::OkStatus(); }

absl::Status LayoutRunner::Write() { return absl::OkStatus(); }

absl::Status LayoutRunner::ConfigIsValid() {
  if (!(config_.dimensions == 2 || config_.dimensions == 3)) {
    return absl::InvalidArgumentError(absl::StrCat(
        "Dimensionality (--d) should be 2 or 3, not ", config_.dimensions));
  }
  return absl::OkStatus();
}

}  // namespace lib_v2
}  // namespace lgl
