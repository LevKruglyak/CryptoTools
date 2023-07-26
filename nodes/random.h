#include "graph.h"
#include "osrng.h"
#include "registry.h"
#include <algorithm>

using namespace cptl;

#define MAX_RANDOM_BUFFER_LEN 10000
#define MAX_RANDOM_PRIME_SIZE 4096

class RandomBufferNode : public Node {
  CryptoPP::AutoSeededRandomPool rng;
  Buffer buffer;

  int num_bytes = 0;
  OutPinID out;

public:
  NODE_STATIC_NAME("Random Buffer");
  RandomBufferNode() : Node(StaticNodeName()) {
    out = AddOutputPin("out", DataType::BUFFER);
  }

  bool DisplayInternal() override {
    ImGui::SetNextItemWidth(width - ImGui::CalcTextSize("bytes").x);
    ImGui::InputInt("bytes", &num_bytes, 0);

    num_bytes = std::clamp(num_bytes, 0, MAX_RANDOM_BUFFER_LEN);

    ImGui::SetNextItemWidth(width);
    if (ImGui::Button("Regenerate")) {
      buffer.resize(num_bytes);
      rng.GenerateBlock(buffer.byte_ptr(), num_bytes);
      return true;
    }

    return false;
  }
  void RecalculateInternal() override { SetOutBuffer(out, buffer); }
};

class RandomPrimeNode : public Node {
  CryptoPP::AutoSeededRandomPool prng;
  CryptoPP::Integer x;
  int num_bits = 0;

  OutPinID out;

public:
  NODE_STATIC_NAME("Random Prime");
  RandomPrimeNode() : Node(StaticNodeName()) {
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override {
    ImGui::SetNextItemWidth(width - ImGui::CalcTextSize("bits").x);
    ImGui::InputInt("bits", &num_bits, 0);

    num_bits = std::clamp(num_bits, 0, MAX_RANDOM_PRIME_SIZE);

    ImGui::SetNextItemWidth(width);
    if (ImGui::Button("Regenerate")) {
      CryptoPP::AlgorithmParameters params = CryptoPP::MakeParameters(
          "BitLength", num_bits)("RandomNumberType", CryptoPP::Integer::PRIME);

      x.GenerateRandom(prng, params);
      return true;
    }

    return false;
  }

  void RecalculateInternal() override { SetOutInteger(out, x); }
};

DEFINE_NODE(RandomBufferNode, Random);
DEFINE_NODE(RandomPrimeNode, Random);
