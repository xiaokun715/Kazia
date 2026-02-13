#include "CameraComponent.h"

namespace Kazia {

CameraComponent::CameraComponent() 
    : m_cameraEntity(0), 
      m_camera(nullptr), 
      m_fov(45.0f), 
      m_near(0.1f), 
      m_far(1000.0f) {
}

void CameraComponent::initialize() {
    // 初始化时创建默认实体
    // 这里需要通过 RenderContext 创建 Filament 实体
    // 暂时设置为 2 作为占位符
    m_cameraEntity = 2;
}

void CameraComponent::update() {
    // 同步相机参数
    if (m_camera) {
        // 这里需要通过 RenderContext 获取视图大小来计算 aspect
        // 暂时留空，后续与 Filament 映射系统集成
    }
}

void CameraComponent::shutdown() {
    // 相机实体由 Filament 引擎管理，这里不需要手动销毁
    m_camera = nullptr;
}

} // namespace Kazia
