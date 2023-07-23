#include "graph.h"
#include "imgui.h"
#include "integer.h"
#include "nodes.h"
#include "osrng.h"
#include "secblockfwd.h"

class RandomBufferNode : public Node {
  CryptoPP::AutoSeededRandomPool prng;
  std::string bytes;
  int num_bytes = 0;
  int out;

public:
  RandomBufferNode() : Node("Random Buffer") {
    out = AddOutput("out", LinkType::BUFFER);
  }

  bool DisplayInternal() override {
    ImGui::SetNextItemWidth(width - ImGui::CalcTextSize("bytes").x);
    ImGui::InputInt("bytes", &num_bytes);

    if (num_bytes > 10000) {
      num_bytes = 10000;
    }

    if (num_bytes < 0) {
      num_bytes = 0;
    }

    ImGui::SetNextItemWidth(width);
    if (ImGui::Button("Regenerate")) {
      bytes.resize(num_bytes);
      prng.GenerateBlock((CryptoPP::byte *)bytes.data(), num_bytes);
      return true;
    }

    return false;
  }

  void ProcessInternal(Graph &graph) override {
    SetOutLinkBuffer(graph, out, bytes);
  }
};

class RandomPrimeNode : public Node {
  CryptoPP::AutoSeededRandomPool prng;
  CryptoPP::Integer x;
  std::string bytes;
  int num_bits = 0;
  int out;

public:
  RandomPrimeNode() : Node("Random Prime") {
    out = AddOutput("out", LinkType::INTEGER);
  }

  bool DisplayInternal() override {
    ImGui::SetNextItemWidth(width - ImGui::CalcTextSize("bits").x);
    ImGui::InputInt("bits", &num_bits);

    if (num_bits > 4096) {
      num_bits = 4096;
    }

    if (num_bits < 0) {
      num_bits = 0;
    }

    ImGui::SetNextItemWidth(width);
    if (ImGui::Button("Regenerate")) {
      CryptoPP::AlgorithmParameters params = CryptoPP::MakeParameters(
          "BitLength", num_bits)("RandomNumberType", CryptoPP::Integer::PRIME);

      x.GenerateRandom(prng, params);
      return true;
    }

    return false;
  }

  void ProcessInternal(Graph &graph) override {
    SetOutLinkInteger(graph, out, x);
  }
};

void ShowRandomNodesMenu(Graph &graph) {
  if (ImGui::BeginMenu("Random")) {
    CreateButton<RandomBufferNode>(graph, "Random Buffer");
    CreateButton<RandomPrimeNode>(graph, "Random Prime");
    ImGui::EndMenu();
  }
}
