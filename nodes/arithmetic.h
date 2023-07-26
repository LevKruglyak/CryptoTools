#include "graph.h"
#include "nbtheory.h"
#include "registry.h"

using namespace cptl;

class AddIntegerNode : public Node {
  InPinID in1;
  InPinID in2;
  OutPinID out;

public:
  NODE_STATIC_NAME("Add");
  AddIntegerNode() : Node(StaticNodeName()) {
    in1 = AddInputPin("a", DataType::INTEGER);
    in2 = AddInputPin("b", DataType::INTEGER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result = GetInInteger(in1) + GetInInteger(in2);
    SetOutInteger(out, result);
  }
};

class SubtractIntegerNode : public Node {
  InPinID in1;
  InPinID in2;
  OutPinID out;

public:
  NODE_STATIC_NAME("Subtract");
  SubtractIntegerNode() : Node(StaticNodeName()) {
    in1 = AddInputPin("a", DataType::INTEGER);
    in2 = AddInputPin("b", DataType::INTEGER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result = GetInInteger(in1) - GetInInteger(in2);
    SetOutInteger(out, result);
  }
};

class MultiplyIntegerNode : public Node {
  InPinID in1;
  InPinID in2;
  OutPinID out;

public:
  NODE_STATIC_NAME("Multiply");
  MultiplyIntegerNode() : Node(StaticNodeName()) {
    in1 = AddInputPin("a", DataType::INTEGER);
    in2 = AddInputPin("b", DataType::INTEGER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result = GetInInteger(in1) * GetInInteger(in2);
    SetOutInteger(out, result);
  }
};

class DivideIntegerNode : public Node {
  InPinID in1;
  InPinID in2;
  OutPinID out;

public:
  NODE_STATIC_NAME("Divide");
  DivideIntegerNode() : Node(StaticNodeName()) {
    in1 = AddInputPin("a", DataType::INTEGER);
    in2 = AddInputPin("b", DataType::INTEGER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result = GetInInteger(in1) / GetInInteger(in2);
    SetOutInteger(out, result);
  }
};

class ModulusIntegerNode : public Node {
  InPinID in1;
  InPinID in2;
  OutPinID out;

public:
  NODE_STATIC_NAME("Modulus");
  ModulusIntegerNode() : Node(StaticNodeName()) {
    in1 = AddInputPin("a", DataType::INTEGER);
    in2 = AddInputPin("b", DataType::INTEGER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result = GetInInteger(in1) % GetInInteger(in2);
    SetOutInteger(out, result);
  }
};

class ORIntegerNode : public Node {
  InPinID in1;
  InPinID in2;
  OutPinID out;

public:
  NODE_STATIC_NAME("OR");
  ORIntegerNode() : Node(StaticNodeName()) {
    in1 = AddInputPin("a", DataType::INTEGER);
    in2 = AddInputPin("b", DataType::INTEGER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result = GetInInteger(in1) | GetInInteger(in2);
    SetOutInteger(out, result);
  }
};

class ANDIntegerNode : public Node {
  InPinID in1;
  InPinID in2;
  OutPinID out;

public:
  NODE_STATIC_NAME("AND");
  ANDIntegerNode() : Node(StaticNodeName()) {
    in1 = AddInputPin("a", DataType::INTEGER);
    in2 = AddInputPin("b", DataType::INTEGER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result = GetInInteger(in1) & GetInInteger(in2);
    SetOutInteger(out, result);
  }
};

class XORIntegerNode : public Node {
  InPinID in1;
  InPinID in2;
  OutPinID out;

public:
  NODE_STATIC_NAME("XOR");
  XORIntegerNode() : Node(StaticNodeName()) {
    in1 = AddInputPin("a", DataType::INTEGER);
    in2 = AddInputPin("b", DataType::INTEGER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result = GetInInteger(in1) ^ GetInInteger(in2);
    SetOutInteger(out, result);
  }
};

class NOTIntegerNode : public Node {
  InPinID in1;
  OutPinID out;

public:
  NODE_STATIC_NAME("NOT");
  NOTIntegerNode() : Node(StaticNodeName()) {
    in1 = AddInputPin("in", DataType::INTEGER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result = !GetInInteger(in1);
    SetOutInteger(out, result);
  }
};

class GCDIntegerNode : public Node {
  InPinID in1;
  InPinID in2;
  OutPinID out;

public:
  NODE_STATIC_NAME("GCD");
  GCDIntegerNode() : Node(StaticNodeName()) {
    in1 = AddInputPin("a", DataType::INTEGER);
    in2 = AddInputPin("b", DataType::INTEGER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result =
        CryptoPP::GCD(GetInInteger(in1), GetInInteger(in2));
    SetOutInteger(out, result);
  }
};

class LCMIntegerNode : public Node {
  InPinID in1;
  InPinID in2;
  OutPinID out;

public:
  NODE_STATIC_NAME("LCM");
  LCMIntegerNode() : Node(StaticNodeName()) {
    in1 = AddInputPin("a", DataType::INTEGER);
    in2 = AddInputPin("b", DataType::INTEGER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result =
        CryptoPP::LCM(GetInInteger(in1), GetInInteger(in2));
    SetOutInteger(out, result);
  }
};

class ModularMultiplicationNode : public Node {
  InPinID in1;
  InPinID in2;
  InPinID in3;
  OutPinID out;

public:
  NODE_STATIC_NAME("Modular Multiplication");
  ModularMultiplicationNode() : Node(StaticNodeName()) {
    in1 = AddInputPin("a", DataType::INTEGER);
    in2 = AddInputPin("b", DataType::INTEGER);
    in3 = AddInputPin("modulus", DataType::INTEGER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result = CryptoPP::ModularMultiplication(
        GetInInteger(in1), GetInInteger(in2), GetInInteger(in3));
    SetOutInteger(out, result);
  }
};

class ModularExponentiationNode : public Node {
  InPinID in1;
  InPinID in2;
  InPinID in3;
  OutPinID out;

public:
  NODE_STATIC_NAME("Modular Exponent");
  ModularExponentiationNode() : Node(StaticNodeName()) {
    in1 = AddInputPin("in", DataType::INTEGER);
    in2 = AddInputPin("exponent", DataType::INTEGER);
    in3 = AddInputPin("modulus", DataType::INTEGER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result = CryptoPP::ModularExponentiation(
        GetInInteger(in1), GetInInteger(in2), GetInInteger(in3));
    SetOutInteger(out, result);
  }
};

class ModularSquareRootNode : public Node {
  InPinID in1;
  InPinID in2;
  OutPinID out;

public:
  NODE_STATIC_NAME("Modular Square Root");
  ModularSquareRootNode() : Node(StaticNodeName()) {
    in1 = AddInputPin("in", DataType::INTEGER);
    in2 = AddInputPin("modulus", DataType::INTEGER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result =
        CryptoPP::ModularSquareRoot(GetInInteger(in1), GetInInteger(in2));
    SetOutInteger(out, result);
  }
};

class ModularInverseNode : public Node {
  InPinID in1;
  InPinID in2;
  OutPinID out;

public:
  NODE_STATIC_NAME("Modular Inverse");
  ModularInverseNode() : Node(StaticNodeName()) {
    in1 = AddInputPin("in", DataType::INTEGER);
    in2 = AddInputPin("modulus", DataType::INTEGER);
    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result = CryptoPP::EuclideanMultiplicativeInverse(
        GetInInteger(in1), GetInInteger(in2));
    SetOutInteger(out, result);
  }
};

class CRTIntegerNode : public Node {
  InPinID xp;
  InPinID p;
  InPinID xq;
  InPinID q;
  OutPinID out;

public:
  NODE_STATIC_NAME("Chinese Remainder Theorem");
  CRTIntegerNode() : Node(StaticNodeName(), 160.0f) {
    xp = AddInputPin("x mod p", DataType::INTEGER);
    p = AddInputPin("p", DataType::INTEGER);
    xq = AddInputPin("x mod q", DataType::INTEGER);
    q = AddInputPin("q", DataType::INTEGER);

    out = AddOutputPin("x", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result = CryptoPP::CRT(
        GetInInteger(xp), GetInInteger(p), GetInInteger(xq), GetInInteger(q),
        CryptoPP::EuclideanMultiplicativeInverse(GetInInteger(p),
                                                 GetInInteger(q)));
    SetOutInteger(out, result);
  }
};

class JacobiSymbolNode : public Node {
  InPinID a;
  InPinID b;
  OutPinID out;

public:
  NODE_STATIC_NAME("Jacobi Symbol");
  JacobiSymbolNode() : Node(StaticNodeName()) {
    a = AddInputPin("a", DataType::INTEGER);
    b = AddInputPin("b", DataType::INTEGER);

    out = AddOutputPin("out", DataType::INTEGER);
  }

