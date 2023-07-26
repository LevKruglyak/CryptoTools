#pragma once

#include "cryptlib.h"
#include "filters.h"
#include "imgui.h"
#include "imnodes.h"
#include "integer.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cryptopp/nbtheory.h>
#include <exception>
#include <map>
#include <optional>
#include <set>
#include <variant>
#include <vector>

namespace cptl {

class Node;
class Link;
class InPin;
class OutPin;
class Graph;

/// shared_ptr to a Node
typedef std::shared_ptr<Node> NodePtr;
/// shared_ptr to a Link
typedef std::shared_ptr<Link> LinkPtr;
/// shared_ptr to an input pin
typedef std::shared_ptr<InPin> InPinPtr;
/// shared_ptr to an output pin
typedef std::shared_ptr<OutPin> OutPinPtr;

/// Generate a globally unique ID
int GenerateID();

/// Utility class for extra type safety when
/// dealing with IDs
template <typename Node> class ID {
  int id;

public:
  ID() { id = GenerateID(); }
  ID(const ID &_id) { id = _id.id; }
  ID(int id) : id(id) {}

  friend bool operator==(const ID &lhs, const ID &rhs) {
    return lhs.id == rhs.id;
  }

  operator int &() { return id; }
  operator int() const { return id; }
};

/// ID type for a Node
typedef ID<Node> NodeID;
/// ID type for a Link
typedef ID<Link> LinkID;
/// ID type for an input pin
typedef ID<InPin> InPinID;
/// ID type for an output pin
typedef ID<OutPin> OutPinID;

/// Simple base class for ImNodes components
/// containing an internal id (e.g. Node, Link, Pin, etc)
///
/// Generic over ID type for extra type safety
template <typename ID> class Component {
  ID id;

public:
  Component() {}

  /// Get the ID of the component
  const ID GetID() const { return id; }
};

enum DataType {
  BUFFER,
  INTEGER,
};

inline constexpr ImNodesPinShape DataTypeToShape(DataType type) {
  switch (type) {
  case DataType::BUFFER:
    return ImNodesPinShape_QuadFilled;
  case DataType::INTEGER:
    return ImNodesPinShape_TriangleFilled;
  }
}

class Buffer {
  std::string data;
  std::shared_ptr<CryptoPP::StringSink> sink;

  static Buffer empty_buffer;

public:
  Buffer() { sink = std::make_shared<CryptoPP::StringSink>(data); }

  Buffer(const Buffer &buffer) {
    data = buffer.data;
    sink = std::make_shared<CryptoPP::StringSink>(data);
  }

  /// Return a constant pointer to a `char` array
  const char *const_char_ptr() const { return data.c_str(); }

  /// Return a constant pointer to a `byte` array
  const CryptoPP::byte *const_byte_ptr() const {
    return (const CryptoPP::byte *)data.c_str();
  }

  /// Return a reference to the internal `StringSink` for compatibility with the
  /// CryptoPP pipelining system
  CryptoPP::BufferedTransformation &sink_ref() { return sink->Ref(); }

  /// Return the length of the buffer in bytes
  size_t length() const { return data.length(); }

  /// Return a const reference to the empty buffer
  static const Buffer &Empty() { return empty_buffer; }

  operator std::string &() { return data; }
};

class Integer {
  CryptoPP::Integer data = CryptoPP::Integer::Zero();

  static Integer zero;

public:
  Integer() {}

  /// Overwrite the content of the integer with the parsed string
  void SetFromDecimalString(const std::string &string) {
    data = CryptoPP::Integer(string.c_str());
  }

  /// Return a const reference to the zero integer
  static const Integer &Zero() { return zero; }

  operator CryptoPP::Integer &() { return data; }
};

class InPin : public Component<InPinID> {
  const char *label;      /// label which appears next to the pin
  const DataType type;    /// type of the pin, dictates shape
  const NodeID parent_id; /// NodeID of the parent
  LinkPtr link;           /// Optional link connected to this pin

  class ManualIntegerInput {
    std::string input_buffer;
    Integer integer;

  public:
    bool Display(const char *label, const float node_width) {
      const float textbox_width = node_width - ImGui::CalcTextSize(label).x;

      ImGui::SetNextItemWidth(textbox_width);
      if (ImGui::InputText(label, &input_buffer,
                           ImGuiInputTextFlags_CharsDecimal)) {
        integer.SetFromDecimalString(input_buffer);
        return true;
      }

      return false;
    }

