#include "MainWindow.h"

#include "Components/MaterialComponent.h"
#include "Components/MeshComponent.h"
#include "Core/Engine.h"
#include "Managers/AssetManager.h"
#include "Managers/JobManager.h"
#include "Managers/SceneManager.h"
#include <cmath>
#include <spdlog/spdlog.h>

//#include <easy/profiler.h>
#include <imgui/imgui.h>

namespace atlas
{
void testJob(void* data, uint count)
{
    float* testData = static_cast<float*>(data);
    for (uint i = 0; i < count; ++i)
        testData[i] = std::sin((float)i / count) + std::cos((float)i / count);
}

MainWindow::MainWindow(const char* title, int x, int y, int w, int h)
    : SDLWindow(title, x, y, w, h)
{
}

MainWindow::~MainWindow() {}

void MainWindow::init()
{
    std::string basePath = SDL_GetBasePath();
    Engine::init();
    Engine::assets().setAssetsDir(basePath);
    AssetPtr object   = Engine::assets().addAsset(AssetTypes::Geometry, "assets/caruta.obj");
    AssetPtr material = Engine::assets().addAsset(AssetTypes::Material, "assets/unlit_textured.material");
    Engine::assets().addAsset(AssetTypes::Code, "scripts/TestNodeScript.wren");
    Engine::assets().loadAssets();

    if (wrenpp::Result::Success != Engine::vm().executeString("main", Engine::wrenModule()))
    {
    }

    // test node
    Node* node =
        Engine::scene().addSpatialNode("Test Node", Engine::scene().root(), math::Vector3::zero(), object, material);
    Engine::scene().attachScript(node, "scripts/TestNodeScript");

    // camera node
    Engine::scene().addCameraNode("Camera", Engine::scene().root(), 60.f, 1.f, 1.f, 100.f, math::Vector3(0.f, 5.f, 5.f),
                                  math::Vector3::zero(), math::Vector3::unitZ());
}

void MainWindow::update(float dt)
{
    //    EASY_FUNCTION(profiler::colors::Amber);
    Engine::scene().update(dt);
}

void MainWindow::onGUI()
{
    //    EASY_FUNCTION(profiler::colors::Amber);
    const bgfx::Stats* stats   = Engine::bgfxStats();
    const double       toMsCpu = 1000.0 / stats->cpuTimerFreq;
    const double       toMsGpu = 1000.0 / stats->gpuTimerFreq;
    const double       frameMs = double(stats->cpuTimeFrame) * toMsCpu;
    ImGui::Text("Frame %0.3f [ms], %0.3f FPS", frameMs, 1000.0 / frameMs);

    ImGui::Text("Submit CPU %0.3f, GPU %0.3f (L: %d)", double(stats->cpuTimeEnd - stats->cpuTimeBegin) * toMsCpu,
                double(stats->gpuTimeEnd - stats->gpuTimeBegin) * toMsGpu, stats->maxGpuLatency);
    Engine::scene().updateGUI();
}

}  // namespace atlas
