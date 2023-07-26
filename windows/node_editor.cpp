#include "graph.h"
#include "imgui.h"
#include "imnodes.h"
#include "main.h"
#include "ui.h"
#include "windows.h"
#include <cfloat>
#include <iostream>

class Editor {
  ImNodesContext *ctx;
  cptl::Graph graph;
  bool minimap = true;
  bool snapgrid = true;

public:
  void RunNodes() {
    // ShowIONodesMenu(graph);
    // ShowConverterNodesMenu(graph);
    // ShowHashNodesMenu(graph);
    // ShowArithmeticNodesMenu(graph);
    // ShowRandomNodesMenu(graph);
  }

  void DeleteSelected() {
    ImNodes::SetCurrentContext(ctx);

    std::vector<int> nodes(ImNodes::NumSelectedNodes());
    std::vector<int> links(ImNodes::NumSelectedLinks());

    ImNodes::GetSelectedLinks(links.data());
    ImNodes::GetSelectedNodes(nodes.data());

    for (auto link : links) {
      graph.Disconnect(link);
    }

    for (auto node : nodes) {
      graph.RemoveNode(node);
    }
  }

  bool RunToolbar() {
    bool modified = false;

    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("Settings")) {
        ImGui::MenuItem("Minimap", NULL, &minimap);
        ImGui::MenuItem("Snap", NULL, &snapgrid);
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Nodes")) {
        RunNodes();
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Delete Selected")) {
          DeleteSelected();
          modified = true;
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    return modified;
  }

  void Run() {
    bool modified = RunToolbar();

    ImNodes::SetCurrentContext(ctx);
    ImNodes::PushAttributeFlag(
        ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

    ImNodesIO &io = ImNodes::GetIO();
    io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
    io.MultipleSelectModifier.Modifier = &ImGui::GetIO().KeyCtrl;

    ImNodesStyle &style = ImNodes::GetStyle();
    style.Flags |= ImNodesStyleFlags_GridLines;
    if (snapgrid) {
      style.Flags |= ImNodesStyleFlags_GridSnapping;
    } else {

      style.Flags &= ~ImNodesStyleFlags_GridSnapping;
    }

    auto window_pad = ImGui::GetStyle().WindowPadding;
    auto frame_pad = ImGui::GetStyle().WindowPadding;
    ImNodes::BeginNodeEditor();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, window_pad);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, frame_pad);
    if (ImGui::BeginPopupContextWindow()) {
      if (ImGui::MenuItem("Delete")) {
        DeleteSelected();
        modified = true;
      }
      if (ImGui::BeginMenu("Add")) {
        RunNodes();
        ImGui::EndMenu();
      }
      ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);

    graph.Display();

    if (minimap) {
      ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    }

    ImNodes::EndNodeEditor();

    {
      int start;
      int end;

      if (ImNodes::IsLinkCreated(&start, &end)) {
        graph.Connect(start, end);
        modified = true;
      }
    }

    {
      int id;

      if (ImNodes::IsLinkDestroyed(&id)) {
        graph.Disconnect(id);
        modified = true;
      }
    }

    if (modified) {
      graph.Recalculate();
    }
    ImNodes::PopAttributeFlag();
  }

  Editor() { ctx = ImNodes::CreateContext(); }
  ~Editor() { ImNodes::DestroyContext(ctx); }
};

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