    const Integer &GetInteger() const { return integer; }
  };

  const bool display_manual_input;
  std::unique_ptr<ManualIntegerInput> manual_input = nullptr;

public:
  InPin(const char *label, DataType type, NodeID parent_id,
        bool display_manual_input = true)
      : label(label), type(type), parent_id(parent_id),
        display_manual_input(display_manual_input), Component() {}

  bool Display(const float node_width) {
    ImGui::PushID(GetID());
    bool modified = false;

    // Make sure all conditions are met to show manual input integer
    bool is_connected = (link == nullptr);
    bool should_display_manual_input =
        !is_connected && display_manual_input && (type == DataType::INTEGER);

    ImNodes::BeginInputAttribute(GetID(), DataTypeToShape(type));
    if (should_display_manual_input) {
      // Allocate a manual input if we haven't allocated yet
      if (manual_input == nullptr) {
        manual_input = std::make_unique<ManualIntegerInput>();
      }

      manual_input->Display(label, node_width);
    } else {
      // Destroy manual input if we haven't yet
      if (manual_input != nullptr) {
        manual_input.reset(nullptr);
      }

      ImGui::TextUnformatted(label);
    }
    ImNodes::EndInputAttribute();
    ImGui::PopID();
    return modified;
  }

  const Integer &GetManualInputInteger() const {
    return manual_input->GetInteger();
  }

  const NodeID GetParentID() const { return parent_id; }
  DataType GetType() const { return type; }

  LinkPtr GetLink() { return link; }
  void SetLink(LinkPtr _link) { link = _link; }
};

class OutPin : public Component<OutPinID> {
  const char *label;       /// label which appears next to the pin
  const DataType type;     /// type of the pin, dictates shape
  const NodeID parent_id;  /// NodeID of the parent
  std::set<LinkPtr> links; /// Links connected to this pin

public:
  OutPin(const char *label, DataType type, NodeID parent_id)
      : label(label), type(type), parent_id(parent_id), Component() {}

  bool Display(const float node_width) {
    // Calculate offset so that pin label is right-aligned
    const float indent_width = node_width - ImGui::CalcTextSize(label).x;

    ImNodes::BeginOutputAttribute(GetID(), DataTypeToShape(type));
    ImGui::Indent(indent_width);
    ImGui::TextUnformatted(label);
    ImNodes::EndOutputAttribute();
    return false;
  }

  const NodeID GetParentID() const { return parent_id; }
  DataType GetType() const { return type; }

  const std::set<LinkPtr> &GetLinks() const { return links; }
  void AddLink(LinkPtr link) { links.insert(link); }
  void RemoveLink(LinkPtr link) { links.erase(link); }
};

class Link : public Component<LinkID> {
  const DataType type;
  std::variant<Buffer, Integer> data;

  const NodeID in_node;
  const NodeID out_node;

  const OutPinID in_pin;
  const InPinID out_pin;

public:
  Link(const OutPinPtr &in, const InPinPtr &out)
      : in_pin(in->GetID()), out_pin(out->GetID()), in_node(in->GetParentID()),
        out_node(out->GetParentID()), type(out->GetType()), Component() {
    switch (type) {
    case DataType::BUFFER:
      data = Buffer();
      break;
    case DataType::INTEGER:
      data = Integer();
      break;
    }
  }

  bool Display() {
    ImNodes::Link(GetID(), in_pin, out_pin);
    return false;
  }

  const Buffer &getBuffer() const { return std::get<Buffer>(data); }
  const Integer &getInteger() const { return std::get<Integer>(data); }
  DataType GetType() const { return type; }

  void setBuffer(const Buffer &buffer) { data = buffer; }
  void setInteger(const Integer &integer) { data = integer; }

