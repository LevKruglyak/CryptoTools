#pragma once

#include "main.h"
#include "registry.h"

class TemplateNode : public cptl::Node {
public:
  NODE_STATIC_NAME("TemplateNode");
  TemplateNode() : Node(StaticNodeName()) {}

  bool DisplayInternal() override { return false; }
  void RecalculateInternal() override {}
};

DEFINE_NODE(TemplateNode, Misc);
