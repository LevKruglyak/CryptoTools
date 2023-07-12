#include "imgui.h"
#include "imgui_internal.h"
#include "main.h"
#include "misc/cpp/imgui_stdlib.h"

#include <string>

class Input {
public:
  Input();

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
};

class Output {
public:
  Output();

  void Render();

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
};
