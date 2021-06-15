#include "MainWindow.h"

#include "Materials/Material.h"
#include "Camera/Camera.h"
#include "Core/Component.h"
#include "Materials/MaterialComponent.h"
#include "Geometry/MeshComponent.h"
#include "Core/TransformComponent.h"
#include "DebugDraw/DebugDraw.h"
#include "Engine/Engine.h"
#include "Assets/AssetManager.h"
#include "Input/InputManager.h"
#include "Ecs/ECSManager.h"
#include "UI/UIManager.h"
#include "Picking/PickingSystem.h"
#include "Hq/Math/Math.h"
#include "Hq/JobManager.h"
#include "Hq/Rng.h"
#include "Hq/Math/Mat4x4.h"
#include "Hq/Math/Vec3.h"
#include "Hq/Math/Quat.h"
#include "Hq/JsonSerializer.h"
#include "Hq/PackUtils.h"
#include "UI/ImGuiSerializer.h"
#include "UI/InfoWindow.h"
#include "UI/InspectorWindow.h"
#include "rttr/property.h"

#include <bx/math.h>

#include <entt/entity/registry.hpp>

#include "fs_axes.bin.h"
#include "vs_axes.bin.h"
#include <imgui.h>

#include <sstream>
#include <ctime>

using namespace hq;
using namespace hq::math;
namespace ui = ImGui;

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
        bgfx_vertex_layout_begin(&ms_decl, bgfx_get_renderer_type());
        bgfx_vertex_layout_add(&ms_decl, BGFX_ATTRIB_POSITION, 3, BGFX_ATTRIB_TYPE_FLOAT, false, false);
        bgfx_vertex_layout_add(&ms_decl, BGFX_ATTRIB_COLOR0, 4, BGFX_ATTRIB_TYPE_UINT8, true, false);
        bgfx_vertex_layout_end(&ms_decl);
    }

    static bgfx_vertex_layout_t ms_decl;
    ;
};

bgfx_vertex_layout_t PosColorVertex::ms_decl;

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

static entt::entity sMainCamera = entt::null;

MainWindow::MainWindow(const char* title, int x, int y, int w, int h)
    : SDLWindow(title, x, y, w, h)
{
}

MainWindow::~MainWindow()
{
    bgfx_destroy_index_buffer(_axesIbh);
    bgfx_destroy_vertex_buffer(_axesVbh);
    bgfx_destroy_program(_axesProgram);
    Engine::release();
}

void MainWindow::onInit()
{
    std::string basePath = SDL_GetBasePath();
    Engine::init(windowSize().width, windowSize().height);
    const bgfx_caps_t* caps = Engine::bgfxCaps();
    (void)caps;

    Engine::assets().setAssetsDir(basePath);
    Engine::assets().addAsset(AssetTypes::Geometry, "assets/models/DefaultCube");
    Engine::assets().addAsset(AssetTypes::Geometry, "assets/models/DefaultFullscreenTriangle");
    Engine::assets().addAsset(AssetTypes::Geometry, "assets/models/DefaultTriangle");
    Engine::assets().addAsset(AssetTypes::Geometry, "assets/models/DefaultQuad");
    AssetPtr object   = Engine::assets().addAsset(AssetTypes::Geometry, "assets/models/caruta.obj");
    AssetPtr material = Engine::assets().addAsset(AssetTypes::Material, "assets/materials/unlit_textured.material");
    Engine::assets().loadAssets();
    Engine::ui().addWindow(new InfoWindow("Info", 0));
    Engine::ui().addWindow(new InspectorWindow("Inspector", 0));

    std::srand((unsigned int)std::time(nullptr));

    PosColorVertex::init();
    bgfx_shader_handle_t axesVsh = bgfx_create_shader(bgfx_make_ref(vs_axes_bin_h, sizeof(vs_axes_bin_h)));
    bgfx_shader_handle_t axesFsh = bgfx_create_shader(bgfx_make_ref(fs_axes_bin_h, sizeof(fs_axes_bin_h)));
    // Create program from shaders.
    _axesProgram = bgfx_create_program(axesVsh, axesFsh, true);

    // Create static vertex buffer.
    _axesVbh = bgfx_create_vertex_buffer(bgfx_make_ref(s_axesVertices, sizeof(s_axesVertices)),
                                         &PosColorVertex::ms_decl, BGFX_BUFFER_NONE);

    // Create static index buffer.
    _axesIbh = bgfx_create_index_buffer(bgfx_make_ref(s_axesIndices, sizeof(s_axesIndices)), false);

    auto& registry = Engine::ecs().registry();

    using namespace hq::math;
    sMainCamera             = registry.create();
    Camera& cameraComponent = registry.emplace<Camera>(sMainCamera);
    cameraComponent.setPerspective(60.f, float(Engine::viewWidth()) / Engine().viewHeight(), .1f, 1000.f);
    Mat4x4 cameraView {Mat4x4::Identity};
    createLookAt(Vec3(5, 5, 10), Vec3::Zero, Vec3(0.f, 1.f, 0.f), cameraView);
    invert(cameraView);
    cameraComponent.setTransform(cameraView);

    auto pickingSystem = std::make_unique<PickingSystem>();
    pickingSystem->setCamera(sMainCamera);
    Engine::ecs().registerUpdateSystem(std::move(pickingSystem));

    for (int i = 0; i < 1000; ++i)
    {
        auto                entity    = registry.create();
        TransformComponent& transform = registry.emplace<TransformComponent>(entity);
        MeshComponent& mesh = registry.emplace<MeshComponent>(entity, std::dynamic_pointer_cast<GeometryAsset>(object));
        MaterialComponent& mat = registry.emplace<MaterialComponent>(entity);

        mat.material = std::dynamic_pointer_cast<MaterialAsset>(material);
        rotateX(transform.world, -kPiHalf + i * kDegToRad);
        rotateY(transform.world, -kPiHalf + i * kDegToRad);
        rotateZ(transform.world, -kPiHalf + i * kDegToRad);
        translate(transform.world, 30 * hq::randMinus11(), 30 * hq::randMinus11(), 30 * hq::randMinus11());
    }
}

void MainWindow::onUpdate(float dt)
{
    //    EASY_FUNCTION(profiler::colors::Amber);

    Camera& cameraComponent = Engine::ecs().registry().get<Camera>(sMainCamera);

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
    bgfx_set_view_transform(0, cameraComponent.getViewMatrix().data, cameraComponent.getProjectionMatrix().data);
    Engine::ecs().runUpdateSystems(Engine::ecs().registry(), dt);
    renderAxes();
    render(dt);
    Engine::ui().update(dt);
    Engine::input().resetInput();
}

void MainWindow::onInputEvent(const SDL_Event& e)
{
    Engine::input().handleInputEvent(e);
}

void MainWindow::renderAxes()
{
    bgfx_set_vertex_buffer(0, _axesVbh, 0, UINT32_MAX);
    bgfx_set_index_buffer(_axesIbh, 0, UINT32_MAX);
    bgfx_set_state(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                       BGFX_STATE_PT_LINES,
                   0);
    bgfx_submit(0, _axesProgram, 0, BGFX_DISCARD_ALL);
}

void MainWindow::render(float dt)
{
    Engine::ecs().runVisualSystems(Engine::ecs().registry(), dt);
}

}  // namespace atlas
