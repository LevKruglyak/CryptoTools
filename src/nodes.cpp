#include "nodes.h"
#include "imgui.h"
#include "imnodes.h"
#include "ui.h"
#include <cfloat>
#include <iostream>

static int id = 0;

int GenerateId() { return id++; }

void Graph::AddNode(std::shared_ptr<Node> node) {
  nodes[node->id] = node;
  for (auto port_pair : node->in) {
    ports[port_pair.first] = port_pair.second.first;
  }
  for (auto port_pair : node->out) {
    ports[port_pair.first] = port_pair.second.first;
  }
}

void Graph::RemoveNode(int id) {
  auto node = nodes[id];
  for (auto port_pair : node->in) {
    ports.erase(port_pair.first);
  }
  for (auto port_pair : node->out) {
    ports.erase(port_pair.first);
  }
  nodes.erase(node->id);
}

void Graph::Connect(int start, int end) {
  auto start_port = ports[start];
  auto end_port = ports[end];

  if (start_port->type == end_port->type) {
    auto start_node = nodes[start_port->parent_id];
    auto end_node = nodes[end_port->parent_id];

    auto link = std::make_shared<Link>(start_node->id, start, end_node->id, end,
                                       start_port->type);

    if (end_node->in[end].second == nullptr) {
      end_node->in[end].second = link;
      start_node->out[start].second.insert(link);
      links[link->id] = link;
    }
  }
}

void Graph::Deconnect(int id) {
  auto link = links[id];

  auto start_node = nodes[link->in_node];
  auto end_node = nodes[link->out_node];

  if (start_node != nullptr) {
    start_node->out[link->in_attr].second.erase(link);
  }

  if (end_node != nullptr) {
    end_node->in[link->out_attr].second = nullptr;
  }

  links.erase(link->id);
}

void Graph::Process() {
  for (auto pair : nodes) {
    pair.second->processed = false;
  }

  for (auto pair : nodes) {
    pair.second->Process(*this);
  }
}

class Editor {
  ImNodesContext *ctx;
  Graph graph;
  bool minimap = true;
  bool snapgrid = true;

public:
  void RunNodes() {
    ShowIONodesMenu(graph);
    ShowConverterNodesMenu(graph);
    ShowHashNodesMenu(graph);
    ShowArithmeticNodesMenu(graph);
    ShowRandomNodesMenu(graph);
  }

  void DeleteSelected() {
    ImNodes::SetCurrentContext(ctx);
    int nodes[ImNodes::NumSelectedNodes()];
    int links[ImNodes::NumSelectedLinks()];

    ImNodes::GetSelectedLinks(links);
    ImNodes::GetSelectedNodes(nodes);

    for (auto link : links) {
      graph.Deconnect(link);
    }

    for (auto node : nodes) {
      graph.RemoveNode(node);
    }
  }

  void RunToolbar() {
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
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
  }

  void Run() {
    RunToolbar();

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
      }
      if (ImGui::BeginMenu("Add")) {
        RunNodes();
        ImGui::EndMenu();
      }
      ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);

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

    graph.Process();
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
