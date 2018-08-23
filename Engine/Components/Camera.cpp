#include "Camera.h"
#include "Components/TransformComponent.h"

#include <cassert>

namespace atlas
{
using namespace math;

Camera::Camera()
    : _cameraType(CameraType::ePerspective)
{
}

Camera::~Camera()
{
}

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

void Camera::setTransform(const Matrix& transform)
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

const Matrix& Camera::getViewMatrix() const
{
    return _view;
}

const Matrix& Camera::getInverseViewMatrix() const
{
    return _inverseView;
}

const Matrix& Camera::getProjectionMatrix() const
{
    return _projection;
}

void Camera::setProjectionMatrix(const Matrix& matrix)
{
    _projection = matrix;
    updateCamera();
}

const Matrix& Camera::getViewProjectionMatrix() const
{
    return _viewProjection;
}

const Matrix& Camera::getInverseViewProjectionMatrix() const
{
    return _inverseViewProjection;
}

const Frustum& Camera::getFrustum() const
{
    return _bounds;
}

void Camera::project(const Rectangle& viewport, const Vector3& position, float* x, float* y, float* depth) const
{
    assert(x);
    assert(y);

    // Transform the point to clip-space.
    Vector4 clipPos;
    _viewProjection.transformVector(Vector4(position.x, position.y, position.z, 1.0f), &clipPos);

    // Compute normalized device coordinates.
    assert(clipPos.w != 0.0f);
    float ndcX = clipPos.x / clipPos.w;
    float ndcY = clipPos.y / clipPos.w;

    // Compute screen coordinates by applying our viewport transformation.
    *x = viewport.x + (ndcX + 1.0f) * 0.5f * viewport.width;
    *y = viewport.y + (1.0f - (ndcY + 1.0f) * 0.5f) * viewport.height;
    if (depth)
    {
        float ndcZ = clipPos.z / clipPos.w;
        *depth     = (ndcZ + 1.0f) / 2.0f;
    }
}

void Camera::project(const Rectangle& viewport, const Vector3& position, Vector2* out) const
{
    assert(out);
    float x, y;
    project(viewport, position, &x, &y);
    out->set(x, y);
}

void Camera::project(const Rectangle& viewport, const Vector3& position, Vector3* out) const
{
    assert(out);
    float x, y, depth;
    project(viewport, position, &x, &y, &depth);
    out->set(x, y, depth);
}

void Camera::unproject(const Rectangle& viewport, float x, float y, float depth, Vector3* dst) const
{
    assert(dst);

    // Create our screen space position in NDC.
    assert(viewport.width != 0.0f && viewport.height != 0.0f);
    Vector4 screen((x - viewport.x) / viewport.width, ((viewport.height - y) - viewport.y) / viewport.height, depth,
                   1.0f);

    // Map to range -1 to 1.
    screen.x = screen.x * 2.0f - 1.0f;
    screen.y = screen.y * 2.0f - 1.0f;
    screen.z = screen.z * 2.0f - 1.0f;

    // Transform the screen-space NDC by our inverse view projection matrix.
    _inverseViewProjection.transformVector(screen, &screen);

    // Divide by our W coordinate.
    if (screen.w != 0.0f)
    {
        screen.x /= screen.w;
        screen.y /= screen.w;
        screen.z /= screen.w;
    }

    dst->set(screen.x, screen.y, screen.z);
}

void Camera::pickRay(const Rectangle& viewport, float x, float y, Ray* dst) const
{
    assert(dst);

    // Get the world-space position at the near clip plane.
    Vector3 nearPoint;
    unproject(viewport, x, y, 0.0f, &nearPoint);

    // Get the world-space position at the far clip plane.
    Vector3 farPoint;
    unproject(viewport, x, y, 1.0f, &farPoint);

    // Set the direction of the ray.
    Vector3 direction;
    Vector3::subtract(farPoint, nearPoint, &direction);
    direction.normalize();

    dst->set(nearPoint, direction);
}

void Camera::updateCamera(const Matrix& transform)
{
    _view = transform;
    _view.invert(&_inverseView);

    if (_cameraType == CameraType::ePerspective)
    {
        Matrix::createPerspective(_fieldOfView, _aspectRatio, _nearPlane, _farPlane, &_projection);
    }
    else
    {
        // Create an ortho projection with the origin at the bottom left of the viewport, +X to the right and +Y
        Matrix::createOrthographic(_zoom[0], _zoom[1], _nearPlane, _farPlane, &_projection);
    }

    Matrix::multiply(_projection, _view, &_viewProjection);
    _viewProjection.invert(&_inverseViewProjection);
    _bounds.set(_viewProjection);
}
}
