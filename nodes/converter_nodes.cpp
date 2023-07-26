// #include "filters.h"
// #include "graph.h"
// #include "imgui.h"
// #include "integer.h"
// #include "nodes.h"
//
// class IntegerToBufferNode : public Node {
//   int in;
//   int out;
//   std::string sink_str;
//   std::shared_ptr<CryptoPP::StringSink> sink;
//   int encoding_type = 0;
//
// public:
//   IntegerToBufferNode() : Node("Integer To Buffer") {
//     width = 160.0f;
//     in = AddInput("in", LinkType::INTEGER);
//     out = AddOutput("out", LinkType::BUFFER);
//     sink = std::make_shared<CryptoPP::StringSink>(sink_str);
//   }
//
//   bool DisplayInternal() override {
//     bool modified = false;
//     ImGui::SetNextItemWidth(width - ImGui::CalcTextSize("encoding").x);
//     return ImGui::Combo("encoding", &encoding_type,
//                         "unsigned\0signed\0DER\0OpenPGP\0\0", 4);
//   }
//
//   void ProcessInternal(Graph &graph) override {
//     CryptoPP::Integer integer = GetInInteger(graph, in);
//
//     sink->SkipAll();
//     sink_str.clear();
//     switch (encoding_type) {
//     case 0:
//       integer.Encode(sink->Ref(),
//                      integer.MinEncodedSize(CryptoPP::Integer::UNSIGNED),
//                      CryptoPP::Integer::UNSIGNED);
//       break;
//     case 1:
//       integer.Encode(sink->Ref(),
//                      integer.MinEncodedSize(CryptoPP::Integer::SIGNED),
//                      CryptoPP::Integer::SIGNED);
//       break;
//     case 2:
//       integer.BEREncode(sink->Ref());
//       break;
//     case 3:
//       integer.OpenPGPEncode(sink->Ref());
//       break;
//     }
//
//     SetOutLinkBuffer(graph, out, sink_str);
//   }
// };
//
// class BufferToIntegerNode : public Node {
//   int in;
//   int out;
//   int encoding_type = 0;
//
// public:
//   BufferToIntegerNode() : Node("Buffer To Integer") {
//     width = 160.0f;
//     in = AddInput("in", LinkType::BUFFER);
//     out = AddOutput("out", LinkType::INTEGER);
//   }
//
//   bool DisplayInternal() override {
//     ImGui::SetNextItemWidth(width - ImGui::CalcTextSize("encoding").x);
//     return ImGui::Combo("encoding", &encoding_type,
//                         "unsigned\0signed\0DER\0OpenPGP\0\0", 4);
//   }
//
//   void ProcessInternal(Graph &graph) override {
//     std::string data = GetInBuffer(graph, in);
//
//     CryptoPP::Integer integer;
//     switch (encoding_type) {
//     case 0:
//       integer = CryptoPP::Integer((CryptoPP::byte *)data.data(), data.size(),
//                                   CryptoPP::Integer::UNSIGNED);
//       break;
//     case 1:
//       integer = CryptoPP::Integer((CryptoPP::byte *)data.data(), data.size(),
//                                   CryptoPP::Integer::SIGNED);
//       break;
//     case 2:
//       integer.BERDecode((CryptoPP::byte *)data.data(), data.size());
//       break;
//     case 3:
//       integer.OpenPGPDecode((CryptoPP::byte *)data.data(), data.size());
//       break;
//     }
//
//     SetOutLinkInteger(graph, out, integer);
//   }
// };
//
// void ShowConverterNodesMenu(Graph &graph) {
//   if (ImGui::BeginMenu("Converter")) {
//     CreateButton<IntegerToBufferNode>(graph, "Integer To Buffer");
//     CreateButton<BufferToIntegerNode>(graph, "Buffer To Integer");
//     ImGui::EndMenu();
//   }
// }
