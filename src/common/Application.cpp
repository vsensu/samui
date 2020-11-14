//
// Created by vsensu on 2020/9/20.
//

#include "Application.h"

#include <iostream>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

constexpr double g_update_freq = 16.6 / 1000;

Application::Application()
: Application(0, nullptr)
{
}

Application::Application(int argc, char *argv[])
{
    InitOpenGL();
    InitImgui();
}

int Application::InitOpenGL()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window_ = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Samui", nullptr, nullptr);
    if (window_ == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
//    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    return 0;
}

int Application::InitImgui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return 0;
}

int Application::Run()
{
	if(!init_)
    {
	    Init();
	    init_ = true;
    }

    // loop
    // -----------
    auto previous = std::chrono::steady_clock::now();
    double lag = 0;
    while (!glfwWindowShouldClose(window_))
    {
        auto current = std::chrono::steady_clock::now();
        auto elapsed_seconds = std::chrono::duration<double>(current-previous).count();
        previous = current;
        if(fixed_fps_)
		{
			lag += elapsed_seconds;
			delta_time_ = lag;
		}
		else
		{
			delta_time_ = elapsed_seconds;
		}

        if(fixed_fps_ && lag < g_update_freq)
            continue;

        // input
        // -----
        HandleKeyboard(window_);

        if(fixed_fps_)
		{
			fps_ = static_cast<float>(1 / lag);
		}
		else
		{
			fps_ = static_cast<float>(1 / elapsed_seconds);
		}

		if(fixed_fps_)
		{
			while(lag >= g_update_freq)
			{
				FixedUpdate(g_update_freq);
				lag -= g_update_freq;
			}
		}
		else
		{
			FixedUpdate(g_update_freq);
		}

		Update();

        // render
        // ------
        auto [r, g, b, a] = clear_color_;
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);

        // First Render Scene
        RenderScene();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Finally Render UI
        RenderUI();

        // Rendering
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void HandleKeyboard(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
