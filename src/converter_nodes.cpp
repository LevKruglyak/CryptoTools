#include "graph.h"
#include "imgui.h"
#include "integer.h"
#include "nodes.h"

class IntegerToBufferNode : public Node {
  int in;
  int out;
  bool sign = false;

public:
  IntegerToBufferNode() : Node("Integer To Buffer") {
    in = AddInput("in", LinkType::INTEGER);
    out = AddOutput("out", LinkType::BUFFER);
  }

  bool DisplayInternal() override { return ImGui::Checkbox("signed", &sign); }

  void ProcessInternal(Graph &graph) override {
    CryptoPP::Integer integer = GetInInteger(graph, in);
    size_t len = integer.MinEncodedSize();
    std::string str;

    str.resize(len);

    if (sign) {
      integer.Encode((CryptoPP::byte *)str.data(), str.size(),
                     CryptoPP::Integer::SIGNED);
    } else {
      integer.Encode((CryptoPP::byte *)str.data(), str.size(),
                     CryptoPP::Integer::UNSIGNED);
    }
    SetOutLinkBuffer(graph, out, str);
  }
};

class BufferToIntegerNode : public Node {
  int in;
  int out;
  bool sign = false;

public:
  BufferToIntegerNode() : Node("Buffer To Integer") {
    in = AddInput("in", LinkType::BUFFER);
    out = AddOutput("out", LinkType::INTEGER);
  }

  bool DisplayInternal() override { return ImGui::Checkbox("signed", &sign); }

  void ProcessInternal(Graph &graph) override {
    std::string data = GetInBuffer(graph, in);
    CryptoPP::Integer::Signedness sn = CryptoPP::Integer::UNSIGNED;
    if (sign) {
      sn = CryptoPP::Integer::SIGNED;
    }

    CryptoPP::Integer integer =
        CryptoPP::Integer((CryptoPP::byte *)data.data(), data.size(), sn);
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
