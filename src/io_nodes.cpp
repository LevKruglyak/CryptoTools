#include "graph.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "nodes.h"
#include "utils.h"
#include <iostream>

class StringInputNode : public Node {
  std::string content = "";
  int out;

public:
  StringInputNode() : Node("String Input", 200.0f) {
    out = AddOutput("out", LinkType::BUFFER);
  }

  void DisplayInternal() override {
    ImGui::SetNextItemWidth(width);
    ImGui::InputTextMultiline("##buffer", &content,
                              ImVec2(width, ImGui::GetTextLineHeight() * 8));
    ImGui::Text("%lu bytes", content.size());
  }

  void ProcessInternal(Graph &graph) override {
    SetOutLinkBuffer(graph, out, content);
  }
};

class HexInputNode : public Node {
  std::string content = "";
  int out;

public:
  HexInputNode() : Node("Hex Input", 200.0f) {
    out = AddOutput("out", LinkType::BUFFER);
  }

  void DisplayInternal() override {
    ImGui::SetNextItemWidth(width);
    ImGui::InputTextMultiline("##buffer", &content,
                              ImVec2(width, ImGui::GetTextLineHeight() * 8));
    ImGui::Text("%lu bytes", content.size());
  }

  void ProcessInternal(Graph &graph) override {
    SetOutLinkBuffer(graph, out, utils::hexDecode(content));
  }
};

class StringDisplayNode : public Node {
  std::string content;
  int in;

public:
  StringDisplayNode() : Node("String Display") {
    width = 200.0f;
    in = AddInput("in", LinkType::BUFFER);
  }

  void DisplayInternal() override {
    ImGui::InputTextMultiline("##buffer", &content,
                              ImVec2(width, ImGui::GetTextLineHeight() * 8),
                              ImGuiInputTextFlags_ReadOnly);
  }

  void ProcessInternal(Graph &graph) override {
    std::shared_ptr<Link> in_link = GetInLink(graph, in);
    if (in_link != nullptr) {
      content = in_link->getBuffer();
    } else {
      content = "";
    }
  }
};

class HexDisplayNode : public Node {
  std::string content;
  int in;

public:
  HexDisplayNode() : Node("Hex Display") {
    width = 200.0f;
    in = AddInput("in", LinkType::BUFFER);
  }

  void DisplayInternal() override {
    ImGui::InputTextMultiline("##buffer", &content,
                              ImVec2(width, ImGui::GetTextLineHeight() * 8),
                              ImGuiInputTextFlags_ReadOnly);
  }

  void ProcessInternal(Graph &graph) override {
    std::shared_ptr<Link> in_link = GetInLink(graph, in);
    if (in_link != nullptr) {
      content = utils::hexEncode(in_link->getBuffer());
    } else {
      content = "";
    }
  }
};

class ConcatBufferNode : public Node {
  int in1;
  int in2;

  int out;

public:
  ConcatBufferNode() : Node("Concat Buffer") {
    in1 = AddInput("first", LinkType::BUFFER);
    in2 = AddInput("second", LinkType::BUFFER);

    out = AddOutput("out", LinkType::BUFFER);
  }

  void DisplayInternal() override {}

  void ProcessInternal(Graph &graph) override {
    std::shared_ptr<Link> in1_link = GetInLink(graph, in1);
    std::shared_ptr<Link> in2_link = GetInLink(graph, in2);

    if (in1_link != nullptr && in2_link != nullptr) {
      std::string in1_buf = in1_link->getBuffer();
      std::string in2_buf = in2_link->getBuffer();

      SetOutLinkBuffer(graph, out, in1_buf + in2_buf);
    } else {
      SetOutLinkBuffer(graph, out, "");
    }
  }
};

void ShowIONodesMenu(Graph &graph) {
  if (ImGui::BeginMenu("Input")) {
    CreateButton<StringInputNode>(graph, "String Input");
    CreateButton<HexInputNode>(graph, "Hex Input");
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Output")) {
    CreateButton<StringDisplayNode>(graph, "String Display");
    CreateButton<HexDisplayNode>(graph, "Hex Display");
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Buffer")) {
    CreateButton<ConcatBufferNode>(graph, "Concat Buffer");
    ImGui::EndMenu();
  }
}
