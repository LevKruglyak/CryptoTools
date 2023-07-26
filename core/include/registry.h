#pragma once

#include "graph.h"
#include "hello_imgui/hello_imgui.h"
#include <map>
#include <vector>

/// ---------------------------------------------------------------------------
/// Boilerplate macros
/// ---------------------------------------------------------------------------
#define CONCAT(x, y) __CONCAT(x, y)
#define MAKE_UNIQUE_ID(x) CONCAT(x, __COUNTER__)

/// ---------------------------------------------------------------------------
/// Reflected data
/// ---------------------------------------------------------------------------
inline std::vector<std::function<HelloImGui::DockableWindow()>>
    REFLECTED_WINDOWS;

struct ReflectedNodeInfo {
  const char *name;
  std::function<cptl::NodePtr()> create;
};

inline std::map<std::string, std::vector<ReflectedNodeInfo>> REFLECTED_NODES;

/// ---------------------------------------------------------------------------
/// Helper template
/// ---------------------------------------------------------------------------

template <typename NODE> class NodeReflectionHelper {
public:
  NodeReflectionHelper<NODE>(std::string group) {
    const char *name = NODE::StaticNodeName();
    std::function<cptl::NodePtr()> function = [] {
      cptl::NodePtr ptr = std::make_shared<NODE>();
      return ptr;
    };

    REFLECTED_NODES[group].push_back({name, function});
    std::cout << "[REGISTRY] Loaded node: " << NODE::StaticNodeName()
              << std::endl;
  }
};

/// ---------------------------------------------------------------------------
/// Macros
/// ---------------------------------------------------------------------------

#define DEFINE_WINDOW(name)                                                    \
  HelloImGui::DockableWindow CreateWindow_##name();                            \
  class __##name##_window_t_ {                                                 \
  public:                                                                      \
    __##name##_window_t_() {                                                   \
      REFLECTED_WINDOWS.push_back(CreateWindow_##name);                        \
      std::cout << "[REGISTRY] Loaded window: " << #name << std::endl;         \
    }                                                                          \
  };                                                                           \
  inline __##name##_window_t_ MAKE_UNIQUE_ID(_trans_window__);

#define DEFINE_NODE(type, group)                                               \
  inline NodeReflectionHelper<type> MAKE_UNIQUE_ID(_trans_node__)(#group);

// #define DEFINE_NODE(type)                                                      \
//   inline NodeReflectionHelper<type> MAKE_UNIQUE_ID(_trans_node__);
