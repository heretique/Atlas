#define SDL_MAIN_HANDLED

#include "AssetManager.h"
#include "Engine.h"
#include "JobManager.h"
#include "LogManager.h"
#include "MainWindow.h"
#include "Math/Utils.h"
#include "SDLApp.h"
#include "SDLWindow.h"
#include "imgui/imgui.h"

#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

#include <easy/profiler.h>
#include <fmt/printf.h>
#include <microbench/microbench.h>

using namespace atlasEditor;
using namespace atlas;

void testJob(void *data, uint count) {
  float *testData = static_cast<float *>(data);
  for (uint i = 0; i < count; ++i)
    testData[i] = math::sin((float)i / count) + math::cos((float)i / count);
}

class Window1 : public SDLWindow {
public:
  Window1(const char *title, int x, int y, int w, int h)
      : SDLWindow(title, x, y, w, h) {}
  bool m_open{true};
  void init() {
    Engine::init();
    Engine::assets().setAssetsDir("/home/cata/temp/");
    AssetHandle cube = Engine::assets().addAsset(
        static_cast<int>(AssetTypes::Geometry), "base_head.obj", 0);

    Engine::assets().loadAssets();

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

  ~Window1() { Engine::release(); }

  void onGUI() {}
};

class Window2 : public SDLWindow {
public:
  Window2(const char *title, int x, int y, int w, int h)
      : SDLWindow(title, x, y, w, h) {}

  void onGUI() {
    ImGui::Text("Window2");
    if (ImGui::Button("Test", ImVec2(50, 20))) {
      ImGui::Text("Clicked");
    }
  }
};

class Window3 : public SDLWindow {
public:
  Window3(const char *title, int x, int y, int w, int h)
      : SDLWindow(title, x, y, w, h) {}

  void onGUI() {
    ImGui::Text("Window3");
    if (ImGui::Button("Test", ImVec2(100, 20))) {
      ImGui::Text("Clicked");
    }
  }
};

int main(int argc, char **argv) {
  EASY_MAIN_THREAD;

  SDLApp &app = SDLApp::get();
  if (0 < app.init(SDL_INIT_VIDEO)) {
    const char *err = SDL_GetError();
    fmt::print("SDL failed to init, err: {}\n", err);
    return -1;
  }

  // TODO: refactor this abomination, can't see ownership on windows
  //  Window1 *win =
  //      new Window1(fmt::format("BGFX {}", 1).c_str(),
  //      SDL_WINDOWPOS_UNDEFINED,
  //                  SDL_WINDOWPOS_UNDEFINED, 1920, 1080);
  //  win->init();

  MainWindow *win =
      new MainWindow(fmt::format("BGFX {}", 1).c_str(), SDL_WINDOWPOS_UNDEFINED,
                     SDL_WINDOWPOS_UNDEFINED, 1920, 1080);
  win->init();
  app.addWindow(win);
  //    new Window2(fmt::format("BGFX {}", 2).c_str(), SDL_WINDOWPOS_UNDEFINED,
  //    SDL_WINDOWPOS_UNDEFINED, 640, 480);
  //  new Window3(fmt::format("BGFX {}", 3).c_str(), SDL_WINDOWPOS_UNDEFINED,
  //              SDL_WINDOWPOS_UNDEFINED, 640, 480);

  profiler::startListen();

  return SDLApp::get().exec();
}
