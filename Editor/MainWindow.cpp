#include "MainWindow.h"
#include <iostream>
#include <ratio>
#include <Engine.h>
#include <Managers/JobManager.h>
#include <bgfx/embedded_shader.h>
#include <bx/math.h>
#include <bx/timer.h>
#include <bx/uint32_t.h>
#include <easy/profiler.h>
#include <imgui/imgui.h>

#include "fs_drawstress.bin.h"
#include "vs_drawstress.bin.h"

namespace atlasEditor
{
using namespace std::chrono;
const int MAX_TASKS = 64 * 1024;

MainWindow::MainWindow(const char* title, int x, int y, int w, int h)
    : SDLWindow(title, x, y, w, h)
{
}

MainWindow::~MainWindow() {}

void MainWindow::init()
{
    atlas::Engine::init();
}

void MainWindow::update(float dt) {}  // namespace atlasEditor

void MainWindow::onGUI() {}

}  // namespace atlasEditor
