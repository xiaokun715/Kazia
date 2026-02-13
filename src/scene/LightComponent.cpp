#include "LightComponent.h"

namespace Kazia {

LightComponent::LightComponent() 
    : m_lightEntity(0), 
      m_lightType(LightManager::Type::SUN), 
      m_color({1.0f, 1.0f, 1.0f}), 
      m_intensity(1.0f), 
      m_direction({0.0f, -1.0f, 0.0f}), 
      m_radius(10.0f) {
}

void LightComponent::initialize() {
    // 初始化时创建默认实体
    // 这里需要通过 RenderContext 创建 Filament 实体
    // 暂时设置为 3 作为占位符
    m_lightEntity = 3;
}

void LightComponent::update() {
    // 同步光源参数到 Filament 实体
    if (getOwner() && m_lightEntity != 0) {
        // 这里需要通过 RenderContext 获取 LightManager
        // 暂时留空，后续与 Filament 映射系统集成
    }
}

void LightComponent::shutdown() {
    // 销毁光源实体
    if (m_lightEntity != 0) {
        // 这里需要通过 RenderContext 销毁 Filament 实体
        // 暂时设置为 0 作为占位符
        m_lightEntity = 0;
    }
}

} // namespace Kazia
