#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "Component.h"
#include "core/Math.h"

namespace Kazia {

class TransformComponent : public Component {
private:
    math::float3 m_position;
    math::float3 m_rotation;
    math::float3 m_scale;
    
public:
    TransformComponent();
    ~TransformComponent() override = default;
    
    // 位置相关
    const math::float3& getPosition() const { return m_position; }
    void setPosition(const math::float3& position);
    
    // 旋转相关
    const math::float3& getRotation() const { return m_rotation; }
    void setRotation(const math::float3& rotation);
    
    // 缩放相关
    const math::float3& getScale() const { return m_scale; }
    void setScale(const math::float3& scale);
    
    // 生命周期方法
    void update() override;
};

} // namespace Kazia

#endif // TRANSFORMCOMPONENT_H
