#ifndef LIGHTSYSTEM_H
#define LIGHTSYSTEM_H

#include <filament/Engine.h>
#include <filament/Scene.h>
#include <filament/LightManager.h>
#include <filament/math.h>

#include <vector>
#include <memory>

namespace Kazia {

class LightSystem {
private:
    filament::Engine* m_engine;
    filament::Scene* m_scene;
    
    struct DirectionalLight {
        utils::Entity entity;
        filament::math::float3 direction;
        filament::math::float3 color;
        float intensity;
    };
    
    struct PointLight {
        utils::Entity entity;
        filament::math::float3 position;
        filament::math::float3 color;
        float intensity;
        float radius;
    };
    
    std::vector<DirectionalLight> m_directionalLights;
    std::vector<PointLight> m_pointLights;
    
public:
    LightSystem();
    ~LightSystem() = default;
    
    // 初始化
    void initialize(filament::Engine* engine, filament::Scene* scene);
    
    // 添加方向光
    void addDirectionalLight(const filament::math::float3& direction, const filament::math::float3& color, float intensity);
    
    // 添加点光源
    void addPointLight(const filament::math::float3& position, const filament::math::float3& color, float intensity, float radius);
    
    // 移除方向光
    void removeDirectionalLight(size_t index);
    
    // 移除点光源
    void removePointLight(size_t index);
    
    // 更新方向光
    void updateDirectionalLight(size_t index, const filament::math::float3& direction, const filament::math::float3& color, float intensity);
    
    // 更新点光源
    void updatePointLight(size_t index, const filament::math::float3& position, const filament::math::float3& color, float intensity, float radius);
    
    // 获取方向光数量
    size_t getDirectionalLightCount() const { return m_directionalLights.size(); }
    
    // 获取点光源数量
    size_t getPointLightCount() const { return m_pointLights.size(); }
    
    // 清理所有光源
    void clearAllLights();
};

} // namespace Kazia

#endif // LIGHTSYSTEM_H
