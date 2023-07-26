#include "graph.h"
#include "registry.h"

using namespace cptl;

class IntegerToBufferNode : public Node {
  InPinID in;
  OutPinID out;
  Buffer buffer;
  Integer integer;
  int encoding_type = 0;

public:
  NODE_STATIC_NAME("Integer To Buffer");
  IntegerToBufferNode() : Node(StaticNodeName(), 160.0f) {
    in = AddInputPin("in", DataType::INTEGER);
    out = AddOutputPin("out", DataType::BUFFER);
  }

  bool DisplayInternal() override {
    ImGui::SetNextItemWidth(width - ImGui::CalcTextSize("encoding").x);
    return ImGui::Combo("encoding", &encoding_type,
                        "unsigned\0signed\0DER\0OpenPGP\0\0", 4);
  }
  void RecalculateInternal() override {
    integer = GetInInteger(in);

    buffer.sink_ref().SkipAll();
    buffer.clear();

    switch (encoding_type) {
    case 0:
      integer.Encode(buffer.sink_ref(),
                     integer.MinEncodedSize(CryptoPP::Integer::UNSIGNED),
                     CryptoPP::Integer::UNSIGNED);
      break;
    case 1:
      integer.Encode(buffer.sink_ref(),
                     integer.MinEncodedSize(CryptoPP::Integer::SIGNED),
                     CryptoPP::Integer::SIGNED);
      break;
    case 2:
      integer.BEREncode(buffer.sink_ref());
      break;
    case 3:
      integer.OpenPGPEncode(buffer.sink_ref());
      break;
    }

    SetOutBuffer(out, buffer);
  }
};

class BufferToIntegerNode : public Node {
  InPinID in;
  OutPinID out;
  Buffer buffer;
  Integer integer;
  int decoding_type = 0;

public:
  NODE_STATIC_NAME("Buffer To Integer");
  BufferToIntegerNode() : Node(StaticNodeName(), 160.0f) {
    in = AddInputPin("in", DataType::BUFFER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override {
    ImGui::SetNextItemWidth(width - ImGui::CalcTextSize("decoding").x);
    return ImGui::Combo("decoding", &decoding_type,
                        "unsigned\0signed\0DER\0OpenPGP\0\0", 4);
  }
  void RecalculateInternal() override {
    buffer = GetInBuffer(in);
    switch (decoding_type) {
    case 0:
      integer = CryptoPP::Integer(buffer.const_byte_ptr(), buffer.length(),
                                  CryptoPP::Integer::UNSIGNED);
      break;
    case 1:
      integer = CryptoPP::Integer(buffer.const_byte_ptr(), buffer.length(),
                                  CryptoPP::Integer::SIGNED);
      break;
    case 2:
      integer.BERDecode(buffer.const_byte_ptr(), buffer.length());
      break;
    case 3:
      integer.OpenPGPDecode(buffer.const_byte_ptr(), buffer.length());
      break;
    }

    SetOutInteger(out, integer);
  }
};

DEFINE_NODE(IntegerToBufferNode, Converter);
DEFINE_NODE(BufferToIntegerNode, Converter);
