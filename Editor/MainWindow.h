#ifndef WINDOW_H
#define WINDOW_H

#include <Base.h>
#include <SDLWindow.h>
#include <EditorViews.h>


namespace atlasEditor {

class MainWindow : public SDLWindow
{
public:
    MainWindow(const char *title,
               int x, int y, int w,
               int h, u32 flags);
    ~MainWindow();
    template <class T>
    void addView();

private:
    vector<std::unique_ptr<EditorView>> _views;
};


template <class T>
void MainWindow::addView()
{
    _views.push_back(make_unique<T>());
}

} // atlasEditor


#endif // WINDOW_H
