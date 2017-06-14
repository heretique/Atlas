#define SDL_MAIN_HANDLED

#include "AssetManager.h"
#include "Engine.h"
#include "JobManager.h"
#include "LogManager.h"
#include "MainWindow.h"
#include "SDLApp.h"
#include "SDLWindow.h"
#include "fmt/format.h"
#include "imgui/imgui.h"

#include <chrono>
#include <microbench/microbench.h>
#include <thread>

using namespace atlasEditor;
using namespace atlas;

void testJob(void *data, uint count) {
  float *testData = static_cast<float *>(data);
  for (int i = 0; i < count; ++i)
    testData[i] = sin((float)i / count) + cos((float)i / count);
}

class Window1 : public SDLWindow {
public:
  Window1(const char *title, int x, int y, int w, int h)
      : SDLWindow(title, x, y, w, h) {}

  void init() {
    Engine::init();
    Engine::assets().setAssetsDir("/home/cata/temp/");
    AssetHandle cube = Engine::assets().addAsset(
        static_cast<int>(AssetTypes::Geometry), "base_head.obj", 0);

    Engine::assets().loadAssets();

    cout << "Waiting for jobs..." << endl;

    const int COUNT = 30;
    float testArray[COUNT];
    memset(testArray, 0, sizeof(testArray));

    moodycamel::stats_t stats = moodycamel::microbench_stats(
        [testArray]() { testJob((float *)testArray, COUNT); }, 100, 100, true);

    double speed = stats.avg();
    printf("Serial job: %.4f\n", stats.avg());

    memset(testArray, 0, sizeof(testArray));

    stats = moodycamel::microbench_stats(
        [testArray]() {
          Engine::jobMan().parallel_for<float, CountSplitter<float, 5>>(
              testJob, (float *)testArray,
              sizeof(testArray) / sizeof(testArray[0]));
          Engine::jobMan().wait();
        },
        100, 100, true);

    printf("Parallel job: %.4f\n", stats.avg());
    speed /= stats.avg();
    printf("Speed parallel vs serial: %.4fx\n", speed);

    //    Engine::jobMan().addSignalingJob(
    //        [](void *, uint) {
    //          std::this_thread::sleep_for(std::chrono::seconds(2));
    //        },
    //        nullptr, 0, []() { cout << "JOB DONE" << endl; });

    //    Engine::jobMan().wait();

    //    for (uint i = 0; i < sizeof(testArray) / sizeof(testArray[0]); ++i) {
    //      //            fmt::print("{} ", testArray[i]);
    //      if (testArray[i] != 1) {
    //        fmt::print("Jobs failed!!!\n");
    //        break;
    //      }
    //    }

    cout << "Finised waiting for jobs..." << endl;
  }

  ~Window1() { Engine::release(); }

  void onGUI() { ImGui::Text("Window1"); }
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
  SDLApp::get().init(SDL_INIT_VIDEO);

  Window1 *win =
      new Window1(fmt::format("BGFX {}", 1).c_str(), SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED, 640, 480);
  win->init();
  //  new Window2(fmt::format("BGFX {}", 2).c_str(), SDL_WINDOWPOS_UNDEFINED,
  //              SDL_WINDOWPOS_UNDEFINED, 640, 480);
  //  new Window3(fmt::format("BGFX {}", 3).c_str(), SDL_WINDOWPOS_UNDEFINED,
  //              SDL_WINDOWPOS_UNDEFINED, 640, 480);

  return SDLApp::get().exec();
}
