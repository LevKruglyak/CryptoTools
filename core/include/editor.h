#pragma once

#include "imgui.h"
#include "imnodes.h"
#include "registry.h"

class Editor {
  ImNodesContext *ctx;
  cptl::Graph graph;
  bool minimap = true;
  bool snapgrid = true;

public:
  bool NodesMenu() {
    bool modified = false;
    for (auto pair : REFLECTED_NODES) {
      if (ImGui::BeginMenu(pair.first.c_str())) {
        for (auto node : pair.second) {
          if (ImGui::MenuItem(node.name.c_str())) {
            cptl::NodePtr ptr = node.create();
            ImNodes::SetNodeScreenSpacePos(ptr->GetID(), ImGui::GetMousePos());
            if (snapgrid) {
              ImNodes::SnapNodeToGrid(ptr->GetID());
            }
            graph.AddNode(ptr);
            modified = true;
          }
        }
        ImGui::EndMenu();
      }
    }

    return modified;
  }

  void DeleteSelected() {
    ImNodes::SetCurrentContext(ctx);
    std::vector<int> nodes = std::vector<int>(1024);
    std::vector<int> links = std::vector<int>(1024);

    ImNodes::GetSelectedLinks(links.data());
    ImNodes::GetSelectedNodes(nodes.data());

    for (int i = 0; i < ImNodes::NumSelectedLinks(); i++) {
      graph.Disconnect(links[i]);
    }

    for (int i = 0; i < ImNodes::NumSelectedNodes(); i++) {
      graph.RemoveNode(nodes[i]);
    }
  }

  bool RunToolbar(bool *needs_delete) {
    bool modified = false;

    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("Settings")) {
        ImGui::MenuItem("Minimap", NULL, &minimap);
        ImGui::MenuItem("Snap", NULL, &snapgrid);
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Nodes")) {
        modified |= NodesMenu();
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Delete Selected")) {
          if (needs_delete != nullptr) {
            *needs_delete = true;
          } else {
            DeleteSelected();
          }
          modified = true;
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    return modified;
  }

  void Run() {
    bool needs_delete = false;
    bool modified = RunToolbar(&needs_delete);

    ImNodes::SetCurrentContext(ctx);
    ImNodes::PushAttributeFlag(
        ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
    ImNodes::GetStyle().GridSpacing = 20.0f;
    ImNodes::GetStyle().NodePadding = ImVec2(5.0f, 5.0f);

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
      if (ImGui::BeginMenu("Add")) {
        modified |= NodesMenu();
        ImGui::EndMenu();
      }
      if (ImGui::MenuItem("Delete")) {
        needs_delete = true;
        modified = true;
      }
      ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);

    modified |= graph.Display();

    if (minimap) {
      ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    }

    ImNodes::EndNodeEditor();

    {
      int hovered_node;
      if (ImNodes::IsNodeHovered(&hovered_node)) {
        auto node = graph.GetNode(hovered_node);
        if (node != nullptr && node->HasError()) {
          ImGui::SetTooltip("[ERROR] %s", node->GetError().c_str());
        }
      }
    }

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

    if (needs_delete) {
      DeleteSelected();
      graph.Recalculate();
    } else if (modified) {
      graph.Recalculate();
    }

    ImNodes::PopAttributeFlag();
  }

  Editor() {
    ctx = ImNodes::CreateContext();
    ImNodes::PushColorStyle(ImNodesCol_GridBackground, ImGuiCol_FrameBg);
  }
  ~Editor() { ImNodes::DestroyContext(ctx); }
};
