// local
#include "utils/logger.h"
#include "systems/window/window_context.h"

#include "demos/cube/cube.h"
#include "resources/images/icon/icon.h"

#define ICON_DATA { image_width, image_height, image_data }
#define WINDOW_PARAMS { 1280, 720, "Cube Demo", 22.0f, ICON_DATA }


int main(void)
{
    Logger::SetPriority(Logger::LogPriority::Debug);
    WindowContext context = WindowContext(WINDOW_PARAMS);

    CubeDemo demo = CubeDemo(context);

    auto update = [&](float deltaTime)
    {
        demo.gameUpdate(deltaTime);
        demo.uiUpdate();
    };

    context.SetDeltaUpdate(update);
    context.BeginLoop();

    Logger::Log("Exiting...");
    return 0;
}