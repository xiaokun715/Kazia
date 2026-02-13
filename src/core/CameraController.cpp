#include "CameraController.h"
#include <math/norm.h>
#include <math/vec3.h>
#include <math/mat4.h>

CameraController::CameraController(filament::Camera* camera)
    : m_camera(camera)
    , m_target({0.0f, 0.0f, 0.0f})
    , m_distance(5.0f)
    , m_yaw(0.0f)
    , m_pitch(0.0f)
    , m_moveSpeed(1.0f)
    , m_rotationSpeed(0.01f)
    , m_zoomSpeed(0.1f)
{
}

CameraController::~CameraController()
{
}

void CameraController::setTarget(const filament::math::float3& target)
{
    m_target = target;
}

void CameraController::setDistance(float distance)
{
    m_distance = distance;
}

void CameraController::setYaw(float yaw)
{
    m_yaw = yaw;
}

void CameraController::setPitch(float pitch)
{
    m_pitch = pitch;
}

void CameraController::setMoveSpeed(float speed)
{
    m_moveSpeed = speed;
}

void CameraController::setRotationSpeed(float speed)
{
    m_rotationSpeed = speed;
}

void CameraController::setZoomSpeed(float speed)
{
    m_zoomSpeed = speed;
}

void CameraController::rotate(float deltaYaw, float deltaPitch)
{
    m_yaw += deltaYaw * m_rotationSpeed;
    m_pitch += deltaPitch * m_rotationSpeed;

    // 限制俯仰角范围
    m_pitch = std::max(-1.5f, std::min(1.5f, m_pitch));
}

void CameraController::zoom(float deltaDistance)
{
    m_distance -= deltaDistance * m_zoomSpeed;
    m_distance = std::max(1.0f, std::min(20.0f, m_distance));
}

void CameraController::pan(const filament::math::float2& delta)
{
    // 计算相机位置
    filament::math::float3 cameraPos = calculateCameraPosition();

    // 计算相机朝向
    filament::math::float3 forward = m_target - cameraPos;
    float length = std::sqrt(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
    if (length > 0.0f) {
        forward /= length;
    }

    // 计算右向量
    filament::math::float3 up = {0.0f, 1.0f, 0.0f};
    filament::math::float3 right = {
        forward.y * up.z - forward.z * up.y,
        forward.z * up.x - forward.x * up.z,
        forward.x * up.y - forward.y * up.x
    };
    length = std::sqrt(right.x * right.x + right.y * right.y + right.z * right.z);
    if (length > 0.0f) {
        right /= length;
    }

    // 重新计算上向量
    up = {
        right.y * forward.z - right.z * forward.y,
        right.z * forward.x - right.x * forward.z,
        right.x * forward.y - right.y * forward.x
    };
    length = std::sqrt(up.x * up.x + up.y * up.y + up.z * up.z);
    if (length > 0.0f) {
        up /= length;
    }

    // 计算平移向量
    filament::math::float3 deltaPos = (-delta.x * right + delta.y * up) * m_moveSpeed * m_distance * 0.01f;

    // 更新目标位置
    m_target += deltaPos;
}

void CameraController::move(const filament::math::float3& delta)
{
    // 更新目标位置
    m_target += delta * m_moveSpeed;
}

void CameraController::update()
{
    if (!m_camera) return;

    // 计算相机位置
    filament::math::float3 cameraPos = calculateCameraPosition();

    // 设置相机矩阵
    filament::math::float3 up = {0.0f, 1.0f, 0.0f};
    filament::math::float3 forward = m_target - cameraPos;
    float length = std::sqrt(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
    if (length > 0.0f) {
        forward /= length;
    }
    
    filament::math::float3 right = {
        forward.y * up.z - forward.z * up.y,
        forward.z * up.x - forward.x * up.z,
        forward.x * up.y - forward.y * up.x
    };
    length = std::sqrt(right.x * right.x + right.y * right.y + right.z * right.z);
    if (length > 0.0f) {
        right /= length;
    }
    
    up = {
        right.y * forward.z - right.z * forward.y,
        right.z * forward.x - right.x * forward.z,
        right.x * forward.y - right.y * forward.x
    };
    length = std::sqrt(up.x * up.x + up.y * up.y + up.z * up.z);
    if (length > 0.0f) {
        up /= length;
    }
    
    filament::math::mat4f viewMatrix = {
        right.x, up.x, -forward.x, 0.0f,
        right.y, up.y, -forward.y, 0.0f,
        right.z, up.z, -forward.z, 0.0f,
        -(right.x * cameraPos.x + right.y * cameraPos.y + right.z * cameraPos.z),
        -(up.x * cameraPos.x + up.y * cameraPos.y + up.z * cameraPos.z),
        forward.x * cameraPos.x + forward.y * cameraPos.y + forward.z * cameraPos.z,
        1.0f
    };
    
    m_camera->setModelMatrix(viewMatrix);
}

filament::math::float3 CameraController::calculateCameraPosition() const
{
    // 计算相机位置
    float x = m_target.x + m_distance * std::cos(m_yaw) * std::cos(m_pitch);
    float y = m_target.y + m_distance * std::sin(m_pitch);
    float z = m_target.z + m_distance * std::sin(m_yaw) * std::cos(m_pitch);

    return filament::math::float3{x, y, z};
}
