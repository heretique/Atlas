#include "MainWindow.h"
#include <ImGUI/imgui.h>

namespace atlasEditor {


MainWindow::MainWindow(const char *title,
                       int x, int y, int w,
                       int h, u32 flags) : SDLWindow(title, x, y, w, h, flags)
{

}


MainWindow::~MainWindow()
{

}


bool MainWindow::imguiInit()
{
    ImGuiIO& io = ImGui::GetIO();

//#if defined(_WIN32) || defined(_WIN64)
//    io.ImeWindowHandle = reinterpret_cast<HWND>(winId());
//#endif

//    io.RenderDrawListsFn = &imguiRenderDrawLists;

    return true;
}

void MainWindow::imguiShutdown()
{

}

void MainWindow::imguiNewFrame()
{

}

void imguiRenderDrawLists(ImDrawData *drawData)
{

}

} // atlasEditor

