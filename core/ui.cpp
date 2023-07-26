#include "ui.h"
#include "imgui.h"
#include "utils.h"
#include <iostream>

void ImGui::HexViewer(std::string &data) {
  static const std::string Header =
      " Address      00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F "
      " ";
  static const std::string NoData =
      "                                                      No Data! "
      " ";

  ImGui::TextDisabled("%s", Header.c_str());
  if (data.size() == 0) {
    ImGui::TextUnformatted(NoData.c_str());
  } else {
    size_t size = data.size();
    ImGuiListClipper clipper;

    clipper.Begin((size + 0x0F) / 0x10);

    while (clipper.Step())
      for (auto y = clipper.DisplayStart; y < clipper.DisplayEnd; y++) {
        auto lineSize = ((size - y * 0x10) < 0x10) ? size % 0x10 : 0x10;

        ImGui::TextUnformatted(cptl::format(" {:07X}:  ", y * 0x10).c_str());
        std::string line;

        for (unsigned int x = 0; x < 0x10; x++) {
          if (x < lineSize)
            line += cptl::format("{:02X} ", data.data()[y * 0x10 + x]);
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
