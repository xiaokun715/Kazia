#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <filament/Camera.h>
#include "core/Math.h"

namespace Kazia {

class CameraController {
private:
    filament::Camera* m_camera;
    
    // 相机参数
    math::float3 m_position;
    math::float3 m_target;
    math::float3 m_up;
    
    // Orbit 控制参数
    float m_zoomSpeed;
    float m_rotationSpeed;
    float m_panSpeed;
    
    float m_minDistance;
    float m_maxDistance;
    
    // 鼠标状态
    bool m_isDragging;
    int m_lastMouseX;
    int m_lastMouseY;
    
public:
    CameraController();
    ~CameraController() = default;
    
    // 设置相机
    void setCamera(filament::Camera* camera);
    
    // 初始化相机参数
    void initialize(const math::float3& position, const math::float3& target, const math::float3& up);
    
    // Orbit 控制
    void orbit(float deltaX, float deltaY);
    
    // 缩放控制
    void zoom(float deltaZ);
    
    // 平移控制
    void pan(float deltaX, float deltaY);
    
    // 设置目标
    void setTarget(const math::float3& target);
    
    // 获取相机参数
    math::float3 getPosition() const { return m_position; }
    math::float3 getTarget() const { return m_target; }
    math::float3 getUp() const { return m_up; }
    
    // 更新相机
    void updateCamera();
    
    // 鼠标事件处理
    void onMousePress(int x, int y);
    void onMouseMove(int x, int y);
    void onMouseRelease();
    void onWheel(int delta);
};

} // namespace Kazia

#endif // CAMERACONTROLLER_H
