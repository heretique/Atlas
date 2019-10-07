#include "Camera.h"
#include "Hq/Math/Vec2.h"
#include "Hq/Math/Vec3.h"
#include "Hq/Math/Vec4.h"
#include "Hq/Math/Mat4x4.h"
#include "Hq/Math/Frustum.h"
#include "Hq/Math/Rect.h"
#include "Components/TransformComponent.h"

#include <cassert>

namespace atlas
{
using namespace hq::math;

Camera::Camera()
    : _cameraType(CameraType::ePerspective)
{
}

Camera::~Camera() {}

void Camera::setPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
    _cameraType  = CameraType::ePerspective;
    _fieldOfView = fieldOfView;
    _aspectRatio = aspectRatio;
    _nearPlane   = nearPlane;
    _farPlane    = farPlane;
    updateCamera();
}

void Camera::setOrthographic(float zoomX, float zoomY, float aspectRatio, float nearPlane, float farPlane)
{
    _cameraType  = CameraType::eOrthographic;
    _zoom[0]     = zoomX;
    _zoom[1]     = zoomY;
    _aspectRatio = aspectRatio;
    _nearPlane   = nearPlane;
    _farPlane    = farPlane;
    updateCamera();
}

void Camera::setTransform(const Mat4x4& transform)
{
    updateCamera(transform);
}

CameraType Camera::getCameraType() const
{
    return _cameraType;
}

float Camera::getFieldOfView() const
{
    assert(_cameraType == CameraType::ePerspective);

    return _fieldOfView;
}

void Camera::setFieldOfView(float fieldOfView)
{
    assert(_cameraType == CameraType::ePerspective);

    _fieldOfView = fieldOfView;
    updateCamera();
}

float Camera::getZoomX() const
{
    assert(_cameraType == CameraType::eOrthographic);

    return _zoom[0];
}

void Camera::setZoomX(float zoomX)
{
    assert(_cameraType == CameraType::eOrthographic);

    _zoom[0] = zoomX;
    updateCamera();
}

float Camera::getZoomY() const
{
    assert(_cameraType == CameraType::eOrthographic);

    return _zoom[1];
}

void Camera::setZoomY(float zoomY)
{
    assert(_cameraType == CameraType::eOrthographic);

    _zoom[1] = zoomY;
    updateCamera();
}

float Camera::getAspectRatio() const
{
    return _aspectRatio;
}

void Camera::setAspectRatio(float aspectRatio)
{
    _aspectRatio = aspectRatio;
    updateCamera();
}

float Camera::getNearPlane() const
{
    return _nearPlane;
}

void Camera::setNearPlane(float nearPlane)
{
    _nearPlane = nearPlane;
    updateCamera();
}

float Camera::getFarPlane() const
{
    return _farPlane;
}

void Camera::setFarPlane(float farPlane)
{
    _farPlane = farPlane;
    updateCamera();
}

const Mat4x4& Camera::getViewMatrix() const
{
    return _view;
}

const Mat4x4& Camera::getInverseViewMatrix() const
{
    return _inverseView;
}

const Mat4x4& Camera::getProjectionMatrix() const
{
    return _projection;
}

void Camera::setProjectionMatrix(const Mat4x4& Mat4x4)
{
    _projection = Mat4x4;
    updateCamera();
}

const Mat4x4& Camera::getViewProjectionMatrix() const
{
    return _viewProjection;
}

const Mat4x4& Camera::getInverseViewProjectionMatrix() const
{
    return _inverseViewProjection;
}

const Frustum& Camera::getFrustum() const
{
    return _bounds;
}

void Camera::project(const Rect& viewport, const Vec3& position, float& x, float& y, float& depth) const
{
    // Transform the point to clip-space.
    Vec4 clipPos;
    transform(Vec4(position.x, position.y, position.z, 1.0f), _viewProjection, clipPos);

    // Compute normalized device coordinates.
    assert(clipPos.w != 0.0f);
    float ndcX = clipPos.x / clipPos.w;
    float ndcY = clipPos.y / clipPos.w;

    // Compute screen coordinates by applying our viewport transformation.
    x = viewport.x + (ndcX + 1.0f) * 0.5f * viewport.width;
    y = viewport.y + (1.0f - (ndcY + 1.0f) * 0.5f) * viewport.height;

    float ndcZ = clipPos.z / clipPos.w;
    depth      = (ndcZ + 1.0f) / 2.0f;
}

void Camera::project(const Rect& viewport, const Vec3& position, Vec2& out) const
{
    float x, y, depth;
    project(viewport, position, x, y, depth);
    out.x = x;
    out.y = y;
}

void Camera::project(const Rect& viewport, const Vec3& position, Vec3& out) const
{
    float x, y, depth;
    project(viewport, position, x, y, depth);
    out.x = x;
    out.y = y;
    out.z = depth;
}

void Camera::unproject(const Rect& viewport, float x, float y, float depth, Vec3& dst) const
{
    // Create our screen space position in NDC.
    assert(viewport.width != 0.0f && viewport.height != 0.0f);
    Vec4 screen((x - viewport.x) / viewport.width, ((viewport.height - y) - viewport.y) / viewport.height, depth, 1.0f);

    // Map to range -1 to 1.
    screen.x = screen.x * 2.0f - 1.0f;
    screen.y = screen.y * 2.0f - 1.0f;
    screen.z = screen.z * 2.0f - 1.0f;

    // Transform the screen-space NDC by our inverse view projection Mat4x4.
    transform(screen, _inverseViewProjection);

    // Divide by our W coordinate.
    if (screen.w != 0.0f)
    {
        screen.x /= screen.w;
        screen.y /= screen.w;
        screen.z /= screen.w;
    }

    dst.x = screen.x;
    dst.y = screen.y;
    dst.z = screen.z;
}

void Camera::pickRay(const Rect& viewport, float x, float y, Ray3& dst) const
{
    // Get the world-space position at the near clip plane.
    Vec3 nearPoint;
    unproject(viewport, x, y, 0.0f, nearPoint);

    // Get the world-space position at the far clip plane.
    Vec3 farPoint;
    unproject(viewport, x, y, 1.0f, farPoint);

    // Set the direction of the ray.
    Vec3 direction;
    sub(farPoint, nearPoint, direction);
    normalize(direction);

    dst.origin    = nearPoint;
    dst.direction = direction;
}

void Camera::updateCamera(const Mat4x4& transform)
{
    _view = transform;
    invert(_view, _inverseView);

    if (_cameraType == CameraType::ePerspective)
    {
        createPerspective(_fieldOfView, _aspectRatio, _nearPlane, _farPlane, _projection);
    }
    else
    {
        // Create an ortho projection with the origin at the bottom left of the viewport, +X to the right and +Y
        createOrthographic(_zoom[0], _zoom[1], _nearPlane, _farPlane, _projection);
    }

    mul(_projection, _view, _viewProjection);
    invert(_viewProjection, _inverseViewProjection);
    _bounds = Frustum(_viewProjection);
}
}
