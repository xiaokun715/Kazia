#include "ScaleGizmo.h"

#include <filament/RenderableManager.h>
#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <filament/TransformManager.h>

#include <utils/EntityManager.h>

namespace Kazia {

ScaleGizmo::ScaleGizmo(filament::Engine* engine, filament::Scene* scene, SelectionManager* selectionManager)
    : Gizmo(engine, scene, selectionManager),
      m_xHandleEntity(utils::Entity::INVALID),
      m_yHandleEntity(utils::Entity::INVALID),
      m_zHandleEntity(utils::Entity::INVALID),
      m_uniformHandleEntity(utils::Entity::INVALID) {
    m_type = GizmoType::SCALE;
    createGizmoGeometry();
}

ScaleGizmo::~ScaleGizmo() {
    // 清理所有实体
    auto cleanupEntity = [this](utils::Entity& entity) {
        if (entity.isValid() && m_scene) {
            m_scene->removeEntity(entity);
        }
        if (entity.isValid() && m_engine) {
            utils::EntityManager::get().destroy(entity);
        }
    };
    
    cleanupEntity(m_xHandleEntity);
    cleanupEntity(m_yHandleEntity);
    cleanupEntity(m_zHandleEntity);
    cleanupEntity(m_uniformHandleEntity);
}

void ScaleGizmo::render() {
    // 渲染逻辑在 Filament 中处理
}

void ScaleGizmo::update() {
    // 更新 Gizmo 位置到选中节点的位置
    if (m_selectionManager && m_selectionManager->getSelectionCount() > 0) {
        Node* activeNode = m_selectionManager->getActiveNode();
        if (activeNode) {
            m_transformOrigin = activeNode->getPosition();
            
            // 更新所有缩放手柄的位置
            auto& transformManager = m_engine->getTransformManager();
            
            auto updateEntityPosition = [&](utils::Entity entity) {
                if (entity.isValid() && transformManager.hasComponent(entity)) {
                    auto instance = transformManager.getInstance(entity);
                    transformManager.setTransform(instance, filament::math::mat4f::translation(m_transformOrigin));
                }
            };
            
            updateEntityPosition(m_xHandleEntity);
            updateEntityPosition(m_yHandleEntity);
            updateEntityPosition(m_zHandleEntity);
            updateEntityPosition(m_uniformHandleEntity);
        }
    }
}

bool ScaleGizmo::intersectRayWithAxis(const filament::math::float3& rayOrigin, const filament::math::float3& rayDirection, Axis axis, filament::math::float3& intersection) {
    // 简化实现：检测射线与轴线的交点
    // 这里使用简单的射线与线段相交检测
    
    filament::math::float3 axisStart = m_transformOrigin;
    filament::math::float3 axisEnd;
    float axisLength = 2.0f;
    
    switch (axis) {
    case Axis::X:
        axisEnd = axisStart + filament::math::float3(axisLength, 0.0f, 0.0f);
        break;
    case Axis::Y:
        axisEnd = axisStart + filament::math::float3(0.0f, axisLength, 0.0f);
        break;
    case Axis::Z:
        axisEnd = axisStart + filament::math::float3(0.0f, 0.0f, axisLength);
        break;
    case Axis::ALL:
        // 均匀缩放
        break;
    default:
        return false;
    }
    
    // 简化实现：返回 false
    return false;
}

void ScaleGizmo::applyTransform(const filament::math::float3& delta) {
    // 应用缩放变换到所有选中的节点
    if (m_selectionManager) {
        const auto& selectedNodes = m_selectionManager->getSelectedNodes();
        for (Node* node : selectedNodes) {
            if (node) {
                filament::math::float3 newScale = node->getScale() * (filament::math::float3{1.0f, 1.0f, 1.0f} + delta);
                node->setScale(newScale);
            }
        }
    }
}

void ScaleGizmo::createGizmoGeometry() {
    // 创建缩放手柄
    createHandle({1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, m_xHandleEntity); // X 轴 - 红色
    createHandle({0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, m_yHandleEntity); // Y 轴 - 绿色
    createHandle({0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, m_zHandleEntity); // Z 轴 - 蓝色
    createHandle({1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, m_uniformHandleEntity); // 均匀缩放 - 白色
    
    // 将所有实体添加到场景
    auto addEntityToScene = [this](utils::Entity& entity) {
        if (entity.isValid() && m_scene) {
            m_scene->addEntity(entity);
        }
    };
    
    addEntityToScene(m_xHandleEntity);
    addEntityToScene(m_yHandleEntity);
    addEntityToScene(m_zHandleEntity);
    addEntityToScene(m_uniformHandleEntity);
}

void ScaleGizmo::createHandle(const filament::math::float3& direction, const filament::math::float3& color, utils::Entity& entity) {
    // 创建缩放手柄实体
    entity = utils::EntityManager::get().create();
    
    // 手柄大小
    float size = 0.2f;
    
    // 创建手柄几何体
    // 这里使用简化的实现，实际项目中可能需要更复杂的几何体
    
    // 暂时留空，实际项目中需要创建真实的几何体
}

} // namespace Kazia
