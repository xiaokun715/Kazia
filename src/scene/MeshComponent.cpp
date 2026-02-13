#include "MeshComponent.h"

namespace Kazia {

MeshComponent::MeshComponent() : m_meshEntity(0) {
}

void MeshComponent::initialize() {
    // 初始化时创建默认实体
    // 这里需要通过 RenderContext 创建 Filament 实体
    // 暂时设置为 1 作为占位符
    m_meshEntity = 1;
}

void MeshComponent::update() {
    // 同步变换到 Filament 实体
    if (getOwner() && m_meshEntity != 0) {
        // 这里需要通过 RenderContext 获取 TransformManager
        // 暂时留空，后续与 Filament 映射系统集成
    }
}

void MeshComponent::shutdown() {
    // 销毁网格实体
    if (m_meshEntity != 0) {
        // 这里需要通过 RenderContext 销毁 Filament 实体
        // 暂时设置为 0 作为占位符
        m_meshEntity = 0;
    }
}

} // namespace Kazia
