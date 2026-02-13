#include "Camera.h"
#include <math/vec3.h>
#include <math/mat4.h>

Camera::Camera(const std::string& name)
    : GameObject(name)
    , m_fov(45.0f)
    , m_nearPlane(0.1f)
    , m_farPlane(1000.0f)
    , m_aspectRatio(1.0f)
{
}

Camera::~Camera()
{
}

void Camera::setProjection(float fov, float aspect, float near, float far)
{
    m_fov = fov;
    m_aspectRatio = aspect;
    m_nearPlane = near;
    m_farPlane = far;
}

void Camera::setAspectRatio(float aspect)
{
    m_aspectRatio = aspect;
}

void Camera::lookAt(const filament::math::float3& target)
{
    // 这里简化处理，实际应该通过 Filament 相机设置
    // 计算相机朝向
    filament::math::float3 direction = target - getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (length > 0.0f) {
        direction /= length;
    }
    // 这里可以计算旋转角度并设置
}

void Camera::update(float deltaTime)
{
    // 相机更新逻辑
    GameObject::update(deltaTime);
}
