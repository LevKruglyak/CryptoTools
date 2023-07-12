#include "imgui.h"
#include "ui.h"
#include <iostream>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/blake2.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/keccak.h>
#include <cryptopp/md5.h>
#include <cryptopp/ripemd.h>
#include <cryptopp/sha.h>
#include <cryptopp/sha3.h>

std::string inputBytes;

Input input;
Output output;

template <typename HASH> std::string performHash(std::string input) {
  CryptoPP::byte digest[HASH::DIGESTSIZE];
  HASH().CalculateDigest(digest, (CryptoPP::byte *)input.data(),
                         input.length());
  return std::string((char *)digest, sizeof(digest));
}

void HashFunction() {
  static int algorithmId = 0;
  if (ImGui::CollapsingHeader("Settings:", ImGuiTreeNodeFlags_DefaultOpen)) {
    const char *algorithms[] = {
        "MD5",        "RIPEMD160",  "SHA1",       "SHA2_224",
        "SHA2_256",   "SHA2_384",   "SHA2_512",   "SHA3_224",
        "SHA3_256",   "SHA3_384",   "SHA3_512",   "BLAKE2b",
        "KECCAK_224", "KECCAK_256", "KECCAK_384", "KECCAK_512",
    };
    ImGui::Combo("Algorithm", &algorithmId, algorithms, 16);
  }

  if (ImGui::CollapsingHeader("Input Data:", ImGuiTreeNodeFlags_DefaultOpen)) {
    if (input.Render()) {
      std::string bytes = input.getBytes();
      std::string out;

      switch (algorithmId) {
      case 0:
        out = performHash<CryptoPP::Weak1::MD5>(bytes);
        break;
      case 1:
        out = performHash<CryptoPP::RIPEMD160>(bytes);
        break;
      case 2:
        out = performHash<CryptoPP::SHA1>(bytes);
        break;
      case 3:
        out = performHash<CryptoPP::SHA224>(bytes);
        break;
      case 4:
        out = performHash<CryptoPP::SHA256>(bytes);
        break;
      case 5:
        out = performHash<CryptoPP::SHA384>(bytes);
        break;
      case 6:
        out = performHash<CryptoPP::SHA512>(bytes);
        break;
      case 7:
        out = performHash<CryptoPP::SHA3_224>(bytes);
        break;
      case 8:
        out = performHash<CryptoPP::SHA3_256>(bytes);
        break;
      case 9:
        out = performHash<CryptoPP::SHA3_384>(bytes);
        break;
      case 10:
        out = performHash<CryptoPP::SHA3_512>(bytes);
        break;
      case 11:
        out = performHash<CryptoPP::BLAKE2b>(bytes);
        break;
      case 12:
        out = performHash<CryptoPP::Keccak_224>(bytes);
        break;
      case 13:
        out = performHash<CryptoPP::Keccak_256>(bytes);
        break;
      case 14:
        out = performHash<CryptoPP::Keccak_384>(bytes);
        break;
      case 15:
        out = performHash<CryptoPP::Keccak_512>(bytes);
        break;
      }

      output.setBytes(out);
    }
  }

  if (ImGui::CollapsingHeader("Output Data:", ImGuiTreeNodeFlags_DefaultOpen)) {
    output.Render();
  }
}

HelloImGui::DockableWindow createHashWindow() {
  HelloImGui::DockableWindow window;
  window.label = "Hash";
  window.dockSpaceName = dockArea;
  window.isVisible = false;
  window.GuiFunction = HashFunction;
  return window;
}
