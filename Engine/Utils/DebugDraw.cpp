#include "DebugDraw.h"
#include "Core/Engine.h"
#include "Managers/AssetManager.h"
#include "Assets/Material.h"
#include <bgfx/bgfx.h>
#include <Hq/Math/Box3.h>

namespace atlas
{
// mesh vertex declaration
struct DebugLineVertex
{
    float    x {0.f};
    float    y {0.f};
    float    z {0.f};
    float    u {0.f};
    uint32_t color {0};

    DebugLineVertex() {}
    DebugLineVertex(const hq::math::Vec3& point, const float u, const uint32_t color)
        : x(point.x)
        , y(point.y)
        , z(point.z)
        , u(u)
        , color(color)
    {
    }

    static void init()
    {
        vertLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 1, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    }

    static size_t size()
    {
        return sizeof(DebugLineVertex);
    }

    static bgfx::VertexLayout vertLayout;
};

static uint16_t sBox3LineIndices[] = {
    0, 1,
    1, 2,
    2, 3,
    3, 0,
    4, 5,
    5, 6,
    6, 7,
    7, 4,
    0, 7,
    1, 6,
    2, 5,
    3, 4
};

bgfx::VertexLayout DebugLineVertex::vertLayout;

struct DebugDrawImpl
{
    std::shared_ptr<MaterialAsset> _debugLineMat;
    std::shared_ptr<MaterialAsset> _debugLineStippleMat;

    std::vector<DebugLineVertex> vertexBuffer;
    std::vector<uint16_t>        indexBuffer;

    void initialize()
    {
        DebugLineVertex::init();
        _debugLineMat = std::static_pointer_cast<MaterialAsset>(Engine::assets().addAsset(AssetTypes::Material, "assets/materials/debug_line.material"));
        //    _debugLineStippleMat = Engine::assets().addAsset(AssetTypes::Material,
        //    "assets/materials/debug_line_stipple.material");
        Engine::assets().loadAssets();
    }

    void begin()
    {
        vertexBuffer.clear();
        indexBuffer.clear();
    }
    void end()
    {
        bgfx::TransientVertexBuffer debugVbo;
        bgfx::TransientIndexBuffer  debugIbo;
        if (bgfx::allocTransientBuffers(&debugVbo, DebugLineVertex::vertLayout, vertexBuffer.size(), &debugIbo,
                                        indexBuffer.size()))
        {
            memcpy(debugVbo.data, vertexBuffer.data(), vertexBuffer.size() * DebugLineVertex::vertLayout.m_stride);
            memcpy(debugIbo.data, indexBuffer.data(), indexBuffer.size() * sizeof(uint16_t));
            bgfx::setVertexBuffer(0, &debugVbo);
            bgfx::setIndexBuffer(&debugIbo);
            bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_PT_LINES);
            bgfx::submit(0, _debugLineMat->program());
        }
    }

    void drawBox3(const hq::math::Box3& box, const hq::math::Mat4x4& m, const uint32_t color)
    {
        using namespace hq::math;
        Vec3 corners[8];
        Box3 transformedBox;
        transform(box, m, transformedBox);

        getCorners(transformedBox, corners);
        int indexCount = sizeof (sBox3LineIndices) / sizeof (sBox3LineIndices[0]);
        size_t indexPos = vertexBuffer.size();
        for (int i = 0;  i < indexCount; ++i)
        {
            indexBuffer.emplace_back(indexPos + sBox3LineIndices[i]);
        }

        for (int i = 0; i < 8; ++i)
        {
            vertexBuffer.emplace_back(DebugLineVertex(corners[i], 0.f, color));
        }
    }
};

DebugDraw::DebugDraw()
    : _impl(std::make_unique<DebugDrawImpl>())
{
}

DebugDraw::~DebugDraw() {}

void DebugDraw::initialize()
{
    _impl->initialize();
}

void DebugDraw::begin()
{
    _impl->begin();
}

void DebugDraw::end()
{
    _impl->end();
}

void DebugDraw::drawBox3(const hq::math::Box3& box, const hq::math::Mat4x4& transform, const uint32_t color)
{
    _impl->drawBox3(box, transform, color);
}

}  // atlas namespace
