#include "imgui.h"
#include "ui.h"
#include <iostream>

Input a("A", 4);
Input b("B", 4);
Input m("Modulus", 4);

Output out("Result");

void ArithmeticFunction() {
  bool modified = false;

  static int operationId = 0;
  if (ImGui::CollapsingHeader("Operation", ImGuiTreeNodeFlags_DefaultOpen)) {
    const char *operations[] = {"Add"};

    if (ImGui::Combo("Algorithm", &operationId, operations, 16)) {
      modified = true;
    }
  }

  modified |= a.Render();
  modified |= b.Render();
  modified |= m.Render();

  out.Render();
}

HelloImGui::DockableWindow createArithmeticWindow() {
  HelloImGui::DockableWindow window;
  window.label = "Arithmetic";
  window.dockSpaceName = dockArea;
  window.isVisible = false;
  window.GuiFunction = ArithmeticFunction;
  return window;
}
