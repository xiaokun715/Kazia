#include "CameraController.h"

namespace Kazia {

// 辅助数学函数
math::float3 normalize(const math::float3& v) {
    float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length > 0.0f) {
        return math::float3(v.x / length, v.y / length, v.z / length);
    }
    return v;
}

float length(const math::float3& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

math::float3 cross(const math::float3& a, const math::float3& b) {
    return math::float3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

CameraController::CameraController()
    : m_camera(nullptr)
    , m_position({0.0f, 0.0f, 5.0f})
    , m_target({0.0f, 0.0f, 0.0f})
    , m_up({0.0f, 1.0f, 0.0f})
    , m_zoomSpeed(0.1f)
    , m_rotationSpeed(0.01f)
    , m_panSpeed(0.01f)
    , m_minDistance(0.1f)
    , m_maxDistance(100.0f)
    , m_isDragging(false)
    , m_lastMouseX(0)
    , m_lastMouseY(0)
{
}

void CameraController::setCamera(filament::Camera* camera)
{
    m_camera = camera;
    if (m_camera) {
        updateCamera();
    }
}

void CameraController::initialize(const math::float3& position, const math::float3& target, const math::float3& up)
{
    m_position = position;
    m_target = target;
    m_up = up;
    
    if (m_camera) {
        updateCamera();
    }
}

void CameraController::orbit(float deltaX, float deltaY)
{
    // 计算相机到目标的向量
    math::float3 direction = normalize(m_position - m_target);
    
    // 计算距离
    float distance = length(m_position - m_target);
    
    // 计算旋转角度
    float thetaY = deltaX * m_rotationSpeed;
    float thetaX = deltaY * m_rotationSpeed;
    
    // 简化实现：使用欧拉角旋转
    // 这里需要实现更复杂的旋转矩阵计算
    // 暂时使用简单的方法
    math::float3 newDirection = direction;
    
    // 更新相机位置
    m_position = m_target + newDirection * distance;
    
    if (m_camera) {
        updateCamera();
    }
}

void CameraController::zoom(float deltaZ)
{
    // 计算相机到目标的向量
    math::float3 direction = normalize(m_position - m_target);
    
    // 计算距离
    float distance = length(m_position - m_target);
    
    // 更新距离
    distance -= deltaZ * m_zoomSpeed;
    distance = std::max(m_minDistance, std::min(m_maxDistance, distance));
    
    // 更新相机位置
    m_position = m_target + direction * distance;
    
    if (m_camera) {
        updateCamera();
    }
}

void CameraController::pan(float deltaX, float deltaY)
{
    // 计算相机到目标的向量
    math::float3 direction = normalize(m_position - m_target);
    
    // 计算右向量和上向量
    math::float3 right = normalize(cross(direction, m_up));
    math::float3 up = normalize(cross(right, direction));
    
    // 计算平移向量
    float distance = length(m_position - m_target);
    math::float3 translation = {right.x * deltaX * m_panSpeed * distance + up.x * deltaY * m_panSpeed * distance,
                               right.y * deltaX * m_panSpeed * distance + up.y * deltaY * m_panSpeed * distance,
                               right.z * deltaX * m_panSpeed * distance + up.z * deltaY * m_panSpeed * distance};
    
    // 更新相机位置和目标
    m_position = {m_position.x + translation.x, m_position.y + translation.y, m_position.z + translation.z};
    m_target = {m_target.x + translation.x, m_target.y + translation.y, m_target.z + translation.z};
    
    if (m_camera) {
        updateCamera();
    }
}

void CameraController::setTarget(const math::float3& target)
{
    m_target = target;
    
    if (m_camera) {
        updateCamera();
    }
}

void CameraController::updateCamera()
{
    if (m_camera) {
        filament::math::float3 filaPosition = {m_position.x, m_position.y, m_position.z};
        filament::math::float3 filaTarget = {m_target.x, m_target.y, m_target.z};
        filament::math::float3 filaUp = {m_up.x, m_up.y, m_up.z};
        m_camera->lookAt(filaPosition, filaTarget, filaUp);
    }
}

void CameraController::onMousePress(int x, int y)
{
    m_isDragging = true;
    m_lastMouseX = x;
    m_lastMouseY = y;
}

void CameraController::onMouseMove(int x, int y)
{
    if (m_isDragging) {
        int deltaX = x - m_lastMouseX;
        int deltaY = y - m_lastMouseY;
        
        orbit(deltaX, deltaY);
        
        m_lastMouseX = x;
        m_lastMouseY = y;
    }
}

void CameraController::onMouseRelease()
{
    m_isDragging = false;
}

void CameraController::onWheel(int delta)
{
    zoom(delta);
}

} // namespace Kazia
