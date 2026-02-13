#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "Component.h"

namespace Kazia {

// 简化的 Entity 类型
typedef uint64_t Entity;

// 简化的 Camera 类
class Camera {
public:
    void setProjection(float fov, float aspect, float near, float far) {}
    void lookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ) {}
};

class CameraComponent : public Component {
private:
    Entity m_cameraEntity;
    Camera* m_camera;
    
    // 相机参数
    float m_fov;
    float m_near;
    float m_far;
    
public:
    CameraComponent();
    ~CameraComponent() override = default;
    
    // 相机实体相关
    Entity getCameraEntity() const { return m_cameraEntity; }
    void setCameraEntity(Entity entity) { m_cameraEntity = entity; }
    
    // 相机指针相关
    Camera* getCamera() const { return m_camera; }
    void setCamera(Camera* camera) { m_camera = camera; }
    
    // 相机参数相关
    float getFOV() const { return m_fov; }
    void setFOV(float fov) { m_fov = fov; }
    
    float getNear() const { return m_near; }
    void setNear(float near) { m_near = near; }
    
    float getFar() const { return m_far; }
    void setFar(float far) { m_far = far; }
    
    // 生命周期方法
    void initialize() override;
    void update() override;
    void shutdown() override;
};

} // namespace Kazia

#endif // CAMERACOMPONENT_H
