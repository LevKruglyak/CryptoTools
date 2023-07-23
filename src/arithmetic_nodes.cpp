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
    std::shared_ptr<Link> in1_link = GetInLink(graph, in1);
    std::shared_ptr<Link> in2_link = GetInLink(graph, in2);
    if (in1_link != nullptr && in2_link != nullptr) {
      CryptoPP::Integer first = in1_link->getInteger();
      CryptoPP::Integer second = in2_link->getInteger();
      SetOutLinkInteger(graph, out, first + second);
    }
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
    std::shared_ptr<Link> in1_link = GetInLink(graph, in1);
    std::shared_ptr<Link> in2_link = GetInLink(graph, in2);
    if (in1_link != nullptr && in2_link != nullptr) {
      CryptoPP::Integer first = in1_link->getInteger();
      CryptoPP::Integer second = in2_link->getInteger();
      SetOutLinkInteger(graph, out, first - second);
    }
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
    std::shared_ptr<Link> in1_link = GetInLink(graph, in1);
    std::shared_ptr<Link> in2_link = GetInLink(graph, in2);
    if (in1_link != nullptr && in2_link != nullptr) {
      CryptoPP::Integer first = in1_link->getInteger();
      CryptoPP::Integer second = in2_link->getInteger();
      SetOutLinkInteger(graph, out, first * second);
    }
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
    std::shared_ptr<Link> in1_link = GetInLink(graph, in1);
    std::shared_ptr<Link> in2_link = GetInLink(graph, in2);
    if (in1_link != nullptr && in2_link != nullptr) {
      CryptoPP::Integer first = in1_link->getInteger();
      CryptoPP::Integer second = in2_link->getInteger();
      SetOutLinkInteger(graph, out, first / second);
    }
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
    std::shared_ptr<Link> in1_link = GetInLink(graph, in1);
    std::shared_ptr<Link> in2_link = GetInLink(graph, in2);
    if (in1_link != nullptr && in2_link != nullptr) {
      CryptoPP::Integer first = in1_link->getInteger();
      CryptoPP::Integer second = in2_link->getInteger();
      SetOutLinkInteger(graph, out, first % second);
    }
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
    std::shared_ptr<Link> in1_link = GetInLink(graph, in1);
    std::shared_ptr<Link> in2_link = GetInLink(graph, in2);
    std::shared_ptr<Link> in3_link = GetInLink(graph, in3);
    if (in1_link != nullptr && in2_link != nullptr && in3_link != nullptr) {
      CryptoPP::Integer x = in1_link->getInteger();
      CryptoPP::Integer exp = in2_link->getInteger();
      CryptoPP::Integer mod = in3_link->getInteger();
      SetOutLinkInteger(graph, out,
                        CryptoPP::ModularExponentiation(x, exp, mod));
    }
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
    std::shared_ptr<Link> in1_link = GetInLink(graph, in1);
    std::shared_ptr<Link> in2_link = GetInLink(graph, in2);
    if (in1_link != nullptr && in2_link != nullptr) {
      CryptoPP::Integer a = in1_link->getInteger();
      CryptoPP::Integer b = in2_link->getInteger();
      SetOutLinkInteger(graph, out,
                        CryptoPP::EuclideanMultiplicativeInverse(a, b));
    }
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
    std::shared_ptr<Link> in1_link = GetInLink(graph, in1);
    std::shared_ptr<Link> in2_link = GetInLink(graph, in2);
    if (in1_link != nullptr && in2_link != nullptr) {
      CryptoPP::Integer a = in1_link->getInteger();
      CryptoPP::Integer b = in2_link->getInteger();
      SetOutLinkInteger(graph, out, CryptoPP::GCD(a, b));
    }
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
    std::shared_ptr<Link> in1_link = GetInLink(graph, in1);
    std::shared_ptr<Link> in2_link = GetInLink(graph, in2);
    if (in1_link != nullptr && in2_link != nullptr) {
      CryptoPP::Integer a = in1_link->getInteger();
      CryptoPP::Integer b = in2_link->getInteger();
      SetOutLinkInteger(graph, out, CryptoPP::LCM(a, b));
    }
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
