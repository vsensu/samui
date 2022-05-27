#ifdef SAMUI_PLATFORM_WINDOWS

// clang-format off
#include "platform_utils.h"

#include <Windows.h>
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <core/engine.h>

#include "../graphics_application.h"
// clang-format on

namespace samui
{

namespace DialogUtils
{
// These return empty strings if cancelled
std::string open_file(const char* filter)
{
    OPENFILENAMEA ofn;
    CHAR          szFile[260] = {0};
    CHAR          currentDir[256] = {0};
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    std::shared_ptr<GraphicsApplication> app =
        std::dynamic_pointer_cast<GraphicsApplication>(
            Engine::instance().app());
    ofn.hwndOwner = glfwGetWin32Window(
        (GLFWwindow*)app->get_window().get_native_window());
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    if (GetCurrentDirectoryA(256, currentDir)) ofn.lpstrInitialDir = currentDir;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn) == TRUE) return ofn.lpstrFile;

    return std::string();
}

std::string save_file(const char* filter)
{
    OPENFILENAMEA ofn;
    CHAR          szFile[260] = {0};
    CHAR          currentDir[256] = {0};
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    std::shared_ptr<GraphicsApplication> app =
        std::dynamic_pointer_cast<GraphicsApplication>(
            Engine::instance().app());
    ofn.hwndOwner = glfwGetWin32Window(
        (GLFWwindow*)app->get_window().get_native_window());
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    if (GetCurrentDirectoryA(256, currentDir)) ofn.lpstrInitialDir = currentDir;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

    // Sets the default extension by extracting it from the filter
    ofn.lpstrDefExt = strchr(filter, '\0') + 1;

    if (GetSaveFileNameA(&ofn) == TRUE) return ofn.lpstrFile;

    return std::string();
}

}  // namespace DialogUtils

}  // namespace samui

#endif  // SAMUI_PLATFORM_WINDOWS