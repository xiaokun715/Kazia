#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <math/vec3.h>

class GameObject
{
private:
    std::string m_name;
    filament::math::float3 m_position;
    filament::math::float3 m_rotation;
    filament::math::float3 m_scale;

public:
    GameObject(const std::string& name = "GameObject");
    virtual ~GameObject();

    virtual void update(float deltaTime);
    virtual void render();

    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

    const filament::math::float3& getPosition() const { return m_position; }
    void setPosition(const filament::math::float3& position) { m_position = position; }

    const filament::math::float3& getRotation() const { return m_rotation; }
    void setRotation(const filament::math::float3& rotation) { m_rotation = rotation; }

    const filament::math::float3& getScale() const { return m_scale; }
    void setScale(const filament::math::float3& scale) { m_scale = scale; }
};

#endif // GAMEOBJECT_H
