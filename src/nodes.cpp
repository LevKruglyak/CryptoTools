#include "nodes.h"
#include "imgui.h"
#include "imnodes.h"
#include "ui.h"
#include <cfloat>
#include <iostream>

void Graph::AddNode(std::shared_ptr<Node> node) {
  nodes[node->id] = node;
  for (auto port : node->in_ports) {
    nodes[port.id] = node;
  }
  for (auto port : node->out_ports) {
    nodes[port.id] = node;
  }
}

void Graph::RemoveNode(int id) {
  auto node = nodes[id];
  for (auto port : node->in_ports) {
    nodes.erase(port.id);
  }
  for (auto port : node->out_ports) {
    nodes.erase(port.id);
  }
  nodes.erase(node->id);
}

void Graph::Connect(int start, int end) {
  auto start_node = nodes[start];
  auto end_node = nodes[end];

  auto link = std::make_shared<Link>(start_node->id, start, end_node->id, end);
  links[link->id] = link;

  start_node->out_links[start] = link->id;
  end_node->in_links[end] = link->id;
}

void Graph::Deconnect(int id) {
  auto link = links[id];

  auto start_node = nodes[link->in_node];
  auto end_node = nodes[link->out_node];

  start_node->out_links.erase(link->in_attr);
  end_node->in_links.erase(link->out_attr);
  links.erase(link->id);
}

class Editor {
  ImNodesContext *ctx;
  Graph graph;
  bool minimap = true;
  bool snapgrid = true;

public:
  void RunToolbar() {
    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("Settings")) {
        ImGui::MenuItem("Minimap", NULL, &minimap);
        ImGui::MenuItem("Snap", NULL, &snapgrid);
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Nodes")) {
        ShowIONodesMenu(graph);
        ShowHashNodesMenu(graph);
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
    } else {

      style.Flags &= ~ImNodesStyleFlags_GridSnapping;
    }

    ImNodes::PushAttributeFlag(
        ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

    ImNodesIO &io = ImNodes::GetIO();
    io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
    io.MultipleSelectModifier.Modifier = &ImGui::GetIO().KeyCtrl;

    ImNodes::SetCurrentContext(ctx);
    ImNodes::BeginNodeEditor();

    for (auto pair : graph.nodes) {
      pair.second->Display();
    }

    for (auto pair : graph.links) {
      pair.second->Display();
    }

    if (minimap) {
      ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    }
    ImNodes::EndNodeEditor();

    {
      int start;
      int end;

      if (ImNodes::IsLinkCreated(&start, &end)) {
        graph.Connect(start, end);
      }
    }

    {
      int id;

      if (ImNodes::IsLinkDestroyed(&id)) {
        graph.Deconnect(id);
      }
    }

    ImNodes::PopAttributeFlag();
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
