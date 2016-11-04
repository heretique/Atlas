#define SDL_MAIN_HANDLED

#include "MainWindow.h"
#include "SDLApp.h"
#include "SDLWindow.h"
#include "fmt/format.h"
#include "imgui/imgui.h"
#include "Engine.h"
#include "JobManager.h"
#include "AssetManager.h"
#include <thread>
#include <chrono>
#include "LogManager.h"

using namespace atlasEditor;
using namespace atlas;

void emptyJob(void* data, uint count)
{
    int* testData = static_cast<int*>(data);
    for (int i = 0;  i < count; ++i)
        testData[i] = 1;
}


class Window1: public SDLWindow
{
public:
    Window1(const char *title,
            int x, int y, int w, int h) :
        SDLWindow(title, x, y, w, h) {}

    void init()
    {
        Engine::init();
        Engine::assets().setAssetsDir("/home/cata/temp/");
        AssetHandle cube = Engine::assets().addAsset((int)AssetTypes::Geometry, "base_head.obj", 0);

        cout << "Waiting for jobs..." << endl;

        int testArray[100000];
        memset(testArray, 0, sizeof(testArray));

        Engine::jobMan().parallel_for<int, CountSplitter<int, 256>>(emptyJob, testArray, sizeof(testArray)/sizeof(testArray[0]));

        Engine::jobMan().addSignalingJob([](void *, uint){std::this_thread::sleep_for(std::chrono::seconds(2));}, nullptr, 0, [](){cout << "JOB DONE" << endl;});

        Engine::jobMan().wait();


        for(int i = 0;  i < sizeof(testArray)/sizeof(testArray[0]); ++i)
        {
//            fmt::print("{} ", testArray[i]);
            if (testArray[i] != 1)
            {
                fmt::print("Jobs failed!!!\n");
                break;
            }
        }
        cout << "Finised waiting for jobs..." << endl;

        Engine::assets().loadAssets();
    }

    ~Window1()
    {
        Engine::release();
    }

    void onGUI() {
        ImGui::Text("Window1");
    }
};

class Window2: public SDLWindow
{
public:
    Window2(const char *title,
            int x, int y, int w, int h) :
        SDLWindow(title, x, y, w, h) {}

    void onGUI() {
        ImGui::Text("Window2");
        if (ImGui::Button("Test", ImVec2(50, 20)))
        {
            ImGui::Text("Clicked");
        }
    }
};

class Window3: public SDLWindow
{
public:
    Window3(const char *title,
            int x, int y, int w, int h) :
        SDLWindow(title, x, y, w, h) {}

    void onGUI() {
        ImGui::Text("Window3");
        if (ImGui::Button("Test", ImVec2(100, 20)))
        {
            ImGui::Text("Clicked");
        }
    }
};

int main(int argc, char **argv)
{
    SDLApp::get().init(SDL_INIT_VIDEO);

    Window1* win = new Window1(fmt::format("BGFX {}", 1).c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480);
    win->init();
        new Window2(fmt::format("BGFX {}", 2).c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480);
        new Window3(fmt::format("BGFX {}", 3).c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480);

    return SDLApp::get().exec();
}

