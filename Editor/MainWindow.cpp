#include "MainWindow.h"
#include <ImGUI/imgui.h>

namespace atlasEditor {


MainWindow::MainWindow(const char *title,
                       int x, int y, int w, int h) : SDLWindow(title, x, y, w, h)
{

}


MainWindow::~MainWindow()
{

}


} // atlasEditor

