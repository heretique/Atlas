#include "MainWindow.h"

#include "Core/Engine.h"
#include "Managers/AssetManager.h"
#include "Managers/JobManager.h"
#include "Managers/SceneManager.h"
#include <cmath>

#include <easy/profiler.h>
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
    Engine::assets().addAsset(AssetTypes::Code, "scripts/main.wren");
    Engine::assets().addAsset(AssetTypes::Code, "scripts/ImGui.wren");
    Engine::assets().addAsset(AssetTypes::Code, "scripts/Math.wren");
    Engine::assets().addAsset(AssetTypes::Code, "scripts/Utils.wren");
    Engine::assets().addAsset(AssetTypes::Code, "scripts/Assets.wren");
    Engine::assets().addAsset(AssetTypes::Code, "scripts/Engine.wren");
    Engine::assets().addAsset(AssetTypes::Code, "scripts/Scene.wren");
    Engine::assets().addAsset(AssetTypes::Code, "scripts/TestNodeScript.wren");
    Engine::assets().loadAssets();

    _vmResult = Engine::vm().executeModule("scripts/main");
    if (_vmResult == wrenpp::Result::Success)
    {
        _init   = Engine::vm().method("main", "Main", "init()");
        _update = Engine::vm().method("main", "Main", "update(_)");
        _ongui  = Engine::vm().method("main", "Main", "onGUI()");

        // call init
        _init();
    }
}

void MainWindow::update(float dt)
{
    EASY_FUNCTION(profiler::colors::Amber);
    if (_vmResult == wrenpp::Result::Success)
    {
        _update(dt);
    }
    Engine::scene().update(dt);
}

void MainWindow::onGUI()
{
    EASY_FUNCTION(profiler::colors::Amber);
    //    if (ImGui::Button("Test", ImVec2(50, 20)))
    //    {
    //        ImGui::Text("Clicked");
    //    }
    if (_vmResult == wrenpp::Result::Success)
    {
        _ongui();
    }
}

}  // namespace atlas
