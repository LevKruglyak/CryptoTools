#pragma once

#include "graph.h"
#include "registry.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "blake2.h"
#include "keccak.h"
#include "md5.h"
#include "ripemd.h"
#include "sha.h"
#include "sha3.h"
#include "shake.h"

using namespace cptl;

template <typename HASH> class HashNode : public cptl::Node {
  InPinID in;
  OutPinID out;

public:
  NODE_STATIC_NAME(HASH::StaticAlgorithmName());

  HashNode() : Node(StaticNodeName()) {
    in = AddInputPin("in", DataType::BUFFER);
    out = AddOutputPin("out", DataType::BUFFER);
  }

  bool DisplayInternal() override { return false; }
  void RecalculateInternal() override {
    auto in_buffer = GetInBuffer(in);

    // Perform hash computation
    CryptoPP::byte digest[HASH::DIGESTSIZE];
    HASH().CalculateDigest(digest, in_buffer.const_byte_ptr(),
                           in_buffer.length());

    SetOutBuffer(out, Buffer(digest, HASH::DIGESTSIZE));
  }
};

DEFINE_NODE(HashNode<CryptoPP::Weak1::MD5>, Hash);
DEFINE_NODE(HashNode<CryptoPP::RIPEMD128>, Hash);
DEFINE_NODE(HashNode<CryptoPP::RIPEMD160>, Hash);
DEFINE_NODE(HashNode<CryptoPP::RIPEMD256>, Hash);
DEFINE_NODE(HashNode<CryptoPP::RIPEMD320>, Hash);
DEFINE_NODE(HashNode<CryptoPP::SHA1>, Hash);
DEFINE_NODE(HashNode<CryptoPP::SHA224>, Hash);
DEFINE_NODE(HashNode<CryptoPP::SHA256>, Hash);
DEFINE_NODE(HashNode<CryptoPP::SHA384>, Hash);
DEFINE_NODE(HashNode<CryptoPP::SHA512>, Hash);
DEFINE_NODE(HashNode<CryptoPP::SHA3_224>, Hash);
DEFINE_NODE(HashNode<CryptoPP::SHA3_256>, Hash);
DEFINE_NODE(HashNode<CryptoPP::SHA3_384>, Hash);
DEFINE_NODE(HashNode<CryptoPP::SHA3_512>, Hash);
DEFINE_NODE(HashNode<CryptoPP::Keccak_224>, Hash);
DEFINE_NODE(HashNode<CryptoPP::Keccak_256>, Hash);
DEFINE_NODE(HashNode<CryptoPP::Keccak_384>, Hash);
DEFINE_NODE(HashNode<CryptoPP::Keccak_512>, Hash);
DEFINE_NODE(HashNode<CryptoPP::BLAKE2s>, Hash);
DEFINE_NODE(HashNode<CryptoPP::BLAKE2b>, Hash);
DEFINE_NODE(HashNode<CryptoPP::SHAKE128>, Hash);
DEFINE_NODE(HashNode<CryptoPP::SHAKE256>, Hash);
