#include "graph.h"
#include "integer.h"
#include "nodes.h"

class IntegerToBufferNode : public Node {
  int in;
  int out;

public:
  IntegerToBufferNode() : Node("Integer To Buffer") {
    in = AddInput("in", LinkType::INTEGER);
    out = AddOutput("out", LinkType::BUFFER);
  }

  void DisplayInternal() override {}

  void ProcessInternal(Graph &graph) override {
    CryptoPP::Integer integer = GetInInteger(graph, in);
    size_t len = integer.MinEncodedSize();
    std::string str;

    str.resize(len);
    integer.Encode((CryptoPP::byte *)str.data(), str.size(),
                   CryptoPP::Integer::UNSIGNED);
    SetOutLinkBuffer(graph, out, str);
  }
};

class BufferToIntegerNode : public Node {
  int in;
  int out;

public:
  BufferToIntegerNode() : Node("Buffer To Integer") {
    in = AddInput("in", LinkType::BUFFER);
    out = AddOutput("out", LinkType::INTEGER);
  }

  void DisplayInternal() override {}

  void ProcessInternal(Graph &graph) override {
    std::string data = GetInBuffer(graph, in);
    CryptoPP::Integer integer =
        CryptoPP::Integer((CryptoPP::byte *)data.data(), data.size());
    SetOutLinkInteger(graph, out, integer);
  }
};

void ShowConverterNodesMenu(Graph &graph) {
  if (ImGui::BeginMenu("Converter")) {
    CreateButton<IntegerToBufferNode>(graph, "Integer To Buffer");
    CreateButton<BufferToIntegerNode>(graph, "Buffer To Integer");
    ImGui::EndMenu();
  }
}
