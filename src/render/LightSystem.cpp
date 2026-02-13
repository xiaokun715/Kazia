#include "LightSystem.h"

#include <utils/EntityManager.h>
#include <utils/Entity.h>

namespace Kazia {

LightSystem::LightSystem()
    : m_engine(nullptr)
    , m_scene(nullptr)
{
}

void LightSystem::initialize(filament::Engine* engine, filament::Scene* scene)
{
    m_engine = engine;
    m_scene = scene;
}

void LightSystem::addDirectionalLight(const filament::math::float3& direction, const filament::math::float3& color, float intensity)
{
    if (!m_engine || !m_scene) {
        return;
    }
    
    // 创建方向光
    filament::LightManager::Builder lightBuilder(filament::LightManager::Type::DIRECTIONAL);
    lightBuilder.color(color);
    lightBuilder.intensity(intensity);
    lightBuilder.direction(direction);
    
    utils::Entity lightEntity = utils::EntityManager::get().create();
    lightBuilder.build(*m_engine, lightEntity);
    m_scene->addEntity(lightEntity);
    
    // 添加到列表
    DirectionalLight light;
    light.entity = lightEntity;
    light.direction = direction;
    light.color = color;
    light.intensity = intensity;
    m_directionalLights.push_back(light);
}

void LightSystem::addPointLight(const filament::math::float3& position, const filament::math::float3& color, float intensity, float radius)
{
    if (!m_engine || !m_scene) {
        return;
    }
    
    // 创建点光源
    filament::LightManager::Builder lightBuilder(filament::LightManager::Type::POINT);
    lightBuilder.color(color);
    lightBuilder.intensity(intensity);
    lightBuilder.position(position);
    lightBuilder.falloff(radius);
    
    utils::Entity lightEntity = utils::EntityManager::get().create();
    lightBuilder.build(*m_engine, lightEntity);
    m_scene->addEntity(lightEntity);
    
    // 添加到列表
    PointLight light;
    light.entity = lightEntity;
    light.position = position;
    light.color = color;
    light.intensity = intensity;
    light.radius = radius;
    m_pointLights.push_back(light);
}

void LightSystem::removeDirectionalLight(size_t index)
{
    if (index >= m_directionalLights.size()) {
        return;
    }
    
    if (m_engine && m_scene) {
        // 从场景中移除
        m_scene->removeEntity(m_directionalLights[index].entity);
        
        // 销毁实体
        utils::EntityManager::get().destroy(m_directionalLights[index].entity);
    }
    
    // 从列表中移除
    m_directionalLights.erase(m_directionalLights.begin() + index);
}

void LightSystem::removePointLight(size_t index)
{
    if (index >= m_pointLights.size()) {
        return;
    }
    
    if (m_engine && m_scene) {
        // 从场景中移除
        m_scene->removeEntity(m_pointLights[index].entity);
        
        // 销毁实体
        utils::EntityManager::get().destroy(m_pointLights[index].entity);
    }
    
    // 从列表中移除
    m_pointLights.erase(m_pointLights.begin() + index);
}

void LightSystem::updateDirectionalLight(size_t index, const filament::math::float3& direction, const filament::math::float3& color, float intensity)
{
    if (index >= m_directionalLights.size()) {
        return;
    }
    
    if (m_engine) {
        // 获取光源管理器
        auto& lightManager = m_engine->getLightManager();
        auto instance = lightManager.getInstance(m_directionalLights[index].entity);
        
        // 更新光源参数
        lightManager.setDirection(instance, direction);
        lightManager.setColor(instance, color);
        lightManager.setIntensity(instance, intensity);
        
        // 更新列表中的参数
        m_directionalLights[index].direction = direction;
        m_directionalLights[index].color = color;
        m_directionalLights[index].intensity = intensity;
    }
}

void LightSystem::updatePointLight(size_t index, const filament::math::float3& position, const filament::math::float3& color, float intensity, float radius)
{
    if (index >= m_pointLights.size()) {
        return;
    }
    
    if (m_engine) {
        // 获取光源管理器
        auto& lightManager = m_engine->getLightManager();
        auto instance = lightManager.getInstance(m_pointLights[index].entity);
        
        // 更新光源参数
        lightManager.setPosition(instance, position);
        lightManager.setColor(instance, color);
        lightManager.setIntensity(instance, intensity);
        lightManager.setFalloff(instance, radius);
        
        // 更新列表中的参数
        m_pointLights[index].position = position;
        m_pointLights[index].color = color;
        m_pointLights[index].intensity = intensity;
        m_pointLights[index].radius = radius;
    }
}

void LightSystem::clearAllLights()
{
    if (m_engine && m_scene) {
        // 移除所有方向光
        for (const auto& light : m_directionalLights) {
            m_scene->removeEntity(light.entity);
            utils::EntityManager::get().destroy(light.entity);
        }
        
        // 移除所有点光源
        for (const auto& light : m_pointLights) {
            m_scene->removeEntity(light.entity);
            utils::EntityManager::get().destroy(light.entity);
        }
    }
    
    // 清空列表
    m_directionalLights.clear();
    m_pointLights.clear();
}

} // namespace Kazia
