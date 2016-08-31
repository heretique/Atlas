#ifndef WINDOW_H
#define WINDOW_H

#include <Base.h>
#include <SDLWindow.h>
#include <EditorViews.h>

struct ImDrawData;


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

protected:
    // imgui related
    bool imguiInit();
    void imguiShutdown();
    void imguiNewFrame();
    void imguiRenderDrawLists(ImDrawData* drawData);

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
