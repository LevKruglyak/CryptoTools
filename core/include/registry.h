#include "hello_imgui/hello_imgui.h"
#include <vector>

inline std::vector<std::function<HelloImGui::DockableWindow()>> CREATE_WINDOWS;

/// Macro hackery to provide compile-time reflection of dockable windows
#define DEFINE_WINDOW(name)                                                    \
  HelloImGui::DockableWindow CreateWindow_##name();                            \
  class __##name##_t_ {                                                        \
  public:                                                                      \
    __##name##_t_() {                                                          \
      CREATE_WINDOWS.push_back(CreateWindow_##name);                           \
      std::cout << "[REGISTRY] Loaded window: " << #name << std::endl;         \
    }                                                                          \
  };                                                                           \
  inline __##name##_t_ __##name##_instance_t_;\
