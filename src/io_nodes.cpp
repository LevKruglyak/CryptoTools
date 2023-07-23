#include "graph.h"
#include "imgui.h"
#include "integer.h"
#include "misc/cpp/imgui_stdlib.h"
#include "nodes.h"
#include "ui.h"
#include "utils.h"
#include <iostream>
#include <sstream>

class StringInputNode : public Node {
  std::string content = "";
  int out;

public:
  StringInputNode() : Node("String Input", 200.0f) {
    out = AddOutput("out", LinkType::BUFFER);
  }

  bool DisplayInternal() override {
    ImGui::SetNextItemWidth(width);
    bool modified = ImGui::InputTextMultiline(
        "##buffer", &content, ImVec2(width, ImGui::GetTextLineHeight() * 8));
    ImGui::Text("%lu bytes", content.size());
    return modified;
  }

  void ProcessInternal(Graph &graph) override {
    SetOutLinkBuffer(graph, out, content);
  }
};

class HexInputNode : public Node {
  std::string content = "";
  std::string decoded = "";
  size_t size = 0;
  int out;

public:
  HexInputNode() : Node("Hex Input", 200.0f) {
    out = AddOutput("out", LinkType::BUFFER);
  }

  bool DisplayInternal() override {
    ImGui::SetNextItemWidth(width);
    bool modified = ImGui::InputTextMultiline(
        "##buffer", &content, ImVec2(width, ImGui::GetTextLineHeight() * 8),
        ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::Text("%lu bytes", decoded.size());
    return modified;
  }

  void ProcessInternal(Graph &graph) override {
    decoded = utils::hexDecode(content);
    SetOutLinkBuffer(graph, out, decoded);
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

  bool DisplayInternal() override {
    ImGui::SetNextItemWidth(width);
    return ImGui::InputTextMultiline(
        "##buffer", &content, ImVec2(width, ImGui::GetTextLineHeight() * 8),
        ImGuiInputTextFlags_ReadOnly);
  }

  void ProcessInternal(Graph &graph) override {
    content = GetInBuffer(graph, in);
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

  bool DisplayInternal() override {
    ImGui::HexViewer(content);
    return false;
  }

  void ProcessInternal(Graph &graph) override {
    content = GetInBuffer(graph, in);
  }
};

class IntegerDisplayNode : public Node {
  CryptoPP::Integer content;
  int in;

public:
  IntegerDisplayNode() : Node("Integer Display") {
    width = 200.0f;
    in = AddInput("in", LinkType::INTEGER, false);
  }

  bool DisplayInternal() override {
    std::ostringstream stm;
    stm << content;
    std::string str = stm.str();

    // Remove decimal place
    str.erase(str.size() - 1);

    ImGui::SetNextItemWidth(width);
    bool modified = ImGui::InputText("##integer_output", &str,
                                     ImGuiInputTextFlags_ReadOnly);
    ImGui::Text("%d bits", content.BitCount());
    return modified;
  }

  void ProcessInternal(Graph &graph) override {
    content = GetInInteger(graph, in);
  }
};

class IntegerInputNode : public Node {
  std::string content;
  int out;

public:
  IntegerInputNode() : Node("Integer Input") {
    width = 200.0f;
    out = AddOutput("out", LinkType::INTEGER);
  }

  bool DisplayInternal() override {
    ImGui::SetNextItemWidth(width);
    return ImGui::InputText("##integer_input", &content,
                            ImGuiInputTextFlags_CharsDecimal);
  }

  void ProcessInternal(Graph &graph) override {
    CryptoPP::Integer integer = CryptoPP::Integer(content.c_str());
    SetOutLinkInteger(graph, out, integer);
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

  bool DisplayInternal() override { return false; }

  void ProcessInternal(Graph &graph) override {
    std::string concat = GetInBuffer(graph, in1) + GetInBuffer(graph, in2);
    SetOutLinkBuffer(graph, out, concat);
  }
};

class CommentNode : public Node {
  std::string comment;

public:
  CommentNode() : Node("Comment") {}

  bool DisplayInternal() override {
    ImGui::TextUnformatted(comment.c_str());

    if (ImGui::Button("Edit")) {
      ImGui::OpenPopup("Edit Comment");
    }

    if (ImGui::BeginPopupModal("Edit Comment")) {
      ImGui::InputTextMultiline("##comment_input", &comment,
                                ImVec2(-FLT_MIN, -FLT_MIN));
      if (ImGui::Button("close")) {
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }

    return false;
  }

  void ProcessInternal(Graph &graph) override {}
};

void ShowIONodesMenu(Graph &graph) {
  if (ImGui::BeginMenu("Input")) {
    CreateButton<StringInputNode>(graph, "String Input");
    CreateButton<HexInputNode>(graph, "Hex Input");
    CreateButton<IntegerInputNode>(graph, "Integer Input");
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Output")) {
    CreateButton<StringDisplayNode>(graph, "String Display");
    CreateButton<HexDisplayNode>(graph, "Hex Display");
    CreateButton<IntegerDisplayNode>(graph, "Integer Display");
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Buffer")) {
    CreateButton<ConcatBufferNode>(graph, "Concat Buffer");
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Comment")) {
    CreateButton<CommentNode>(graph, "Comment");
    ImGui::EndMenu();
  }
}
