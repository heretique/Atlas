#include "MainWindow.h"

#include "Assets/Material.h"
#include "Components/Camera.h"
#include "Components/MaterialComponent.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "Utils/DebugDraw.h"
#include "Core/Engine.h"
#include "Managers/AssetManager.h"
#include "Managers/InputManager.h"
#include "Managers/ECSManager.h"
#include "Systems/PickingSystem.h"
#include "Hq/Math/Math.h"
#include "Hq/JobManager.h"
#include "Hq/Rng.h"
#include "Hq/Math/Mat4x4.h"
#include "Hq/Math/Vec3.h"
#include "Hq/Math/Quat.h"
#include "Hq/JsonSerializer.h"
#include "Hq/PackUtils.h"
#include <bx/math.h>

#include <entt/entity/registry.hpp>
#include <spdlog/spdlog.h>

#include "fs_axes.bin.h"
#include "vs_axes.bin.h"
#include <imgui/imgui.h>

using namespace hq;
using namespace hq::math;

namespace atlas
{
void testJob(void* data, uint count)
{
    float* testData = static_cast<float*>(data);
    for (uint i = 0; i < count; ++i)
        testData[i] = math::sin(float(i) / count) + math::cos(float(i) / count);
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

    static bgfx::VertexLayout ms_decl;
};

bgfx::VertexLayout PosColorVertex::ms_decl;

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
    bgfx::destroy(_axesIbh);
    bgfx::destroy(_axesVbh);
    bgfx::destroy(_axesProgram);
}

void MainWindow::onInit()
{
    std::string basePath = SDL_GetBasePath();
    Engine::init(windowSize().width, windowSize().height);
    const bgfx::Caps* caps = Engine::bgfxCaps();
    (void)caps;

    Engine::assets().setAssetsDir(basePath);
    Engine::assets().addAsset(AssetTypes::Geometry, "assets/models/DefaultCube");
    Engine::assets().addAsset(AssetTypes::Geometry, "assets/models/DefaultFullscreenTriangle");
    Engine::assets().addAsset(AssetTypes::Geometry, "assets/models/DefaultTriangle");
    Engine::assets().addAsset(AssetTypes::Geometry, "assets/models/DefaultQuad");
    AssetPtr object   = Engine::assets().addAsset(AssetTypes::Geometry, "assets/models/caruta.obj");
    AssetPtr material = Engine::assets().addAsset(AssetTypes::Material, "assets/materials/unlit_textured.material");
    Engine::assets().loadAssets();

    std::srand((unsigned int)std::time(nullptr));

    PosColorVertex::init();
    bgfx::ShaderHandle axesVsh = bgfx::createShader(bgfx::makeRef(vs_axes_bin_h, sizeof(vs_axes_bin_h)));
    bgfx::ShaderHandle axesFsh = bgfx::createShader(bgfx::makeRef(fs_axes_bin_h, sizeof(fs_axes_bin_h)));
    // Create program from shaders.
    _axesProgram = bgfx::createProgram(axesVsh, axesFsh, true);

    // Create static vertex buffer.
    _axesVbh = bgfx::createVertexBuffer(bgfx::makeRef(s_axesVertices, sizeof(s_axesVertices)), PosColorVertex::ms_decl);

    // Create static index buffer.
    _axesIbh = bgfx::createIndexBuffer(bgfx::makeRef(s_axesIndices, sizeof(s_axesIndices)));

    auto& registry                  = Engine::ecs().registry();

    for (int i = 0; i < 100; ++i)
    {
        auto                entity    = registry.create();
        TransformComponent& transform = registry.emplace<TransformComponent>(entity);
        MeshComponent&      mesh      = registry.emplace<MeshComponent>(entity, object);
        MaterialComponent&  mat       = registry.emplace<MaterialComponent>(entity);

        mesh.setGeomtry(object);
        mat.setMaterial(material);
        rotateX(transform.world(), -kPiHalf + i * kDegToRad);
        rotateY(transform.world(), -kPiHalf + i * kDegToRad);
        rotateZ(transform.world(), -kPiHalf + i * kDegToRad);
        translate(transform.world(), 30 * hq::randMinus11(), 30 * hq::randMinus11(), 30 * hq::randMinus11());
    }
}

void MainWindow::onUpdate(float dt)
{
    //    EASY_FUNCTION(profiler::colors::Amber);

    Camera& cameraComponent = Engine::ecs().registry().get<Camera>(Engine::ecs().mainCamera());

    Mat4x4 trans {Mat4x4::Identity};

    Vec3 forward(0.f, 0.f, 1.f);
    Vec3 right(1.f, 0.f, 0.f);
    Vec3 up(0.f, 1.f, 0.f);
    Vec3 translation;
    Quat rotation;

    Quat rotateHoriz = createFromAxisAngle(-up, dt * Engine::input().mouseHorizontalAxis());
    Quat rotateVert  = createFromAxisAngle(-right, dt * Engine::input().mouseVerticalAxis());
    rotation         = rotateHoriz * rotateVert;
    translation      = translation - forward * (5 * dt * Engine::input().verticalAxis());
    translation      = translation - right * (5 * dt * Engine::input().horizontalAxis());

    rotate(trans, rotation);
    translate(trans, translation);
    Mat4x4 transf;
    mul(cameraComponent.getInverseViewMatrix(), trans, transf);
    cameraComponent.setTransform(transf);
    bgfx::setViewTransform(0, cameraComponent.getViewMatrix().data, cameraComponent.getProjectionMatrix().data);
    Engine::ecs().runUpdateSystems(Engine::ecs().registry(), dt);
    renderAxes();
    render(dt);
}

void MainWindow::onGUI()
{
    //    EASY_FUNCTION(profiler::colors::Amber);
    const bgfx::Stats* stats      = Engine::bgfxStats();
    const double       toMsCpu    = 1000.0 / stats->cpuTimerFreq;
    const double       toMsGpu    = 1000.0 / stats->gpuTimerFreq;
    const double       frameMs    = double(stats->cpuTimeFrame) * toMsCpu;
    static bool        windowOpen = true;
    if (ImGui::Begin("Info", &windowOpen))
    {
        ImGui::Text("Frame %0.3f [ms], %0.3f FPS", frameMs, 1000.0 / frameMs);

        ImGui::Text("Submit CPU %0.3f, GPU %0.3f (L: %d)", double(stats->cpuTimeEnd - stats->cpuTimeBegin) * toMsCpu,
                    double(stats->gpuTimeEnd - stats->gpuTimeBegin) * toMsGpu, stats->maxGpuLatency);
        ImGui::Separator();
        ImGui::Text("Horizontal Axis: %0.1f", Engine::input().horizontalAxis());
        ImGui::Text("Vertical Axis: %0.1f", Engine::input().verticalAxis());
        ImGui::Text("Mouse Horizontal Axis: %0.1f", Engine::input().mouseHorizontalAxis());
        ImGui::Text("Mouse Vertical Axis: %0.1f", Engine::input().mouseVerticalAxis());
    }
    ImGui::End();

    Engine::input().resetInput();
}

void MainWindow::onInputEvent(const SDL_Event& e)
{
    Engine::input().handleInputEvent(e);
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
    Engine::ecs().runVisualSystems(Engine::ecs().registry(), dt);
}

}  // namespace atlas
