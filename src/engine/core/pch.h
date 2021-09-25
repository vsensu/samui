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
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#endif // SAMUI_PCH_H_