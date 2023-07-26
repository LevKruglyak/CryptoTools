#include "imgui.h"
#include "windows.h"

void Run() {
  ImGui::TextUnformatted("Hello, World!");

  if (ImGui::Button("Example Button")) {
    // Do something
  }
}

HelloImGui::DockableWindow CreateWindow_WINDOW_NAME() {
  HelloImGui::DockableWindow window;
  window.label = "WINDOW_NAME";
  window.dockSpaceName = DOCK_AREA;
  window.isVisible = false;
  window.GuiFunction = [&] { Run(); };
  // window.imGuiWindowFlags = // Any window flags here

  return window;
}
