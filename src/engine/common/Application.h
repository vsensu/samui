//
// Created by vsensu on 2020/9/20.
//

#ifndef LEARNCRAFT_APPLICATION_H
#define LEARNCRAFT_APPLICATION_H

#include <tuple>
#include <functional>

struct GLFWwindow;

class Application
{
public:
    virtual ~Application() {}

    Application();
    Application(int argc, char *argv[]);
    int Run();

    using Color = std::tuple<float, float, float, float>;

    static inline Color MakeColor(float r, float g, float b, float a)
    {
        return std::make_tuple(r, g, b, a);
    }

    void SetFixedFPS(int fps)
	{
    	if(fps < 1)
		{
    		fixed_fps_ = false;
		}
		else
		{
			// TODO: fixed fps
			fixed_fps_ = true;
		}
	}

protected:
    double delta_time_;
    float fps_;
    GLFWwindow* window_ {nullptr};

private:
    int InitOpenGL();
    int InitImgui();
    // Custom init
    virtual void Init() {}
    virtual void FixedUpdate(double fixedDeltaTime) {}
    virtual void Update() {}
    virtual void RenderScene() {}
    virtual void RenderUI() {}
    virtual void HandleKeyboard(GLFWwindow *window) {}

    // no need to free memory in deconstructor, glfwTerminate will handle it.
    Color clear_color_ { MakeColor(0.f, 0.f, 0.f, 1.f)};
    bool init_ {false};
    bool fixed_fps_ { true };
};

#endif //LEARNCRAFT_APPLICATION_H
