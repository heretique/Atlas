#include "MainWindow.h"

#include "Core/Engine.h"
#include "Managers/AssetManager.h"
#include "Managers/JobManager.h"
#include <cmath>

//#include <easy/profiler.h>
#include <imgui/imgui.h>

namespace atlas
{
void testJob(void* data, uint count)
{
    float* testData = static_cast<float*>(data);
    for (uint i     = 0; i < count; ++i)
        testData[i] = std::sinf((float)i / count) + std::cosf((float)i / count);
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
    Engine::init();
    Engine::assets().setAssetsDir("/Users/catalinm2/temp/");
    Engine::assets().addAsset(AssetTypes::Code, "atlas/main.wren");
    Engine::assets().addAsset(AssetTypes::Code, "atlas/imgui.wren");
    Engine::assets().loadAssets();

    _vmResult = Engine::vm().executeModule("atlas/main");
    if (_vmResult == wrenpp::Result::Success)
    {
        _init   = Engine::vm().method("main", "Main", "init()");
        _update = Engine::vm().method("main", "Main", "update(_)");
        _ongui  = Engine::vm().method("main", "Main", "onGUI()");
    }

    //    AssetHandle cube = Engine::assets().addAsset(static_cast<int>(AssetTypes::Geometry), "base_head.obj", 0);

    //    Engine::assets().loadAssets();

    //    std::cout << "Waiting for jobs..."
    //              << "\n";

    //    const int COUNT = 100000;
    //    float testArray[COUNT];
    //    std::memset(testArray, 0, sizeof(testArray));

    //    moodycamel::stats_t stats = moodycamel::microbench_stats(
    //        [testArray]() { testJob((float *)testArray, COUNT); });

    //    double speed = stats.avg();
    //    fmt::printf("Serial job: %.4f\n", stats.avg());

    //    std::memset(testArray, 0, sizeof(testArray));

    //    stats = moodycamel::microbench_stats([testArray]() {
    //      Engine::jobMan().parallel_for<float, CountSplitter<float, 1024>>(
    //          testJob, (float *)testArray,
    //          sizeof(testArray) / sizeof(testArray[0]));
    //      Engine::jobMan().wait();
    //    });

    //    fmt::printf("Parallel job: %.4f\n", stats.avg());
    //    speed /= stats.avg();
    //    fmt::printf("Speed parallel vs serial: %.4fx\n", speed);
    //    fmt::printf("Finised waiting for jobs...\n");
}

void MainWindow::update(float dt)
{
    //    EASY_FUNCTION(profiler::colors::Amber);
    if (_vmResult == wrenpp::Result::Success)
    {
        _update(dt);
    }
}

void MainWindow::onGUI()
{
    //    EASY_FUNCTION(profiler::colors::Amber);
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
