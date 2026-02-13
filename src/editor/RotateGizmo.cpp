#include "RotateGizmo.h"

#include <filament/RenderableManager.h>
#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <filament/TransformManager.h>

#include <utils/EntityManager.h>

namespace Kazia {

RotateGizmo::RotateGizmo(filament::Engine* engine, filament::Scene* scene, SelectionManager* selectionManager)
    : Gizmo(engine, scene, selectionManager),
      m_xRingEntity(utils::Entity::INVALID),
      m_yRingEntity(utils::Entity::INVALID),
      m_zRingEntity(utils::Entity::INVALID) {
    m_type = GizmoType::ROTATE;
    createGizmoGeometry();
}

RotateGizmo::~RotateGizmo() {
    // 清理所有实体
    auto cleanupEntity = [this](utils::Entity& entity) {
        if (entity.isValid() && m_scene) {
            m_scene->removeEntity(entity);
        }
        if (entity.isValid() && m_engine) {
            utils::EntityManager::get().destroy(entity);
        }
    };
    
    cleanupEntity(m_xRingEntity);
    cleanupEntity(m_yRingEntity);
    cleanupEntity(m_zRingEntity);
}

void RotateGizmo::render() {
    // 渲染逻辑在 Filament 中处理
}

void RotateGizmo::update() {
    // 更新 Gizmo 位置到选中节点的位置
    if (m_selectionManager && m_selectionManager->getSelectionCount() > 0) {
        Node* activeNode = m_selectionManager->getActiveNode();
        if (activeNode) {
            m_transformOrigin = activeNode->getPosition();
            
            // 更新所有旋转环的位置
            auto& transformManager = m_engine->getTransformManager();
            
            auto updateEntityPosition = [&](utils::Entity entity) {
                if (entity.isValid() && transformManager.hasComponent(entity)) {
                    auto instance = transformManager.getInstance(entity);
                    transformManager.setTransform(instance, filament::math::mat4f::translation(m_transformOrigin));
                }
            };
            
            updateEntityPosition(m_xRingEntity);
            updateEntityPosition(m_yRingEntity);
            updateEntityPosition(m_zRingEntity);
        }
    }
}

bool RotateGizmo::intersectRayWithAxis(const filament::math::float3& rayOrigin, const filament::math::float3& rayDirection, Axis axis, filament::math::float3& intersection) {
    // 简化实现：检测射线与旋转环的交点
    // 这里使用简单的射线与圆环相交检测
    
    float radius = 1.0f;
    
    switch (axis) {
    case Axis::X:
        // X 轴旋转环
        break;
    case Axis::Y:
        // Y 轴旋转环
        break;
    case Axis::Z:
        // Z 轴旋转环
        break;
    default:
        return false;
    }
    
    // 简化实现：返回 false
    return false;
}

void RotateGizmo::applyTransform(const filament::math::float3& delta) {
    // 应用旋转变换到所有选中的节点
    if (m_selectionManager) {
        const auto& selectedNodes = m_selectionManager->getSelectedNodes();
        for (Node* node : selectedNodes) {
            if (node) {
                filament::math::float3 newRotation = node->getRotation() + delta;
                node->setRotation(newRotation);
            }
        }
    }
}

void RotateGizmo::createGizmoGeometry() {
    // 创建旋转环
    createRing({1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, m_xRingEntity); // X 轴 - 红色
    createRing({0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, m_yRingEntity); // Y 轴 - 绿色
    createRing({0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, m_zRingEntity); // Z 轴 - 蓝色
    
    // 将所有实体添加到场景
    auto addEntityToScene = [this](utils::Entity& entity) {
        if (entity.isValid() && m_scene) {
            m_scene->addEntity(entity);
        }
    };
    
    addEntityToScene(m_xRingEntity);
    addEntityToScene(m_yRingEntity);
    addEntityToScene(m_zRingEntity);
}

void RotateGizmo::createRing(const filament::math::float3& axis, const filament::math::float3& color, utils::Entity& entity) {
    // 创建旋转环实体
    entity = utils::EntityManager::get().create();
    
    // 圆环半径和厚度
    float radius = 1.0f;
    float thickness = 0.05f;
    
    // 创建圆环几何体
    // 这里使用简化的实现，实际项目中可能需要更复杂的几何体
    
    // 暂时留空，实际项目中需要创建真实的几何体
}

} // namespace Kazia
