#include "graph.h"
#include "registry.h"

class Template : public cptl::Node {
public:
  NODE_STATIC_NAME("Template");
  Template() : Node(StaticNodeName()) {}

  bool DisplayInternal() override { return false; }
  void RecalculateInternal() override {}
};

DEFINE_NODE(Template, Miscellaneous)
