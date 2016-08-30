#ifndef WINDOW_H
#define WINDOW_H

#include <Base.h>
#include <QBGFXWindow.h>
#include <EditorViews.h>

struct ImDrawData;


namespace atlasEditor {

class MainWindow : public QBGFXWindow
{
public:
    MainWindow();
    ~MainWindow();
    template <class T>
    void addView();

protected:
    void initializeBGFX() Q_DECL_OVERRIDE;
    void releaseBGFX() Q_DECL_OVERRIDE;
    void resizeBGFX(const QSize &size) Q_DECL_OVERRIDE;
    void paintBGFX() Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

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
