#include "editor.h"
#include "graph.h"
#include "imgui.h"
#include "main.h"

HelloImGui::DockableWindow CreateWindow_NodeEditor() {
  static Editor editor;
  HelloImGui::DockableWindow window;
  window.label = "Node Editor";
  window.dockSpaceName = DOCK_AREA;
  window.isVisible = false;
  window.GuiFunction = [&] { editor.Run(); };
  window.imGuiWindowFlags |= ImGuiWindowFlags_MenuBar;
  return window;
}
