#include "MainWindow.h"
#include <Engine.h>
#include <Managers/JobManager.h>
#include <bgfx/embedded_shader.h>
#include <bx/math.h>
#include <bx/timer.h>
#include <bx/uint32_t.h>
#include <easy/profiler.h>
#include <imgui/imgui.h>
#include <iostream>
#include <ratio>

#include "fs_drawstress.bin.h"
#include "vs_drawstress.bin.h"

namespace atlasEditor {
using namespace std::chrono;
const int MAX_TASKS = 64 * 1024;

static const bgfx::EmbeddedShader s_embeddedShaders[] = {
    BGFX_EMBEDDED_SHADER(vs_drawstress), BGFX_EMBEDDED_SHADER(fs_drawstress),

    BGFX_EMBEDDED_SHADER_END()};

struct JobData {
  float mtxR[16];
  float mtx[16];
  uint32_t xx;
  uint32_t yy;
  uint32_t zz;
};

struct PosColorVertex {
  float m_x;
  float m_y;
  float m_z;
  uint32_t m_abgr;

  static void init() {
    ms_decl.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
  }

  static bgfx::VertexDecl ms_decl;
};

bgfx::VertexDecl PosColorVertex::ms_decl;

static PosColorVertex s_cubeVertices[8] = {
    {-1.0f, 1.0f, 1.0f, 0xff000000},   {1.0f, 1.0f, 1.0f, 0xff0000ff},
    {-1.0f, -1.0f, 1.0f, 0xff00ff00},  {1.0f, -1.0f, 1.0f, 0xff00ffff},
    {-1.0f, 1.0f, -1.0f, 0xffff0000},  {1.0f, 1.0f, -1.0f, 0xffff00ff},
    {-1.0f, -1.0f, -1.0f, 0xffffff00}, {1.0f, -1.0f, -1.0f, 0xffffffff},
};

static const uint16_t s_cubeIndices[36] = {
    0, 1, 2,          // 0
    1, 3, 2, 4, 6, 5, // 2
    5, 6, 7, 0, 2, 4, // 4
    4, 2, 6, 1, 5, 3, // 6
    5, 7, 3, 0, 4, 1, // 8
    4, 5, 1, 2, 3, 6, // 10
    6, 3, 7,
};

MainWindow::MainWindow(const char *title, int x, int y, int w, int h)
    : SDLWindow(title, x, y, w, h) {}

MainWindow::~MainWindow() {
  delete[] m_jobs;
  bgfx::destroy(m_ibh);
  bgfx::destroy(m_vbh);
  bgfx::destroy(m_program);
}

void MainWindow::init() {
  atlas::Engine::init();
  m_autoAdjust = false;
  m_dim = 6;
  m_maxDim = 32;
  m_transform = 0;
  m_deltaTimeNs = duration<double>::zero();
  m_deltaTimeAvgNs = duration<double>::zero();
  m_numFrames = 0;
  m_timeOffset = high_resolution_clock::now();
  // Create vertex stream declaration.
  PosColorVertex::init();

  bgfx::RendererType::Enum type = bgfx::getRendererType();

  // Create program from shaders.
  m_program = bgfx::createProgram(
      bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_drawstress"),
      bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_drawstress"),
      true /* destroy shaders when program is destroyed */
  );

  // Create static vertex buffer.
  m_vbh = bgfx::createVertexBuffer(
      bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
      PosColorVertex::ms_decl);

  // Create static index buffer.
  m_ibh = bgfx::createIndexBuffer(
      bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices)));

  const bgfx::Caps *caps = bgfx::getCaps();
  m_maxDim = (int32_t)powf(float(caps->limits.maxDrawCalls), 1.0f / 3.0f);
  m_jobs = new JobData[MAX_TASKS];
}

