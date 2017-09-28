#ifndef WINDOW_H
#define WINDOW_H

#include <vector>
#include <EditorViews.h>
#include <SDLWindow.h>

namespace atlasEditor
{
class MainWindow : public SDLWindow
{
public:
    MainWindow(const char* title, int x, int y, int w, int h);
    ~MainWindow();
    template <class T>
    void addView();

private:
    std::vector<std::unique_ptr<EditorView> > _views;
};

template <class T>
void MainWindow::addView()
{
    _views.push_back(std::make_unique<T>());
}

}  // atlasEditor

#endif  // WINDOW_H