  NodeID GetInNode() const { return in_node; }
  NodeID GetOutNode() const { return out_node; }
  OutPinID GetInPin() const { return in_pin; }
  InPinID GetOutPin() const { return out_pin; }
};

/// A complete set of nodes connected by links
class GraphStorage {
protected:
  std::map<NodeID, NodePtr> nodes;        /// Map Component ID to Node
  std::map<LinkID, LinkPtr> links;        /// Map Component ID to Link
  std::map<InPinID, InPinPtr> in_pins;    /// Map Component ID to input pins
  std::map<OutPinID, OutPinPtr> out_pins; /// Map Component ID to input pins

public:
  NodePtr GetNode(NodeID node_id) { return nodes[node_id]; }
  LinkPtr GetLink(LinkID link_id) { return links[link_id]; }
  InPinPtr GetInPin(InPinID pin_id) { return in_pins[pin_id]; }
  OutPinPtr GetOutPin(OutPinID pin_id) { return out_pins[pin_id]; }

  void EraseNode(NodeID node_id) { nodes.erase(node_id); }
  void EraseLink(LinkID link_id) { links.erase(link_id); }
  void EraseInPin(InPinID pin_id) { in_pins.erase(pin_id); }
  void EraseOutPin(OutPinID pin_id) { out_pins.erase(pin_id); }
};

class Node : public Component<NodeID> {
  const char *label;
  float width;

  bool error = false;
  bool error_toggle = false;
  std::string error_message;

  class StaticAttribute {};
  Component<ID<StaticAttribute>> internal;

public:
  bool needs_update = true;

  std::map<InPinID, std::shared_ptr<InPin>> in_pins;
  std::map<OutPinID, std::shared_ptr<OutPin>> out_pins;

public:
  Node(const char *label, float width = 120.0f)
      : label(label), width(width), Component() {}

  const Integer &GetInInteger(InPinID in_id) {
    auto in_link = in_pins[in_id]->GetLink();
    if (in_link != nullptr) {
      return in_link->getInteger();
    }

    return in_pins[in_id]->GetManualInputInteger();
  }

  const Buffer &GetInBuffer(InPinID in_id) {
    auto in_link = in_pins[in_id]->GetLink();
    if (in_link != nullptr) {
      return in_link->getBuffer();
    }

    return Buffer::Empty();
  }

  void SetOutBuffer(OutPinID out_id, const Buffer &buffer) {
    for (auto out_link : out_pins[out_id]->GetLinks()) {
      out_link->setBuffer(buffer);
    }
  }

  void SetInBuffer(OutPinID out_id, const Buffer &buffer) {
    for (auto out_link : out_pins[out_id]->GetLinks()) {
      out_link->setBuffer(buffer);
    }
  }

  InPinID AddInputPin(const char *label, DataType type,
                      bool display_manual_input = true) {
    InPinPtr pin =
        std::make_shared<InPin>(label, type, GetID(), display_manual_input);
    in_pins[pin->GetID()] = pin;
    return pin->GetID();
  }

  OutPinID AddOutputPin(const char *label, DataType type) {
    OutPinPtr pin = std::make_shared<OutPin>(label, type, GetID());
    out_pins[pin->GetID()] = pin;
    return pin->GetID();
  }

  bool Display() {
    bool modified = false;
    ImNodes::BeginNode(GetID());

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(label);
    ImNodes::EndNodeTitleBar();

    for (auto pin : out_pins) {
      modified |= pin.second->Display(width);
    }

    ImNodes::BeginStaticAttribute(internal.GetID());
    ImGui::PushID(internal.GetID());
    modified |= DisplayInternal();

    // If there was an error with the calculation, display it
    if (error) {
      if (!error_toggle) {
        error_toggle = true;
        modified = true;
      }

      ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + width);
      ImGui::TextDisabled("[ERROR] %s", error_message.c_str());
      ImGui::PopTextWrapPos();
    }

    ImGui::PopID();
    ImNodes::EndStaticAttribute();

    for (auto pin : in_pins) {
      modified |= pin.second->Display(width);
    }

    ImNodes::EndNode();
    return modified;
  }

  void Recalculate(GraphStorage &graph) {
    if (needs_update) {
      needs_update = false;

      for (auto pin : in_pins) {
        auto link = pin.second->GetLink();
        if (link != nullptr) {
          graph.GetNode(link->GetInNode())->Recalculate(graph);
        }
      }

      error = true;
      const char *what = "";

      /// Check for errors in internal calculation to avoid crashing the program
      try {
        RecalculateInternal();
        error = false;
        error_toggle = false;
      } catch (CryptoPP::Exception &e) {
        what = e.what();
      } catch (std::exception &e) {
        what = e.what();
      }

      error_message = std::string(what);
    }
  }

protected:
  virtual bool DisplayInternal() { return false; };
  virtual bool RecalculateInternal() = 0;
};

