#include "utils.h"

#include <cctype>
#include <cryptopp/hex.h>
#include <cstdint>
#include <iomanip>
#include <sstream>

namespace utils {
std::string hexEncode(std::string input) {
  std::string encoded;

  CryptoPP::HexEncoder encoder;
  encoder.Put((CryptoPP::byte *)input.data(), input.size());
  encoder.MessageEnd();

  CryptoPP::word64 size = encoder.MaxRetrievable();
  if (size && size <= SIZE_MAX) {
    encoded.resize(size);
    encoder.Get((CryptoPP::byte *)&encoded[0], encoded.size());
  }

  return encoded;
}

std::string hexDecode(std::string input) {
  std::string decoded;

  CryptoPP::HexDecoder decoder;
  decoder.Put((CryptoPP::byte *)input.data(), input.size());
  decoder.MessageEnd();

  CryptoPP::word64 size = decoder.MaxRetrievable();
  if (size && size <= SIZE_MAX) {
    decoded.resize(size);
    decoder.Get((CryptoPP::byte *)&decoded[0], decoded.size());
  }

  return decoded;
}

std::string javaEncode(std::string input) { return ""; }
std::string javaDecode(std::string input) { return ""; }

std::string decEncode(std::string input) { return ""; }
std::string decDecode(std::string input) { return ""; }

std::string hexdump(const std::string &input) {
  if (input.length() == 0) {
    return "No data!";
  }

  std::ostringstream os;
  int line_count = 0;

  for (size_t i = 0; i < input.size(); i += 16) {
    os << std::hex << std::setw(8) << std::setfill('0') << line_count << "  ";

    for (size_t j = i; j < i + 16; ++j) {
      if (j < input.size()) {
        os << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(static_cast<unsigned char>(input[j])) << ' ';
      } else {
        os << "   ";
      }

      if (j == i + 7) {
        os << ' ';
      }
    }

    os << " ";

    for (size_t j = i; j < i + 16 && j < input.size(); ++j) {
      char c = input[j];
      os << (std::isprint(static_cast<unsigned char>(c)) ? c : '.');
    }

    os << "\n";
    line_count += 16;
  }

  return os.str();
}
} // namespace utils
