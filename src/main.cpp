// local
#include "utils/logger.h"
#include "systems/window_context.h"

#include "demos/triangle.h"

#define WIDTH 1600
#define HEIGHT 900
#define WINDOW_TITLE "Triangle Demo"

int main(void)
{
    Logger::SetPriority(Logger::LogPriority::Debug);
    
    WindowContext context = WindowContext(WIDTH, HEIGHT, WINDOW_TITLE);
    TriangleDemo demo = TriangleDemo(context);

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