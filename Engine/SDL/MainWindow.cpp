#include "MainWindow.h"

#include "Assets/Material.h"
#include "Components/Camera.h"
#include "Components/MaterialComponent.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "Core/Engine.h"
#include "Managers/AssetManager.h"
#include "Managers/JobManager.h"
#include "Math/Utils.h"
#include <cmath>
#include <entt/entity/registry.hpp>
#include <spdlog/spdlog.h>

//#include <easy/profiler.h>
#include "fs_axes.bin.h"
#include "vs_axes.bin.h"
#include <imgui/imgui.h>

namespace atlas
{
void testJob(void* data, uint count)
{
    float* testData = static_cast<float*>(data);
    for (uint i     = 0; i < count; ++i)
        testData[i] = std::sin((float)i / count) + std::cos((float)i / count);
}

struct PosColorVertex
{
    float    m_x;
    float    m_y;
    float    m_z;
    uint32_t m_abgr;

    static void init()
    {
        ms_decl.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    }

    static bgfx::VertexDecl ms_decl;
};

bgfx::VertexDecl PosColorVertex::ms_decl;

static PosColorVertex s_axesVertices[] = {
    {0.f, 0.f, 0.f, 0xff0000ff}, {1.f, 0.f, 0.f, 0xff0000ff},  // x axis
    {0.f, 0.f, 0.f, 0xff00ff00}, {0.f, 1.f, 0.f, 0xff00ff00},  // y axis
    {0.f, 0.f, 0.f, 0xffff0000}, {0.f, 0.f, 1.f, 0xffff0000},  // z axis
};

static const uint16_t s_axesIndices[] = {
    0, 1,  // x axis
    2, 3,  // y axis
    4, 5,  // z axis
};

MainWindow::MainWindow(const char* title, int x, int y, int w, int h)
    : SDLWindow(title, x, y, w, h)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::init()
{
    std::string basePath = SDL_GetBasePath();
    Engine::init();
    Engine::assets().setAssetsDir(basePath);
    AssetPtr object   = Engine::assets().addAsset(AssetTypes::Geometry, "assets/caruta.obj");
    AssetPtr material = Engine::assets().addAsset(AssetTypes::Material, "assets/unlit_textured.material");
    Engine::assets().loadAssets();
    std::srand(std::time(NULL));

    PosColorVertex::init();
    bgfx::ShaderHandle axesVsh = bgfx::createShader(bgfx::makeRef(vs_axes_bin_h, sizeof(vs_axes_bin_h)));
    bgfx::ShaderHandle axesFsh = bgfx::createShader(bgfx::makeRef(fs_axes_bin_h, sizeof(fs_axes_bin_h)));
    // Create program from shaders.
    _axesProgram = bgfx::createProgram(axesVsh, axesFsh, true);

    // Create static vertex buffer.
    _axesVbh = bgfx::createVertexBuffer(bgfx::makeRef(s_axesVertices, sizeof(s_axesVertices)), PosColorVertex::ms_decl);

    // Create static index buffer.
    _axesIbh = bgfx::createIndexBuffer(bgfx::makeRef(s_axesIndices, sizeof(s_axesIndices)));

    auto&           registry        = Engine::ecs();
    auto            camera          = registry.create();
    Camera&         cameraComponent = registry.assign<Camera>(entt::tag_t{}, camera);
    SDLWindow::Size size            = windowSize();
    cameraComponent.setPerspective(60.f, (float)size.width / size.height, .1f, 100.f);
    TransformComponent& transform = registry.assign<TransformComponent>(camera);
    math::Matrix::createLookAt(math::Vector3(5, 5, 10), math::Vector3::zero(), math::Vector3::unitY(),
                               &transform.world());
    cameraComponent.setTransform(transform.world());

    for (int i = 0; i < 1000; ++i)
    {
        auto                entity    = registry.create();
        TransformComponent& transform = registry.assign<TransformComponent>(entity);
        MeshComponent&      mesh      = registry.assign<MeshComponent>(entity);
        MaterialComponent&  mat       = registry.assign<MaterialComponent>(entity);

        mesh.setGeomtry(object);
        mat.setMaterial(material);
        transform.world().translate(10 * MATH_RANDOM_MINUS1_1(), 10 * MATH_RANDOM_MINUS1_1(),
                                    10 * MATH_RANDOM_MINUS1_1());
        transform.world().rotateX(-M_PI_2);
    }
}

void MainWindow::update(float dt)
{
    //    EASY_FUNCTION(profiler::colors::Amber);

    const Camera& cameraComponent = Engine::ecs().get<Camera>();
    bgfx::setViewTransform(0, cameraComponent.getViewMatrix().m, cameraComponent.getProjectionMatrix().m);
    renderAxes();
    render(dt);
}

void MainWindow::onGUI()
{
    //    EASY_FUNCTION(profiler::colors::Amber);
    const bgfx::Stats* stats   = Engine::bgfxStats();
    const double       toMsCpu = 1000.0 / stats->cpuTimerFreq;
    const double       toMsGpu = 1000.0 / stats->gpuTimerFreq;
    const double       frameMs = double(stats->cpuTimeFrame) * toMsCpu;
    ImGui::Text("Frame %0.3f [ms], %0.3f FPS", frameMs, 1000.0 / frameMs);

    ImGui::Text("Submit CPU %0.3f, GPU %0.3f (L: %d)", double(stats->cpuTimeEnd - stats->cpuTimeBegin) * toMsCpu,
                double(stats->gpuTimeEnd - stats->gpuTimeBegin) * toMsGpu, stats->maxGpuLatency);
}

void MainWindow::renderAxes()
{
    bgfx::setVertexBuffer(0, _axesVbh);
    bgfx::setIndexBuffer(_axesIbh);
    bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                   BGFX_STATE_PT_LINES);
    bgfx::submit(0, _axesProgram);
}

void MainWindow::render(float dt)
{
    auto view = Engine::ecs().view<TransformComponent, MeshComponent, MaterialComponent>();

    for (auto entity : view)
    {
        TransformComponent& transform = view.get<TransformComponent>(entity);
        MeshComponent&      mesh      = view.get<MeshComponent>(entity);
        MaterialComponent&  material  = view.get<MaterialComponent>(entity);

        bgfx::setTransform(transform.world().m);
        bgfx::setVertexBuffer(0, mesh.geometry()->vbo());
        bgfx::setIndexBuffer(mesh.geometry()->ibo());
        material.material()->bind();
        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(0, material.material()->program());
    }
}

}  // namespace atlas
