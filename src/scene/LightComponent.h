#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "Component.h"
#include "core/Math.h"

namespace Kazia {

// 简化的 Entity 类型
typedef uint64_t Entity;

// 简化的 LightManager 类
class LightManager {
public:
    enum class Type {
        SUN,
        POINT,
        SPOT,
        AREA
    };
};

class LightComponent : public Component {
private:
    Entity m_lightEntity;
    LightManager::Type m_lightType;
    
    // 光源参数
    math::float3 m_color;
    float m_intensity;
    
    // 方向光参数
    math::float3 m_direction;
    
    // 点光源参数
    float m_radius;
    
public:
    LightComponent();
    ~LightComponent() override = default;
    
    // 光源实体相关
    Entity getLightEntity() const { return m_lightEntity; }
    void setLightEntity(Entity entity) { m_lightEntity = entity; }
    
    // 光源类型相关
    LightManager::Type getLightType() const { return m_lightType; }
    void setLightType(LightManager::Type type) { m_lightType = type; }
    
    // 通用参数
    const math::float3& getColor() const { return m_color; }
    void setColor(const math::float3& color) { m_color = color; }
    
    float getIntensity() const { return m_intensity; }
    void setIntensity(float intensity) { m_intensity = intensity; }
    
    // 方向光参数
    const math::float3& getDirection() const { return m_direction; }
    void setDirection(const math::float3& direction) { m_direction = direction; }
    
    // 点光源参数
    float getRadius() const { return m_radius; }
    void setRadius(float radius) { m_radius = radius; }
    
    // 生命周期方法
    void initialize() override;
    void update() override;
    void shutdown() override;
};

} // namespace Kazia

#endif // LIGHTCOMPONENT_H
