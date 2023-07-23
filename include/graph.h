#pragma once

#include "cryptlib.h"
#include "imgui.h"
#include "imnodes.h"
#include "integer.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cryptopp/nbtheory.h>
#include <exception>
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
class Port;

class Graph {
public:
  std::map<int, std::shared_ptr<Node>> nodes;
  std::map<int, std::shared_ptr<Link>> links;
  std::map<int, std::shared_ptr<Port>> ports;

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
    return ImNodesPinShape_TriangleFilled;
  }
}

class Port : public ImNodesComponent {

public:
  std::string input;
  CryptoPP::Integer input_integer;
  ImNodesPinShape shape;
  LinkType type;
  std::string label;
  float width;
  bool show_input;
  int parent_id;
  bool in;

  Port(LinkType type, bool in, std::string label, int parent_id, float width,
       bool show_input = true)
      : in(in), label(label), parent_id(parent_id), width(width), type(type),
        show_input(show_input), ImNodesComponent() {
    shape = LinkTypeToImNodesShape(type);
  }

  void Display(bool connected) {
    if (in) {
      ImNodes::BeginInputAttribute(id, shape);
      if (show_input && type == INTEGER && !connected) {
        ImGui::PushID(id);
        ImGui::SetNextItemWidth(width - ImGui::CalcTextSize(label.c_str()).x);
        if (ImGui::InputText(label.c_str(), &input,
                             ImGuiInputTextFlags_CharsDecimal)) {
          input_integer = CryptoPP::Integer(input.c_str());
        }
        ImGui::PopID();
      } else {
        ImGui::TextUnformatted(label.c_str());
      }
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

  const CryptoPP::Integer &GetManualInputInteger() { return input_integer; }
};

template <typename NODE> void CreateButton(Graph &graph, std::string label) {
  if (ImGui::MenuItem(label.c_str())) {
    auto node = std::make_shared<NODE>();
    graph.AddNode(node);

    ImNodes::SetNodeScreenSpacePos(node->id, ImGui::GetMousePos());
    ImNodes::SnapNodeToGrid(node->id);
  }
}

static std::string empty_buffer = "";

class Link : public ImNodesComponent {
  std::variant<std::string, CryptoPP::Integer> data;

public:
  int in_node;
  int in_attr;

  int out_node;
  int out_attr;

  std::string &getBuffer() { return std::get<std::string>(data); }
  void setBuffer(const std::string &buffer) { data = buffer; }

  CryptoPP::Integer &getInteger() { return std::get<CryptoPP::Integer>(data); }
  void setInteger(const CryptoPP::Integer &integer) { data = integer; }

  void Display() { ImNodes::Link(id, in_attr, out_attr); }

  Link(int in_node, int in_attr, int out_node, int out_attr, LinkType type)
      : in_node(in_node), in_attr(in_attr), out_node(out_node),
        out_attr(out_attr), ImNodesComponent() {
    switch (type) {
    case BUFFER:
      setBuffer(empty_buffer);
      break;
    case INTEGER:
      setInteger(CryptoPP::Integer::Zero());
      break;
    }
  }
};

class Node : public ImNodesComponent {
protected:
  float width;
  std::string label;

public:
  bool processed;
  bool error = false;
  std::string error_message;

  std::map<int, std::pair<std::shared_ptr<Port>, std::shared_ptr<Link>>> in;
  std::map<int,
           std::pair<std::shared_ptr<Port>, std::set<std::shared_ptr<Link>>>>
      out;

  int internal_id;

  Node(std::string label, float width = 120.0f)
      : label(label), width(width), ImNodesComponent() {
    internal_id = GenerateId();
  }

  const CryptoPP::Integer &GetInInteger(Graph &graph, int port) {
    if (in[port].second != nullptr) {
      return in[port].second->getInteger();
    } else {
      return in[port].first->GetManualInputInteger();
    }

    return CryptoPP::Integer::Zero();
  }

  const std::string &GetInBuffer(Graph &graph, int port) {
    if (in[port].second != nullptr) {
      return in[port].second->getBuffer();
    }

    return empty_buffer;
  }

  void SetOutLinkBuffer(Graph &graph, int port, std::string &buffer) {
    for (auto out_link : out[port].second) {
      graph.links[out_link->id]->setBuffer(buffer);
    }
  }

  void SetOutLinkInteger(Graph &graph, int port, CryptoPP::Integer &integer) {
    for (auto out_link : out[port].second) {
      graph.links[out_link->id]->setInteger(integer);
    }
  }

  int AddInput(std::string label, LinkType type, bool show_input = true) {
    Port port(type, true, label, id, width, show_input);
    in[port.id] = std::make_pair(std::make_shared<Port>(port), nullptr);
    return port.id;
  }

  int AddOutput(std::string label, LinkType type) {
    Port port(type, false, label, id, width);
    out[port.id] = std::make_pair(std::make_shared<Port>(port),
                                  std::set<std::shared_ptr<Link>>());
    return port.id;
  }
  void Display() {
    ImNodes::BeginNode(id);

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(label.c_str());
    ImNodes::EndNodeTitleBar();

    for (auto port_link : out) {
      port_link.second.first->Display(false);
    }

    ImNodes::BeginStaticAttribute(internal_id);
    ImGui::PushID(id);
    DisplayInternal();

    if (error) {
      ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + width);
      ImGui::TextDisabled("[ERROR] %s", error_message.c_str());
      ImGui::PopTextWrapPos();
    }
    ImGui::PopID();
    ImNodes::EndStaticAttribute();

    for (auto port_link : in) {
      port_link.second.first->Display(port_link.second.second != nullptr);
    }

    ImNodes::EndNode();
  }

  void Process(Graph &graph) {
    if (!processed) {
      processed = true;

      for (auto pair : in) {
        auto link = pair.second.second;
        if (link != nullptr) {
          graph.nodes[link->in_node]->Process(graph);
        }
      }

      try {
        ProcessInternal(graph);
        error = false;
        error_message = "";
      } catch (CryptoPP::Exception &e) {
        error_message = std::string(e.what());
        error = true;
      } catch (std::exception &e) {
        error_message = std::string(e.what());
        error = true;
      }
    }
  }

  virtual void ProcessInternal(Graph &graph) = 0;
  virtual void DisplayInternal() = 0;
};
