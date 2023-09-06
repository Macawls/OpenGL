// local
#include "utils/logger.h"
#include "systems/window/window_context.h"

// #include "demos/triangle/triangle.h"
#include "demos/cube/cube.h"

#define WIDTH 1600
#define HEIGHT 900
#define WINDOW_TITLE "Cube Demo"

int main(void)
{
    Logger::SetPriority(Logger::LogPriority::Debug);
    
    WindowContext context = WindowContext(WIDTH, HEIGHT, WINDOW_TITLE);
    CubeDemo demo = CubeDemo(context);

    auto update = [&](float deltaTime)
    {
        demo.gameUpdate(deltaTime);
        demo.uiUpdate();
    };

    context.SetUpdate(update);
    context.BeginLoop();
    
    Logger::Log("Exiting...");
    return 0;
}