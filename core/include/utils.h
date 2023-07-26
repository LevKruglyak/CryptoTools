#include <fmt/format.h>
#include <string>

namespace cptl {
template <typename... Args>
inline std::string format(std::string_view format, Args... args) {
  return fmt::format(fmt::runtime(format), args...);
}
} // namespace cptl
