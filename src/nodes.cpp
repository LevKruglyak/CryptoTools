#include "imgui.h"
#include "imnodes.h"
#include "ui.h"
#include <cfloat>
#include <iostream>

class Editor {
  ImNodesContext *ctx;
  bool minimap = true;
  bool snapgrid = true;

public:
  void RunToolbar() {
    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("Settings")) {
        ImGui::Checkbox("Minimap", &minimap);
        ImGui::Checkbox("Snap", &snapgrid);
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
  }

  void Run() {
    RunToolbar();

    ImNodesStyle &style = ImNodes::GetStyle();
    style.Flags |= ImNodesStyleFlags_GridLines;
    if (snapgrid) {
      style.Flags |= ImNodesStyleFlags_GridSnapping;
    }

    ImNodes::SetCurrentContext(ctx);
    ImNodes::BeginNodeEditor();

    if (minimap) {
      ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    }
    ImNodes::EndNodeEditor();
  }

  Editor() { ctx = ImNodes::CreateContext(); }
  ~Editor() { ImNodes::DestroyContext(ctx); }
};

void NodesFunction() {}

HelloImGui::DockableWindow createNodesWindow() {
  static Editor editor;
  HelloImGui::DockableWindow window;
  window.label = "Node Editor";
  window.dockSpaceName = dockArea;
  window.isVisible = false;
  window.GuiFunction = [&] { editor.Run(); };
  window.imGuiWindowFlags |= ImGuiWindowFlags_MenuBar;
  return window;
}
