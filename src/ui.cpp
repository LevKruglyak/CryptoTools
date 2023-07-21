#include "ui.h"
#include "imgui.h"
#include "utils.h"
#include <iostream>

void HexViewer(std::string bytes, float clip) {
  static const std::string Header =
      " Address      00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F          "
      "             ";
  static const std::string NoData =
      "                                                      No Data!          "
      "              ";

  ImGui::Separator();
  if (ImGui::BeginChild("##hex_view",
                        ImVec2(ImGui::CalcTextSize(Header.c_str()).x, clip)),
      true) {
    ImGui::TextDisabled("%s", Header.c_str());

    auto size = bytes.size();
    if (size == 0) {
      ImGui::TextUnformatted(NoData.c_str());
    } else {
      ImGuiListClipper clipper;

      clipper.Begin((size + 0x0F) / 0x10);

      while (clipper.Step())
        for (auto y = clipper.DisplayStart; y < clipper.DisplayEnd; y++) {
          auto lineSize = ((size - y * 0x10) < 0x10) ? size % 0x10 : 0x10;

          ImGui::TextDisabled("%s",
                              utils::format(" {:07X}:  ", y * 0x10).c_str());
          std::string line;

          for (unsigned int x = 0; x < 0x10; x++) {
            if (x < lineSize)
              line += utils::format("{:02X} ", bytes[y * 0x10 + x]);
            else
              line += "   ";

            if (x == 7)
              line += " ";
          }

          ImGui::SameLine();
          ImGui::TextUnformatted(line.c_str());
        }
      clipper.End();
    }
  }
  ImGui::EndChild();
  ImGui::Separator();
}

bool Output::Render() {
  ImGui::PushID(this);
  bool modified = false;

  if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {

    ImGui::TextUnformatted("Format: ");
    ImGui::SameLine();
    if (ImGui::Combo("##ui_output_format", (int *)&format,
                     "char\0hex\0hexdump\0decimal\0java\0c\0\0")) {
      modified = true;
    }

    ImGuiTextFlags flags = ImGuiInputTextFlags_ReadOnly;
    bool notext = false;

    switch (format) {
    case OutputFormat::CHAR:
      text = bytes;
      break;
    case OutputFormat::HEX:
      text = utils::hexEncode(bytes);
      break;
    case OutputFormat::HEXDUMP:
      HexViewer(bytes, ImGui::GetTextLineHeight() * 8);
      notext = true;
      break;
    case OutputFormat::DECIMAL:
      text = bytes;
      break;
    case OutputFormat::JAVA:
      text = bytes;
      break;
    case OutputFormat::C:
      text = bytes;
      break;
    }

    if (!notext) {
      if (ImGui::InputTextMultiline(
              "##ui_output", &text,
              ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8), flags)) {
      }
    }

    ImGui::PopID();
  }

  return modified;
}

bool Input::Render() {
  ImGui::PushID(this);
  bool modified = false;

  if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {

    ImGui::TextUnformatted("Format: ");
    ImGui::SameLine();
    if (ImGui::Combo("##ui_input_format", (int *)&format,
                     "char\0hex\0decimal\0\0")) {
      modified = true;
    }

    ImGuiTextFlags flags = 0;
    switch (format) {
    case InputFormat::CHAR:
      break;
    case InputFormat::HEX:
      flags |= ImGuiInputTextFlags_CharsHexadecimal;
      break;
    case InputFormat::DECIMAL:
      flags |= ImGuiInputTextFlags_CharsDecimal;
      break;
    }

    if (ImGui::InputTextMultiline(
            "##ui_input", &text,
            ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * height), flags)) {
      modified = true;
    }

    if (modified) {
      switch (format) {
      case InputFormat::CHAR:
        bytes = text;
        break;
      case InputFormat::HEX:
        bytes = utils::hexDecode(text);
        break;
      case InputFormat::DECIMAL:
        bytes = text;
        break;
      }
    }

    ImGui::Text("Total Size: %d bytes", (int)bytes.size());

    if (ImGui::TreeNode("Preview:")) {
      HexViewer(bytes, 200);
      ImGui::TreePop();
    }
  }

  ImGui::PopID();

  return modified;
}
