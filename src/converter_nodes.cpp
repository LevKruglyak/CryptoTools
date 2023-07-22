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
    std::shared_ptr<Link> in_link = GetInLink(graph, in);
    if (in_link != nullptr) {
      CryptoPP::Integer integer = in_link->getInteger();
      size_t len = integer.MinEncodedSize();
      std::string str;

      str.resize(len);
      integer.Encode((CryptoPP::byte *)str.data(), str.size(),
                     CryptoPP::Integer::UNSIGNED);
      SetOutLinkBuffer(graph, out, str);
    }
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
    std::shared_ptr<Link> in_link = GetInLink(graph, in);
    if (in_link != nullptr) {
      std::string data = in_link->getBuffer();
      CryptoPP::Integer integer =
          CryptoPP::Integer((CryptoPP::byte *)data.data(), data.size());
      SetOutLinkInteger(graph, out, integer);
    }
  }
};

void ShowConverterNodesMenu(Graph &graph) {
  if (ImGui::BeginMenu("Converter")) {
    CreateButton<IntegerToBufferNode>(graph, "Integer To Buffer");
    CreateButton<BufferToIntegerNode>(graph, "Buffer To Integer");
    ImGui::EndMenu();
  }
}
