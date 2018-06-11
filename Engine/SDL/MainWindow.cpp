#include "MainWindow.h"

#include "Core/Engine.h"
#include "Managers/AssetManager.h"
#include "Managers/JobManager.h"
#include "Managers/SceneManager.h"
#include "Nodes/Types.h"
#include <cmath>
#include <spdlog/spdlog.h>

//#include <easy/profiler.h>
#include <imgui/imgui.h>

namespace atlas
{
void testJob(void* data, uint count)
{
    float* testData = static_cast<float*>(data);
    for (uint i     = 0; i < count; ++i)
        testData[i] = std::sin((float)i / count) + std::cos((float)i / count);
}

MainWindow::MainWindow(const char* title, int x, int y, int w, int h)
    : SDLWindow(title, x, y, w, h)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::init()
{
    std::string basePath = SDL_GetBasePath();
    Engine::init();
    Engine::assets().setAssetsDir(basePath);
    Engine::assets().addAsset(AssetTypes::Code, "scripts/TestNodeScript.wren");
    Engine::assets().loadAssets();

    if (wrenpp::Result::Success != Engine::vm().executeString(Engine::wrenModule()))
    {
    }

    NodePtr node = Engine::scene().addNode(NodeTypes::Spatial, "Test Node", Engine::scene().root());
    Engine::scene().attachScript(node, "scripts/TestNodeScript");
}

void MainWindow::update(float dt)
{
    //    EASY_FUNCTION(profiler::colors::Amber);
    Engine::scene().update(dt);
}

void MainWindow::onGUI()
{
    //    EASY_FUNCTION(profiler::colors::Amber);
    Engine::scene().onGUI();
}

}  // namespace atlas
