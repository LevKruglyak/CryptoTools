#pragma once

#include "imnodes.h"
#include "integer.h"
#include <cryptopp/nbtheory.h>
#include <map>
#include <optional>
#include <set>
#include <variant>
#include <vector>

int GenerateId();

class ImNodesComponent {
public:
  int id;

  ImNodesComponent() { id = GenerateId(); }
};

class Node;
class Link;

class Graph {
public:
  std::map<int, std::shared_ptr<Node>> nodes;
  std::map<int, std::shared_ptr<Link>> links;

  void AddNode(std::shared_ptr<Node> node);
  void RemoveNode(int id);
  void Connect(int start, int end);
  void Deconnect(int id);

  void Process();
};

enum LinkType {
  BUFFER,
  INTEGER,
};

inline ImNodesPinShape LinkTypeToImNodesShape(LinkType type) {
  switch (type) {
  case BUFFER:
    return ImNodesPinShape_QuadFilled;
  case INTEGER:
    return ImNodesPinShape_CircleFilled;
  }
}

class Port : public ImNodesComponent {
  ImNodesPinShape shape;
  std::string label;
  float width;
  int parent_id;
  bool in;

public:
  Port(LinkType type, bool in, std::string label, int parent_id, float width)
      : in(in), label(label), parent_id(parent_id), width(width),
        ImNodesComponent() {
    shape = LinkTypeToImNodesShape(type);
  }

  void Display() {
    if (in) {
      ImNodes::BeginInputAttribute(id, shape);
      ImGui::TextUnformatted(label.c_str());
      ImNodes::EndInputAttribute();
    } else {
      ImNodes::BeginOutputAttribute(id, shape);
      const float text_width = ImGui::CalcTextSize(label.c_str()).x;
      ImGui::Indent(width + ImGui::CalcTextSize(label.c_str()).x -
                    2 * text_width);
      ImGui::TextUnformatted(label.c_str());
      ImNodes::EndOutputAttribute();
    }
  }
};

template <typename NODE> void CreateButton(Graph &graph, std::string label) {
  if (ImGui::MenuItem(label.c_str())) {
    auto node = std::make_shared<NODE>();
    graph.AddNode(node);

    ImNodes::SetNodeScreenSpacePos(node->id, ImGui::GetMousePos());
    ImNodes::SnapNodeToGrid(node->id);
  }
}

class Link : public ImNodesComponent {
  std::variant<std::string, CryptoPP::Integer> data;

public:
  int in_node;
  int in_attr;

  int out_node;
  int out_attr;

  Link(int in_node, int in_attr, int out_node, int out_attr)
      : in_node(in_node), in_attr(in_attr), out_node(out_node),
        out_attr(out_attr), ImNodesComponent() {}

  std::string &getBuffer() { return std::get<std::string>(data); }
  void setBuffer(std::string buffer) { data = buffer; }

  CryptoPP::Integer &getInteger() { return std::get<CryptoPP::Integer>(data); }
  void setInteger(CryptoPP::Integer integer) { data = integer; }

  void Display() { ImNodes::Link(id, in_attr, out_attr); }
};

class Node : public ImNodesComponent {
protected:
  float width;
  std::string label;

public:
  bool processed;

  std::vector<Port> in_ports;
  std::vector<Port> out_ports;

  // Map port attr -> link
  std::map<int, int> in_links;
  // Map port attr -> link
  std::map<int, std::set<int>> out_links;

  int internal_id;

  Node(std::string label, float width = 120.0f)
      : label(label), width(width), ImNodesComponent() {
    internal_id = GenerateId();
  }

  std::shared_ptr<Link> GetInLink(Graph &graph, int port) {
    if (in_links.count(port) > 0) {
      return graph.links[in_links[port]];
    }
    return nullptr;
  }

  void SetOutLinkBuffer(Graph &graph, int port, std::string buffer) {
    for (auto link_id : out_links[port]) {
      graph.links[link_id]->setBuffer(buffer);
    }
  }

  int AddInput(std::string label, LinkType type) {
    Port port(type, true, label, id, width);
    in_ports.push_back(port);
    return port.id;
  }

  int AddOutput(std::string label, LinkType type) {
    Port port(type, false, label, id, width);
    out_ports.push_back(port);
    return port.id;
  }
  void Display() {
    ImNodes::BeginNode(id);

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(label.c_str());
    ImNodes::EndNodeTitleBar();

    for (auto port : out_ports) {
      port.Display();
    }

    ImNodes::BeginStaticAttribute(internal_id);
    ImGui::PushID(id);
    DisplayInternal();
    ImGui::PopID();
    ImNodes::EndStaticAttribute();

    for (auto port : in_ports) {
      port.Display();
    }

    ImNodes::EndNode();
  }

  void Process(Graph &graph) {
    if (!processed) {
      processed = true;

      for (auto pair : in_links) {
        if (graph.links[pair.second] != nullptr) {
          graph.nodes[graph.links[pair.second]->in_node]->Process(graph);
        }
      }

      ProcessInternal(graph);
    }
  }

  virtual void ProcessInternal(Graph &graph) = 0;
  virtual void DisplayInternal() = 0;
};
