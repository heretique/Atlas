#include "MainWindow.h"
#include <QCoreApplication>
#include <QKeyEvent>
#include <ImGUI/imgui.h>

namespace atlasEditor {


MainWindow::MainWindow() : QBGFXWindow(Q_NULLPTR)
{

}


MainWindow::~MainWindow()
{

}


void MainWindow::initializeBGFX()
{
    addView<SceneView>();
}

void MainWindow::releaseBGFX()
{

}

void MainWindow::resizeBGFX(const QSize &size)
{
    Q_UNUSED(size);
}

void MainWindow::paintBGFX()
{
    for (auto& view: _views)
        view->onGUI();

    update();
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        qApp->quit();
}

bool MainWindow::imguiInit()
{
    ImGuiIO& io = ImGui::GetIO();

#if defined(_WIN32) || defined(_WIN64)
    io.ImeWindowHandle = reinterpret_cast<HWND>(winId());
#endif

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