class Graph : public GraphStorage {
public:
  /// Destroy a link from the graph.
  void Disconnect(LinkPtr link) {
    auto start_pin = out_pins[link->GetInPin()];
    auto end_pin = in_pins[link->GetOutPin()];

    if (start_pin != nullptr) {
      start_pin->RemoveLink(link);
    }

    if (end_pin != nullptr) {
      end_pin->SetLink(nullptr);
    }

    links.erase(link->GetID());
  }

  /// Destroy a link from the graph.
  void Disconnect(LinkID link_id) {
    auto link = links[link_id];

    if (link != nullptr) {
      Disconnect(link);
    }
  }

  /// Destroy a link between the two pins in the graph if one exists. Returns
  /// true if a link was found and destroyed.
  bool Disconnect(OutPinID start_id, InPinID end_id) {
    auto start_pin = out_pins[start_id];
    auto end_pin = in_pins[end_id];

    // Make sure pins exist, pin types match
    if (start_pin != nullptr ||
        end_pin != nullptr && start_pin->GetType() == end_pin->GetType()) {
      auto link = end_pin->GetLink();

      if (link != nullptr) {
        Disconnect(link);
        return true;
      }
    }

    return false;
  }

  /// Create a link between two pins in the graph, if such a link would be
  /// valid. Returns the ID of the created link if this operation was
  /// succesful.
  void Connect(OutPinID start_id, InPinID end_id) {
    auto start_pin = out_pins[start_id];
    auto end_pin = in_pins[end_id];

    // TODO: prevent circular dependencies

    // Make sure pins exist, pin types match
    if (start_pin != nullptr ||
        end_pin != nullptr && start_pin->GetType() == end_pin->GetType()) {
      auto start_node = nodes[start_pin->GetParentID()];
      auto end_node = nodes[end_pin->GetParentID()];

      if (start_node != nullptr && end_node != nullptr) {
        auto link = std::make_shared<Link>(start_pin, end_pin);

        // If the output pin already has a connected pin, disconnect it
        auto old_link = end_pin->GetLink();
        if (old_link != nullptr) {
          Disconnect(old_link);
        }

        // Add new link and return ID
        end_pin->SetLink(link);
      }
    }
  }

  /// Add a node to the graph.
  void AddNode(NodePtr node_ptr) {
    nodes[node_ptr->GetID()] = node_ptr;

    for (auto pin : node_ptr->in_pins) {
      in_pins[pin.first] = pin.second;
    }

    for (auto pin : node_ptr->out_pins) {
      out_pins[pin.first] = pin.second;
    }
  }

  /// Remove a node from the graph.
  void RemoveNode(NodeID node_id) {
    NodePtr node_ptr = nodes[node_id];

    if (node_ptr != nullptr) {
      nodes.erase(node_ptr->GetID());

      for (auto pin : node_ptr->in_pins) {
        in_pins.erase(pin.first);
      }

      for (auto pin : node_ptr->out_pins) {
        out_pins.erase(pin.first);
      }
    }
  }

  /// Display the node graph, return whether or not the state was changed
  /// in a way that requires recalculation. (e.g. moving a node will not
  /// register as a state change, but editing a node
  bool Display() {
    bool modified = false;

    // Display nodes
    for (auto pair : nodes) {
      modified |= pair.second->Display();
    }

    // Display links
    for (auto pair : links) {
      modified |= pair.second->Display();
    }

    return modified;
  }

  /// Recalculate the node graph
  void Recalculate() {
    for (auto pair : nodes) {
      pair.second->needs_update = true;
    }

    for (auto pair : nodes) {
      pair.second->Recalculate(*this);
    }
  }
};

} // namespace cptl

namespace ImGui {

template <typename NODE>
bool AddNodeMenuItem(cptl::Graph &graph, const char *label) {
  if (ImGui::MenuItem(label)) {
    auto node = std::make_shared<NODE>();
    graph.AddNode(node);

    ImNodes::SetNodeScreenSpacePos(node->id, ImGui::GetMousePos());
    ImNodes::SnapNodeToGrid(node->id);
    return true;
  }

  return false;
}

} // namespace ImGui
