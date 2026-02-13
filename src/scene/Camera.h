#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"
#include <filament/Camera.h>
#include <math/vec3.h>

class Camera : public GameObject
{
private:
    float m_fov;
    float m_nearPlane;
    float m_farPlane;
    float m_aspectRatio;

public:
    Camera(const std::string& name = "Camera");
    ~Camera();

    void setProjection(float fov, float aspect, float near, float far);
    void setAspectRatio(float aspect);

    float getFov() const { return m_fov; }
    float getNearPlane() const { return m_nearPlane; }
    float getFarPlane() const { return m_farPlane; }
    float getAspectRatio() const { return m_aspectRatio; }

    void lookAt(const filament::math::float3& target);

    virtual void update(float deltaTime) override;
};

#endif // CAMERA_H
