#include "imgui.h"
#include "imgui_internal.h"
#include "main.h"
#include "misc/cpp/imgui_stdlib.h"

#include <string>

class Input {
public:
  Input(std::string label, int height = 8) : label(label), height(height) {}

  bool Render();

  enum InputFormat {
    CHAR = 0,
    HEX = 1,
    DECIMAL = 2,
  };

  std::string getBytes() { return bytes; }

private:
  InputFormat format;
  std::string text;
  std::string bytes;

  std::string label;
  int height;
};

class Output {
public:
  Output(std::string label) : label(label) {}

  bool Render();

  enum OutputFormat {
    CHAR = 0,
    HEX = 1,
    HEXDUMP = 2,
    DECIMAL = 3,
    JAVA = 4,
    C = 5,
  };

  void setBytes(std::string input) { bytes = input; }

private:
  OutputFormat format;
  std::string text;
  std::string bytes;

  std::string label;
};
