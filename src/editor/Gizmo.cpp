#include "Gizmo.h"

#include "scene/Node.h"

namespace Kazia {

Gizmo::Gizmo(filament::Engine* engine, filament::Scene* scene, SelectionManager* selectionManager) 
    : m_engine(engine), 
      m_scene(scene), 
      m_selectionManager(selectionManager), 
      m_gizmoEntity(utils::Entity::INVALID), 
      m_type(GizmoType::MOVE), 
      m_activeAxis(Axis::NONE), 
      m_isDragging(false), 
      m_lastMouseX(0), 
      m_lastMouseY(0), 
      m_transformOrigin({0.0f, 0.0f, 0.0f}) {
    // 创建 Gizmo 几何体
    createGizmoGeometry();
}

Gizmo::~Gizmo() {
    // 从场景中移除并销毁 Gizmo 实体
    if (m_gizmoEntity.isValid() && m_scene) {
        m_scene->removeEntity(m_gizmoEntity);
    }
    
    if (m_gizmoEntity.isValid() && m_engine) {
        utils::EntityManager::get().destroy(m_gizmoEntity);
    }
}

void Gizmo::setActive(bool active) {
    if (m_gizmoEntity.isValid() && m_scene) {
        if (active) {
            m_scene->addEntity(m_gizmoEntity);
        } else {
            m_scene->removeEntity(m_gizmoEntity);
        }
    }
}

bool Gizmo::onMousePress(int x, int y, const filament::math::float3& rayOrigin, const filament::math::float3& rayDirection) {
    // 检查是否有选中的节点
    if (!m_selectionManager || m_selectionManager->getSelectionCount() == 0) {
        return false;
    }
    
    // 计算变换原点（使用第一个选中节点的位置）
    Node* activeNode = m_selectionManager->getActiveNode();
    if (activeNode) {
        m_transformOrigin = activeNode->getPosition();
    }
    
    // 检测射线与 Gizmo 的交点，确定激活的轴线
    for (int i = static_cast<int>(Axis::X); i <= static_cast<int>(Axis::ALL); ++i) {
        Axis axis = static_cast<Axis>(i);
        filament::math::float3 intersection;
        
        if (intersectRayWithAxis(rayOrigin, rayDirection, axis, intersection)) {
            m_activeAxis = axis;
            m_isDragging = true;
            m_lastMouseX = x;
            m_lastMouseY = y;
            return true;
        }
    }
    
    return false;
}

bool Gizmo::onMouseMove(int x, int y, const filament::math::float3& rayOrigin, const filament::math::float3& rayDirection) {
    if (!m_isDragging || m_activeAxis == Axis::NONE) {
        return false;
    }
    
    // 计算鼠标移动 delta
    int deltaX = x - m_lastMouseX;
    int deltaY = y - m_lastMouseY;
    
    // 根据激活的轴线计算变换 delta
    filament::math::float3 transformDelta = {0.0f, 0.0f, 0.0f};
    
    // 这里需要根据具体的 Gizmo 类型和激活的轴线来计算变换 delta
    // 暂时留空，由子类实现
    
    // 应用变换
    applyTransform(transformDelta);
    
    // 更新鼠标位置
    m_lastMouseX = x;
    m_lastMouseY = y;
    
    return true;
}

bool Gizmo::onMouseRelease() {
    m_isDragging = false;
    m_activeAxis = Axis::NONE;
    return true;
}

} // namespace Kazia
