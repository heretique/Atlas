#include "MainWindow.h"

#include "Assets/Material.h"
#include "Components/Camera.h"
#include "Components/MaterialComponent.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "Core/Engine.h"
#include "Managers/AssetManager.h"
#include "Managers/InputManager.h"
#include "Hq/Math/Math.h"
#include "Hq/JobManager.h"
#include "Hq/Rng.h"
#include "Hq/Math/Mat4x4.h"
#include "Hq/Math/Vec3.h"
#include "Hq/Math/Quat.h"

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
        testData[i] = math::sin((float)i / count) + math::cos((float)i / count);
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

MainWindow::~MainWindow() {}

void MainWindow::onInit()
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

    auto& registry                  = Engine::ecs();
    _camera                         = registry.create();
    Camera&         cameraComponent = registry.assign<Camera>(_camera);
    SDLWindow::Size size            = windowSize();
    cameraComponent.setPerspective(60.f, (float)size.width / size.height, .1f, 1000.f);
    TransformComponent& transform = registry.assign<TransformComponent>(_camera);
    createLookAt(Vec3(5, 5, 10), Vec3::Zero, Vec3(0.f, 1.f, 0.f), transform.world());
    cameraComponent.setTransform(transform.world());

    for (int i = 0; i < 500; ++i)
    {
        auto                entity    = registry.create();
        TransformComponent& transform = registry.assign<TransformComponent>(entity);
        MeshComponent&      mesh      = registry.assign<MeshComponent>(entity);
        MaterialComponent&  mat       = registry.assign<MaterialComponent>(entity);

        mesh.setGeomtry(object);
        mat.setMaterial(material);
        translate(transform.world(), 10 * hq::randMinus11(), 10 * hq::randMinus11(), 10 * hq::randMinus11());
        rotateX(transform.world(), -kPiHalf + i * kDegToRad);
        rotateY(transform.world(), -kPiHalf + i * kDegToRad);
        rotateZ(transform.world(), -kPiHalf + i * kDegToRad);
    }
}

void MainWindow::onUpdate(float dt)
{
    //    EASY_FUNCTION(profiler::colors::Amber);

    Camera& cameraComponent = Engine::ecs().get<Camera>(_camera);

    Mat4x4 trans {Mat4x4::Identity};
    Vec3   forward(0.f, 0.f, 1.f);
    Vec3   right(1.f, 0.f, 0.f);
    Vec3   up(0.f, 1.f, 0.f);
    Vec3   translation;
    Quat   rotation;
    //    decompose(cameraComponent.getViewMatrix(), rotation, scale, translation);
    Quat rotateHoriz = createFromAxisAngle(up, dt * Engine::input().mouseHorizontalAxis());
    Quat rotateVert  = createFromAxisAngle(right, dt * Engine::input().mouseVerticalAxis());
    rotation         = mul(rotateHoriz, rotateVert);
    translation      = add(translation, mul(forward, 5 * dt * Engine::input().verticalAxis()));
    translation      = add(translation, mul(right, 5 * dt * Engine::input().horizontalAxis()));
    rotate(trans, rotation);
    translate(trans, translation);
    mul(trans, cameraComponent.getViewMatrix());
    cameraComponent.setTransform(trans);
    bgfx::setViewTransform(0, cameraComponent.getViewMatrix().data, cameraComponent.getProjectionMatrix().data);
    renderAxes();
    render(dt);
    Engine::input().resetInput();
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
    ImGui::Separator();
    ImGui::Text("Horizontal Axis: %0.1f", Engine::input().horizontalAxis());
    ImGui::Text("Vertical Axis: %0.1f", Engine::input().verticalAxis());
    ImGui::Text("Mouse Horizontal Axis: %0.1f", Engine::input().mouseHorizontalAxis());
    ImGui::Text("Mouse Vertical Axis: %0.1f", Engine::input().mouseVerticalAxis());
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
    auto view = Engine::ecs().view<TransformComponent, MeshComponent, MaterialComponent>();

    for (auto entity : view)
    {
        TransformComponent& transform = view.get<TransformComponent>(entity);
        MeshComponent&      mesh      = view.get<MeshComponent>(entity);
        MaterialComponent&  material  = view.get<MaterialComponent>(entity);

        Vec3 forward(0.f, 0.f, 1.f);
        Vec3 right(1.f, 0.f, 0.f);
        Vec3 up(0.f, 1.f, 0.f);

        Quat rotZ = createFromAxisAngle(forward, 10 * kDegToRad * dt);
        Quat rotY = createFromAxisAngle(up, 10 * kDegToRad * dt);
        Quat rotX = createFromAxisAngle(right, 10 * kDegToRad * dt);

        Mat4x4 rot;
        createRotation(mul(mul(rotZ, rotY), rotX), rot);
        mul(transform.world(), rot);

        bgfx::setTransform(transform.world().data);
        bgfx::setVertexBuffer(0, mesh.geometry()->vbo());
        bgfx::setIndexBuffer(mesh.geometry()->ibo());
        material.material()->bind();
        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(0, material.material()->program());
    }
}

}  // namespace atlas