  bool DisplayInternal() override { return false; }

  void RecalculateInternal() override {
    CryptoPP::Integer result =
        CryptoPP::Jacobi(GetInInteger(a), GetInInteger(b));
    SetOutInteger(out, result);
  }
};

DEFINE_NODE(AddIntegerNode, Arithmetic);
DEFINE_NODE(SubtractIntegerNode, Arithmetic);
DEFINE_NODE(MultiplyIntegerNode, Arithmetic);
DEFINE_NODE(DivideIntegerNode, Arithmetic);
DEFINE_NODE(ModulusIntegerNode, Arithmetic);
DEFINE_NODE(ORIntegerNode, Arithmetic);
DEFINE_NODE(ANDIntegerNode, Arithmetic);
DEFINE_NODE(XORIntegerNode, Arithmetic);
DEFINE_NODE(NOTIntegerNode, Arithmetic);
DEFINE_NODE(ModularMultiplicationNode, Arithmetic);
DEFINE_NODE(ModularExponentiationNode, Arithmetic);
DEFINE_NODE(ModularSquareRootNode, Arithmetic);
DEFINE_NODE(ModularInverseNode, Arithmetic);
DEFINE_NODE(CRTIntegerNode, Arithmetic);
DEFINE_NODE(JacobiSymbolNode, Arithmetic);
