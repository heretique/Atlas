#ifndef WINDOW_H
#define WINDOW_H

#include <chrono>
#include <memory>
#include <vector>
#include <EditorViews.h>
#include <SDLWindow.h>

namespace atlasEditor
{
struct JobData;

class MainWindow : public SDLWindow
{
public:
    MainWindow(const char* title, int x, int y, int w, int h);
    ~MainWindow();
    template <class T>
    void addView();

public:
    void init() override;
    void update(float dt) override;
    void onGUI() override;

private:
    std::vector<std::unique_ptr<EditorView> > _views;
};

template <class T>
void MainWindow::addView()
{
    _views.push_back(std::make_unique<T>());
}

}  // namespace atlasEditor

#endif  // WINDOW_H
