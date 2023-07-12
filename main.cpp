#include "hello_imgui/hello_imgui.h"

int main(int, char *[]) {
  HelloImGui::Run([] { ImGui::Text("Hello, World!"); }, "Crypto Tools", false);
  return 0;
}
