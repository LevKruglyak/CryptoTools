#include "ui.h"
#include "imgui.h"
#include "utils.h"

Input::Input() {}

Output::Output() {}

void Output::Render() {
  bool modified = false;

  if (ImGui::Combo("Output Format", (int *)&format,
                   "char\0hex\0hexdump\0decimal\0java\0c\0\0")) {
    modified = true;
  }

  ImGuiTextFlags flags = ImGuiInputTextFlags_ReadOnly;
  switch (format) {
  case OutputFormat::CHAR:
    text = bytes;
    break;
  case OutputFormat::HEX:
    text = hexEncode(bytes);
    break;
  case OutputFormat::HEXDUMP:
    text = hexdump(bytes);
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

  if (ImGui::InputTextMultiline(
          "_", &text, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16),
          flags)) {
  }
}

bool Input::Render() {
  bool modified = false;

  if (ImGui::Combo("Input Format", (int *)&format, "char\0hex\0decimal\0\0")) {
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
          "", &text, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16),
          flags)) {
    modified = true;
  }

  if (modified) {
    switch (format) {
    case InputFormat::CHAR:
      bytes = text;
      break;
    case InputFormat::HEX:
      bytes = hexDecode(text);
      break;
    case InputFormat::DECIMAL:
      bytes = text;
      break;
    }
  }

  if (ImGui::TreeNode("Preview:")) {
    std::string hexPreview = hexdump(bytes);
    ImGui::TextUnformatted(hexPreview.c_str());
    ImGui::TreePop();
  }

  ImGui::Text("Total Size: %d bytes", (int)bytes.size());

  return modified;
}
