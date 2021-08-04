#ifndef SAMUI_PCH_H_
#define SAMUI_PCH_H_

#include <memory>
#include <functional>
#include <string>
#include <sstream>
#include <vector>
#include <array>

#ifdef SAMUI_PLATFORM_WINDOWS
#define NOMINMAX    // Don't define min and max macros
#include <Windows.h>
#endif

#endif // SAMUI_PCH_H_