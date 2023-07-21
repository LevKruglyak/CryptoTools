#include <string>

#include <fmt/format.h>

namespace utils {
template <typename... Args>
inline std::string format(std::string_view format, Args... args) {
  return fmt::format(fmt::runtime(format), args...);
}

std::string hexdump(const std::string &input);

std::string hexEncode(std::string input);
std::string hexDecode(std::string input);

std::string javaEncode(std::string input);
std::string javaDecode(std::string input);

std::string decEncode(std::string input);
std::string decDecode(std::string input);
} // namespace utils
