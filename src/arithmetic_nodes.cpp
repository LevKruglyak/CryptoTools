#include "graph.h"
#include "nbtheory.h"
#include "nodes.h"

class AddIntegerNode : public Node {
  int in1;
  int in2;
  int out;

public:
  AddIntegerNode() : Node("Add Integer") {
    in1 = AddInput("first", LinkType::INTEGER);
    in2 = AddInput("second", LinkType::INTEGER);
    out = AddOutput("out", LinkType::INTEGER);
  }

  void DisplayInternal() override {}

  void ProcessInternal(Graph &graph) override {
    CryptoPP::Integer result =
        GetInInteger(graph, in1) + GetInInteger(graph, in2);
    SetOutLinkInteger(graph, out, result);
  }
};

class SubtractIntegerNode : public Node {
  int in1;
  int in2;
  int out;

public:
  SubtractIntegerNode() : Node("Subtract Integer") {
    in1 = AddInput("first", LinkType::INTEGER);
    in2 = AddInput("second", LinkType::INTEGER);
    out = AddOutput("out", LinkType::INTEGER);
  }

  void DisplayInternal() override {}

  void ProcessInternal(Graph &graph) override {
    CryptoPP::Integer result =
        GetInInteger(graph, in1) - GetInInteger(graph, in2);
    SetOutLinkInteger(graph, out, result);
  }
};

class MultiplyIntegerNode : public Node {
  int in1;
  int in2;
  int out;

public:
  MultiplyIntegerNode() : Node("Multiply Integer") {
    in1 = AddInput("first", LinkType::INTEGER);
    in2 = AddInput("second", LinkType::INTEGER);
    out = AddOutput("out", LinkType::INTEGER);
  }

  void DisplayInternal() override {}

  void ProcessInternal(Graph &graph) override {
    CryptoPP::Integer result =
        GetInInteger(graph, in1) * GetInInteger(graph, in2);
    SetOutLinkInteger(graph, out, result);
  }
};

class DivideIntegerNode : public Node {
  int in1;
  int in2;
  int out;

public:
  DivideIntegerNode() : Node("Divide Integer") {
    in1 = AddInput("first", LinkType::INTEGER);
    in2 = AddInput("second", LinkType::INTEGER);
    out = AddOutput("out", LinkType::INTEGER);
  }

  void DisplayInternal() override {}

  void ProcessInternal(Graph &graph) override {
    CryptoPP::Integer result =
        GetInInteger(graph, in1) / GetInInteger(graph, in2);
    SetOutLinkInteger(graph, out, result);
  }
};

class ModulusIntegerNode : public Node {
  int in1;
  int in2;
  int out;

public:
  ModulusIntegerNode() : Node("Modulus Integer") {
    in1 = AddInput("first", LinkType::INTEGER);
    in2 = AddInput("second", LinkType::INTEGER);
    out = AddOutput("out", LinkType::INTEGER);
  }

  void DisplayInternal() override {}

  void ProcessInternal(Graph &graph) override {
    CryptoPP::Integer result =
        GetInInteger(graph, in1) % GetInInteger(graph, in2);
    SetOutLinkInteger(graph, out, result);
  }
};

class ModularExponentNode : public Node {
  int in1;
  int in2;
  int in3;
  int out;

public:
  ModularExponentNode() : Node("Modular Exponent") {
    in1 = AddInput("in", LinkType::INTEGER);
    in2 = AddInput("exponent", LinkType::INTEGER);
    in3 = AddInput("modulus", LinkType::INTEGER);
    out = AddOutput("out", LinkType::INTEGER);
  }

  void DisplayInternal() override {}

  void ProcessInternal(Graph &graph) override {
    CryptoPP::Integer result = CryptoPP::ModularExponentiation(
        GetInInteger(graph, in1), GetInInteger(graph, in2),
        GetInInteger(graph, in3));
    SetOutLinkInteger(graph, out, result);
  }
};

class ModularInverseNode : public Node {
  int in1;
  int in2;
  int out;

public:
  ModularInverseNode() : Node("Modular Inverse") {
    in1 = AddInput("in", LinkType::INTEGER);
    in2 = AddInput("modulus", LinkType::INTEGER);
    out = AddOutput("out", LinkType::INTEGER);
  }

  void DisplayInternal() override {}

  void ProcessInternal(Graph &graph) override {
    CryptoPP::Integer result = CryptoPP::EuclideanMultiplicativeInverse(
        GetInInteger(graph, in1), GetInInteger(graph, in2));
    SetOutLinkInteger(graph, out, result);
  }
};

class GcdNode : public Node {
  int in1;
  int in2;
  int out;

public:
  GcdNode() : Node("GCD") {
    in1 = AddInput("first", LinkType::INTEGER);
    in2 = AddInput("second", LinkType::INTEGER);
    out = AddOutput("out", LinkType::INTEGER);
  }

  void DisplayInternal() override {}

  void ProcessInternal(Graph &graph) override {
    CryptoPP::Integer result =
        CryptoPP::GCD(GetInInteger(graph, in1), GetInInteger(graph, in2));
    SetOutLinkInteger(graph, out, result);
  }
};

class LcmNode : public Node {
  int in1;
  int in2;
  int out;

public:
  LcmNode() : Node("LCM") {
    in1 = AddInput("first", LinkType::INTEGER);
    in2 = AddInput("second", LinkType::INTEGER);
    out = AddOutput("out", LinkType::INTEGER);
  }

  void DisplayInternal() override {}

  void ProcessInternal(Graph &graph) override {
    CryptoPP::Integer result =
        CryptoPP::LCM(GetInInteger(graph, in1), GetInInteger(graph, in2));
    SetOutLinkInteger(graph, out, result);
  }
};

void ShowArithmeticNodesMenu(Graph &graph) {
  if (ImGui::BeginMenu("Arithmetic")) {
    CreateButton<AddIntegerNode>(graph, "Add");
    CreateButton<SubtractIntegerNode>(graph, "Subtract");
    CreateButton<MultiplyIntegerNode>(graph, "Multiply");
    CreateButton<DivideIntegerNode>(graph, "Divide");
    CreateButton<ModulusIntegerNode>(graph, "Modulus");
    CreateButton<ModularExponentNode>(graph, "Modular Exponent");
    CreateButton<ModularInverseNode>(graph, "Modular Inverse");
    CreateButton<GcdNode>(graph, "GCD");
    CreateButton<LcmNode>(graph, "LCM");
    ImGui::EndMenu();
  }
}