void MainWindow::update(float dt) {
  using namespace atlas;

  //  return;
  EASY_FUNCTION(profiler::colors::Magenta);

  high_resolution_clock::time_point now = high_resolution_clock::now();

  static high_resolution_clock::time_point last = now;
  const duration<double> frameTime =
      duration_cast<duration<double>>(now - last);
  last = now;

  const int64_t hpFreq = bx::getHPFrequency();
  const double freq = double(hpFreq);
  const double toMs = 1000.0 / freq;

  m_deltaTimeNs += frameTime;

  if (m_deltaTimeNs > std::chrono::seconds(1)) {
    m_deltaTimeAvgNs = m_deltaTimeNs / bx::int64_max(1, m_numFrames);
    //    std::cout << "DeltaAvg: " << m_deltaTimeAvgNs.count() << "\n";
    if (m_autoAdjust) {
      if (m_deltaTimeAvgNs <
          duration_cast<duration<double>>(std::chrono::milliseconds(16))) {
        m_dim = bx::uint32_min(m_dim + 1, m_maxDim);
      } else if (m_deltaTimeAvgNs > duration_cast<duration<double>>(
                                        std::chrono::milliseconds(17))) {
        m_dim = bx::uint32_max(m_dim - 1, 2);
      }
    }

    m_deltaTimeNs = duration<double>::zero();
    m_numFrames = 0;
  } else {
    ++m_numFrames;
  }

  float time = duration_cast<duration<float>>(now - m_timeOffset).count();

  float at[3] = {0.0f, 0.0f, 0.0f};
  float eye[3] = {0.0f, 0.0f, -35.0f};

  float view[16];
  float proj[16];
  bx::mtxLookAt(view, eye, at);
  const bgfx::Caps *caps = bgfx::getCaps();
  SDLWindow::Size wSize = windowSize();
  bx::mtxProj(proj, 60.0f, float(wSize.width) / float(wSize.height), 0.1f,
              100.0f, caps->homogeneousDepth);

  // Set view and projection matrix for view 0.
  bgfx::setViewTransform(0, view, proj);

  // Use debug font to print information about this example.
  bgfx::dbgTextClear();
  bgfx::dbgTextPrintf(0, 1, 0x4f, "bgfx/examples/17-drawstress");
  bgfx::dbgTextPrintf(
      0, 2, 0x6f, "Description: Draw stress, maximizing number of draw calls.");
  bgfx::dbgTextPrintf(0, 3, 0x4f, "Frame: %7.3f[ms]", frameTime.count());
  bgfx::dbgTextPrintf(0, 4, 0x4f, "Dim: %d/%d", m_dim, m_maxDim);
  bgfx::dbgTextPrintf(0, 5, 0x4f, "Draw calls: %d", m_dim * m_dim * m_dim);
  bgfx::dbgTextPrintf(0, 6, 0x4f, "Avg Delta Time (1 second) [ms]: %0.4f",
                      m_deltaTimeAvgNs / 1000.0f);

  const bgfx::Stats *stats = bgfx::getStats();
  bgfx::dbgTextPrintf(0, 7, 0x4f, "GPU %0.6f [ms]",
                      double(stats->gpuTimeEnd - stats->gpuTimeBegin) * 1000.0 /
                          stats->gpuTimerFreq);
  bgfx::dbgTextPrintf(0, 8, 0x4f, "CPU %0.6f [ms]",
                      double(stats->cpuTimeEnd - stats->cpuTimeBegin) * 1000.0 /
                          stats->cpuTimerFreq);
  bgfx::dbgTextPrintf(0, 9, 0x4f, "Waiting for render thread %0.6f [ms] ",
                      double(stats->waitRender) * toMs);
  bgfx::dbgTextPrintf(0, 10, 0x4f, "Waiting for submit thread %0.6f [ms] ",
                      double(stats->waitSubmit) * toMs);

  float mtxS[16];
  const float scale = 0 == m_transform ? 0.25f : 0.0f;
  bx::mtxScale(mtxS, scale, scale, scale);

  const float step = 0.6f;
  float pos[3];
  pos[0] = -step * m_dim / 2.0f;
  pos[1] = -step * m_dim / 2.0f;
  pos[2] = -15.0;
  uint taskCount = 0;

  for (uint32_t zz = 0; zz < uint32_t(m_dim); ++zz) {
    for (uint32_t yy = 0; yy < uint32_t(m_dim); ++yy) {
      for (uint32_t xx = 0; xx < uint32_t(m_dim); ++xx) {
        JobData &task = m_jobs[taskCount];
        task.xx = xx;
        task.yy = yy;
        task.zz = zz;
        taskCount++;
      }
    }
  }

  std::function<void(void *, uint)> jobFunc = [&](void *data, uint count) {
    JobData *tasks = static_cast<JobData *>(data);
    for (uint i = 0; i < count; ++i) {
      JobData &task = tasks[i];
      bx::mtxRotateXYZ(task.mtxR, time + task.xx * 0.21f,
                       time + task.yy * 0.37f, time + task.yy * 0.13f);
      bx::mtxMul(task.mtx, mtxS, task.mtxR);

      task.mtx[12] = pos[0] + float(task.xx) * step;
      task.mtx[13] = pos[1] + float(task.yy) * step;
      task.mtx[14] = pos[2] + float(task.zz) * step;
    }
  };

  Engine::jobMan().parallel_for<float, atlas::CountSplitter<float, 1024>>(
      jobFunc, m_jobs, taskCount);
  Engine::jobMan().wait();

  for (uint i = 0; i < taskCount; ++i) {
    JobData &task = m_jobs[i];
    // Set model matrix for rendering.
    bgfx::setTransform(task.mtx);

    // Set vertex and index buffer.
    bgfx::setVertexBuffer(0, m_vbh);
    bgfx::setIndexBuffer(m_ibh);

    // Set render states.
    bgfx::setState(BGFX_STATE_DEFAULT);

    // Submit primitive for rendering to view 0.
    bgfx::submit(0, m_program);
  }
} // namespace atlasEditor

void MainWindow::onGUI() {}

} // namespace atlasEditor
