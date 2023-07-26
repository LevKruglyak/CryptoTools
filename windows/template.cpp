#include "imgui.h"
#include "main.h"

void Run() {
  ImGui::TextUnformatted("Hello, World!");

  if (ImGui::Button("Example Button")) {
    // Do something
  }
}

HelloImGui::DockableWindow CreateWindow_Template() {
  HelloImGui::DockableWindow window;
  window.label = "Example Window";
  window.dockSpaceName = DOCK_AREA;
  window.isVisible = false;
  window.GuiFunction = [&] { Run(); };
  // window.imGuiWindowFlags = // Any window flags here

  return window;
}
