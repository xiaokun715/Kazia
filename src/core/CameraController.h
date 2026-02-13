#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <filament/Camera.h>
#include <filament/math/mathfwd.h>

class CameraController
{
private:
    filament::Camera* m_camera;
    filament::math::float3 m_target;
    float m_distance;
    float m_yaw;
    float m_pitch;
    float m_moveSpeed;
    float m_rotationSpeed;
    float m_zoomSpeed;

public:
    CameraController(filament::Camera* camera);
    ~CameraController();

    void setTarget(const filament::math::float3& target);
    void setDistance(float distance);
    void setYaw(float yaw);
    void setPitch(float pitch);
    void setMoveSpeed(float speed);
    void setRotationSpeed(float speed);
    void setZoomSpeed(float speed);

    void rotate(float deltaYaw, float deltaPitch);
    void zoom(float deltaDistance);
    void pan(const filament::math::float2& delta);
    void move(const filament::math::float3& delta);

    void update();

    const filament::math::float3& getTarget() const { return m_target; }
    float getDistance() const { return m_distance; }
    float getYaw() const { return m_yaw; }
    float getPitch() const { return m_pitch; }

private:
    filament::math::float3 calculateCameraPosition() const;
};

#endif // CAMERACONTROLLER_H
