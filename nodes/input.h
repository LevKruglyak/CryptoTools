#pragma once

#include "graph.h"
#include "registry.h"

#include "base64.h"
#include "hex.h"
#include "imgui.h"

using namespace cptl;

class StringInputNode : public cptl::Node {
  OutPinID out;

  Buffer buffer;

public:
  NODE_STATIC_NAME("String Input");
  StringInputNode() : Node(StaticNodeName(), 200.0f) {
    out = AddOutputPin("out", DataType::BUFFER);
  }

  bool DisplayInternal() override {
    bool modified = ImGui::InputTextMultiline(
        "##buffer", buffer, ImVec2(width, ImGui::GetTextLineHeight() * 4));
    ImGui::Text("%lu bytes", buffer.length());
    return modified;
  }

  void RecalculateInternal() override { SetOutBuffer(out, buffer); }
};

template <typename Decoder> class DecoderInputNode : public cptl::Node {
  OutPinID out;

  Buffer buffer;
  Buffer decoded;

public:
  NODE_STATIC_NAME(Decoder::StaticName());
  DecoderInputNode() : Node(StaticNodeName(), 200.0f) {
    out = AddOutputPin("out", DataType::BUFFER);
  }

  bool DisplayInternal() override {
    bool modified = ImGui::InputTextMultiline(
        "##buffer", buffer, ImVec2(width, ImGui::GetTextLineHeight() * 4));

    ImGui::Text("%lu bytes", decoded.length());
    return modified;
  }

  void RecalculateInternal() override {
    decoded = buffer.Transform<Decoder>();
    SetOutBuffer(out, decoded);
  }
};

class HexDecoder : public CryptoPP::HexDecoder {
public:
  static std::string StaticName() { return "Hex Input"; }
};
class Base64Decoder : public CryptoPP::Base64Decoder {
public:
  static std::string StaticName() { return "Base64 Input"; }
};

class IntegerInputNode : public Node {
  std::string content;
  OutPinID out;

public:
  NODE_STATIC_NAME("Integer Input");
  IntegerInputNode() : Node(StaticNodeName()) {
    width = 200.0f;
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override {
    ImGui::SetNextItemWidth(width);
    return ImGui::InputText("##integer_input", &content,
                            ImGuiInputTextFlags_CharsDecimal);
  }

  void RecalculateInternal() override {
    CryptoPP::Integer integer = CryptoPP::Integer(content.c_str());
    SetOutInteger(out, integer);
  }
};

DEFINE_NODE(StringInputNode, Input);
DEFINE_NODE(DecoderInputNode<HexDecoder>, Input);
DEFINE_NODE(DecoderInputNode<Base64Decoder>, Input);
DEFINE_NODE(IntegerInputNode, Input);
