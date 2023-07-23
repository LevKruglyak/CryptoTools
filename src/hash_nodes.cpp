#include "graph.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "blake2.h"
#include "imgui.h"
#include "keccak.h"
#include "md5.h"
#include "nodes.h"
#include "ripemd.h"
#include "sha.h"
#include "sha3.h"

template <typename HASH_ALG> class HashNode : public Node {
  int in;
  int out;

public:
  HashNode() : Node(HASH_ALG::StaticAlgorithmName()) {
    in = AddInput("in", LinkType::BUFFER);
    out = AddOutput("out", LinkType::BUFFER);
  }

  void DisplayInternal() override {}

  void ProcessInternal(Graph &graph) override {
    auto content = GetInBuffer(graph, in);
    CryptoPP::byte digest[HASH_ALG::DIGESTSIZE];
    HASH_ALG().CalculateDigest(digest, (CryptoPP::byte *)content.data(),
                               content.length());
    std::string out_buf = std::string((char *)digest, HASH_ALG::DIGESTSIZE);
    SetOutLinkBuffer(graph, out, out_buf);
  }
};

template <typename HASH_ALG> void CreateHashButton(Graph &graph) {
  std::string name = HASH_ALG::StaticAlgorithmName();
  CreateButton<HashNode<HASH_ALG>>(graph, name.c_str());
}

void ShowHashNodesMenu(Graph &graph) {
  if (ImGui::BeginMenu("Hash")) {
    CreateHashButton<CryptoPP::Weak1::MD5>(graph);
    CreateHashButton<CryptoPP::RIPEMD128>(graph);
    CreateHashButton<CryptoPP::RIPEMD160>(graph);
    CreateHashButton<CryptoPP::RIPEMD256>(graph);
    CreateHashButton<CryptoPP::RIPEMD320>(graph);
    CreateHashButton<CryptoPP::SHA1>(graph);
    CreateHashButton<CryptoPP::SHA224>(graph);
    CreateHashButton<CryptoPP::SHA256>(graph);
    CreateHashButton<CryptoPP::SHA384>(graph);
    CreateHashButton<CryptoPP::SHA512>(graph);
    CreateHashButton<CryptoPP::SHA3_224>(graph);
    CreateHashButton<CryptoPP::SHA3_256>(graph);
    CreateHashButton<CryptoPP::SHA3_384>(graph);
    CreateHashButton<CryptoPP::SHA3_512>(graph);
    CreateHashButton<CryptoPP::Keccak_224>(graph);
    CreateHashButton<CryptoPP::Keccak_256>(graph);
    CreateHashButton<CryptoPP::Keccak_384>(graph);
    CreateHashButton<CryptoPP::Keccak_512>(graph);
    CreateHashButton<CryptoPP::BLAKE2s>(graph);
    CreateHashButton<CryptoPP::BLAKE2b>(graph);
    ImGui::EndMenu();
  }
}
