#pragma once

#include "graph.h"
#include "registry.h"

#include "base64.h"
#include "hex.h"
#include "imgui.h"
#include <sstream>

using namespace cptl;

class StringOutputNode : public cptl::Node {
  InPinID in;

  Buffer buffer;

public:
  NODE_STATIC_NAME("String Output");
  StringOutputNode() : Node(StaticNodeName(), 200.0f) {
    in = AddInputPin("in", DataType::BUFFER);
  }

  bool DisplayInternal() override {
    bool modified = ImGui::InputTextMultiline(
        "##buffer", buffer, ImVec2(width, ImGui::GetTextLineHeight() * 4),
        ImGuiInputTextFlags_ReadOnly);
    ImGui::Text("%lu bytes", buffer.length());
    return modified;
  }

  void RecalculateInternal() override { buffer = GetInBuffer(in); }
};

template <typename Encoder> class EncoderOutput : public cptl::Node {
  InPinID in;

  Buffer buffer;
  Buffer encoded;

public:
  NODE_STATIC_NAME(Encoder::StaticName());
  EncoderOutput() : Node(StaticNodeName(), 200.0f) {
    in = AddInputPin("out", DataType::BUFFER);
  }

  bool DisplayInternal() override {
    ImGui::InputTextMultiline("##buffer", buffer,
                              ImVec2(width, ImGui::GetTextLineHeight() * 4),
                              ImGuiInputTextFlags_ReadOnly);
    ImGui::Text("%lu bytes", encoded.length());
    return false;
  }

  void RecalculateInternal() override {
    encoded = GetInBuffer(in);
    buffer = encoded.Transform<Encoder>();
  }
};

class HexEncoder : public CryptoPP::HexEncoder {
public:
  static std::string StaticName() { return "Hex Output"; }
};
class Base64Encoder : public CryptoPP::Base64Encoder {
public:
  static std::string StaticName() { return "Base64 Output"; }
};

class IntegerOutputNode : public Node {
  CryptoPP::Integer content;
  InPinID in;

public:
  NODE_STATIC_NAME("Integer Output");
  IntegerOutputNode() : Node(StaticNodeName()) {
    width = 200.0f;
    in = AddInputPin("in", DataType::INTEGER, false);
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

  void RecalculateInternal() override { content = GetInInteger(in); }
};

DEFINE_NODE(StringOutputNode, Output);
DEFINE_NODE(EncoderOutput<HexEncoder>, Output);
DEFINE_NODE(EncoderOutput<Base64Encoder>, Output);
DEFINE_NODE(IntegerOutputNode, Output);
