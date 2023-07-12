#include "hello_imgui/hello_imgui.h"

#include <cryptopp/cryptlib.h>
#include <sstream>

int main(int, char *[]) {
  int version = CryptoPP::LibraryVersion();
  std::ostringstream stm;
  stm << "Crypto++ Library version: " << version;

  HelloImGui::Run([] { ImGui::Text("Hello, World!"); }, stm.str(), false);
  return 0;
}
