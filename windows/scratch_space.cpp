#include "imgui.h"
#include "main.h"
#include "windows.h"
#include <string>

std::string buffer = "";

void ScratchFunction() {
  ImGui::InputTextMultiline("##scratch_input", &buffer,
                            ImVec2(-FLT_MIN, -FLT_MIN));
}

HelloImGui::DockableWindow CreateWindow_ScratchSpace() {
  HelloImGui::DockableWindow window;
  window.label = "Scratch Space";
  window.dockSpaceName = DOCK_AREA;
  window.isVisible = false;
  window.GuiFunction = ScratchFunction;
  return window;
}
