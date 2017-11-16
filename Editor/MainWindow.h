#ifndef WINDOW_H
#define WINDOW_H

#include <EditorViews.h>
#include <SDLWindow.h>
#include <chrono>
#include <memory>
#include <vector>

namespace atlasEditor {

struct JobData;

class MainWindow : public SDLWindow {
public:
  MainWindow(const char *title, int x, int y, int w, int h);
  ~MainWindow();
  template <class T> void addView();

public:
  void init() override;
  void update(float dt) override;
  void onGUI() override;

private:
  std::vector<std::unique_ptr<EditorView>> _views;

private:
  bool m_open{true};
  bool m_autoAdjust{true};
  int32_t m_dim;
  int32_t m_maxDim;
  uint32_t m_transform;

  std::chrono::high_resolution_clock::time_point m_timeOffset;

  std::chrono::duration<double> m_deltaTimeNs;
  std::chrono::duration<double> m_deltaTimeAvgNs;
  int64_t m_numFrames;

  bgfx::ProgramHandle m_program;
  bgfx::VertexBufferHandle m_vbh;
  bgfx::IndexBufferHandle m_ibh;
  JobData *m_jobs{nullptr};
};

template <class T> void MainWindow::addView() {
  _views.push_back(std::make_unique<T>());
}

} // namespace atlasEditor

#endif // WINDOW_H
