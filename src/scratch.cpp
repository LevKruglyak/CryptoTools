#include "imgui.h"
#include "ui.h"
#include <cfloat>
#include <iostream>

std::string buffer = "";

void ScratchFunction() {
  ImGui::InputTextMultiline("##scratch_input", &buffer,
                            ImVec2(-FLT_MIN, -FLT_MIN));
}

HelloImGui::DockableWindow createScratchWindow() {
  HelloImGui::DockableWindow window;
  window.label = "Scratch Space";
  window.dockSpaceName = dockArea;
  window.isVisible = false;
  window.GuiFunction = ScratchFunction;
  return window;
}
