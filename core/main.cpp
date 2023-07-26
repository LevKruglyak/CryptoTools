#include "main.h"
#include "docking_params.h"
#include "hello_imgui.h"
#include "imgui.h"
#include "imgui_default_settings.h"
#include "registry.h"

// // Registry
// #include "nodes.h"
// #include "windows.h"

#include <iostream>
#include <sstream>

struct CryptoToolsSettings {
  std::string name = "CryptoTools";
};

struct AppState {
  CryptoToolsSettings settings;
};

//////////////////////////////////////////////////////////////////////////
//    Additional fonts handling
//////////////////////////////////////////////////////////////////////////
ImFont *gTitleFont;
ImFont *codeFont;

void LoadFonts() // This is called by runnerParams.callbacks.LoadAdditionalFonts
{
  HelloImGui::ImGuiDefaultSettings::LoadDefaultFont_WithFontAwesomeIcons();
  gTitleFont = HelloImGui::LoadFontTTF("fonts/DroidSans.ttf", 18.f);
}

//////////////////////////////////////////////////////////////////////////
//    Save additional settings in the ini file
//////////////////////////////////////////////////////////////////////////
std::string SerializeSettings(const CryptoToolsSettings &myAppSettings) {
  std::stringstream ss;
  ss << myAppSettings.name << "\n";
  return ss.str();
}

CryptoToolsSettings DeserializeSettings(const std::string &s) {
  std::stringstream ss(s);
  CryptoToolsSettings myAppSettings;
  ss >> myAppSettings.name;
  return myAppSettings;
}

void LoadCryptoToolsAppState(AppState &appState) //
{
  appState.settings = DeserializeSettings(HelloImGui::LoadUserPref("UserInfo"));
}

void SaveCryptoToolsAppState(const AppState &appState) {
  HelloImGui::SaveUserPref("UserInfo", SerializeSettings(appState.settings));
}

std::vector<HelloImGui::DockableWindow> addQueue;
std::vector<HelloImGui::DockableWindow> removeQueue;

void AddDockableWindow(HelloImGui::DockableWindow window) {
  addQueue.push_back(window);
}

void PreNewFrame(void) {
  for (auto window : addQueue) {
    HelloImGui::GetRunnerParams()->dockingParams.dockableWindows.push_back(
        window);
  }

  addQueue.clear();
}

//////////////////////////////////////////////////////////////////////////
//    Gui functions used in this demo
//////////////////////////////////////////////////////////////////////////

void showWindow(std::string name) {
  auto windowPtr =
      HelloImGui::GetRunnerParams()->dockingParams.dockableWindowOfName(name);
  if (windowPtr) {
    windowPtr->isVisible = true;
  }
}

// The Gui of the status bar
void StatusBarGui(AppState &app_state) {}

void ShowMenuGui() {
  if (ImGui::BeginMenu("About")) {
    ImGui::EndMenu();
  }
}

void ShowAppMenuItems() {
  if (ImGui::MenuItem("Custom Item")) {
    HelloImGui::DockableWindow window;
    window.label = "Sample Window";
    window.dockSpaceName = DOCK_AREA;
    window.GuiFunction = [] { ImGui::Text("hello, wrold"); };
    window.focusWindowAtNextFrame = true;
    window.isVisible = true;

    AddDockableWindow(window);
  }
}

//////////////////////////////////////////////////////////////////////////
//    Docking Layouts and Docking windows
//////////////////////////////////////////////////////////////////////////

//
// 1. Define the Docking splits (two versions are available)
//
std::vector<HelloImGui::DockingSplit> CreateDefaultDockingSplits() {
  HelloImGui::DockingSplit splitMainMisc;
  splitMainMisc.initialDock = "MainDockSpace";
  splitMainMisc.newDock = DOCK_AREA;
  splitMainMisc.direction = ImGuiDir_Down;
  splitMainMisc.ratio = 0.25f;

  std::vector<HelloImGui::DockingSplit> splits{splitMainMisc};
  return splits;
}

std::vector<HelloImGui::DockableWindow>
CreateDockableWindows(AppState &appState) {
  std::vector<HelloImGui::DockableWindow> dockableWindows;

  for (auto create_window : CREATE_WINDOWS) {
    dockableWindows.push_back(create_window());
  }

  return dockableWindows;
};

HelloImGui::DockingParams CreateDefaultLayout(AppState &appState) {
  HelloImGui::DockingParams dockingParams;
  dockingParams.layoutName = "Default";
  dockingParams.dockingSplits = CreateDefaultDockingSplits();
  dockingParams.dockableWindows = CreateDockableWindows(appState);
  return dockingParams;
}

std::vector<HelloImGui::DockingParams>
CreateAlternativeLayouts(AppState &appState) {
  return {};
};

int main(int, char **) {
  // ###############################################################################################
  //  Part 1: Define the application state, fill the status and menu bars, and
  //  load additional font
  // ###############################################################################################

  AppState appState;
  HelloImGui::RunnerParams runnerParams;

  runnerParams.appWindowParams.windowTitle = "CryptoTools";
  runnerParams.imGuiWindowParams.menuAppTitle = "CryptoTools";
  runnerParams.appWindowParams.windowGeometry.size = {1920, 1080};
  runnerParams.appWindowParams.restorePreviousGeometry = true;
  runnerParams.callbacks.LoadAdditionalFonts = LoadFonts;
  // runnerParams.imGuiWindowParams.showStatusBar = false;
  runnerParams.imGuiWindowParams.showStatusBar = true;
  runnerParams.imGuiWindowParams.showStatus_Fps = false;
  runnerParams.callbacks.ShowStatus = [&appState]() { StatusBarGui(appState); };
  runnerParams.imGuiWindowParams.showMenuBar = true;
  runnerParams.callbacks.ShowMenus = ShowMenuGui;
  runnerParams.callbacks.ShowAppMenuItems = ShowAppMenuItems;

  runnerParams.callbacks.PostInit = [&appState] {
    LoadCryptoToolsAppState(appState);
  };
  runnerParams.callbacks.BeforeExit = [&appState] {
    SaveCryptoToolsAppState(appState);
  };

  // Dynamic windows
  runnerParams.callbacks.PreNewFrame = PreNewFrame;

  // ###############################################################################################
  //  Part 2: Define the application layout and windows
  // ###############################################################################################

  runnerParams.imGuiWindowParams.defaultImGuiWindowType =
      HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;
  runnerParams.imGuiWindowParams.enableViewports = true;
  runnerParams.dockingParams = CreateDefaultLayout(appState);
  runnerParams.alternativeDockingLayouts = CreateAlternativeLayouts(appState);

  // ###############################################################################################
  //  Part 3: Run the app
  // ###############################################################################################
  HelloImGui::Run(runnerParams);

  return 0;
}
