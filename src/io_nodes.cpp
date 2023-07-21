#include "graph.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "nodes.h"

class BufferInputNode : public Node {
  std::string content;

public:
  BufferInputNode() : Node("Buffer Input", 200.0f) {
    // content = std::make_shared<LinkData>(LinkData{std::string()});
    AddOutput("out", LinkType::BUFFER);
  }

  void DisplayInternal() override {
    ImGui::PushID(this);
    ImGui::InputTextMultiline("##buffer_input_node", &content,
                              ImVec2(width, ImGui::GetTextLineHeight() * 8));
    ImGui::Text("%lu bytes", content.size());
    ImGui::PopID();
  }

  void ProcessInternal() override {}
};

class BufferDisplayNode : public Node {
public:
  BufferDisplayNode() : Node("Buffer Display") {
    width = 200.0f;
    AddInput("in", LinkType::BUFFER);
  }

  void DisplayInternal() override {
    ImGui::PushID(this);
    // HexViewer(&data.get()->buffer, width);
    ImGui::PopID();
  }

  void ProcessInternal() override {}
};

void ShowIONodesMenu(Graph &graph) {
  if (ImGui::BeginMenu("Input")) {
    CreateButton<BufferInputNode>(graph, "Buffer Input");
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Output")) {
    CreateButton<BufferDisplayNode>(graph, "Buffer Display");
    ImGui::EndMenu();
  }
}
