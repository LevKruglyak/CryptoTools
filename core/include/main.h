#pragma once

#include "docking_params.h"
#include <chrono>
#include <functional>
#include <sstream>
#include <string>

#ifdef HELLOIMGUI_USE_SDL_OPENGL3
#define SDL_MAIN_HANDLED
#include <SDL.h>
#endif

#include "hello_imgui/hello_imgui.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

inline const char *DOCK_AREA = "MiscArea";
