#include "MoveGizmo.h"

#include <filament/RenderableManager.h>
#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <filament/TransformManager.h>

#include <utils/EntityManager.h>

namespace Kazia {

MoveGizmo::MoveGizmo(filament::Engine* engine, filament::Scene* scene, SelectionManager* selectionManager)
    : Gizmo(engine, scene, selectionManager),
      m_xAxisEntity(utils::Entity::INVALID),
      m_yAxisEntity(utils::Entity::INVALID),
      m_zAxisEntity(utils::Entity::INVALID),
      m_xyPlaneEntity(utils::Entity::INVALID),
      m_xzPlaneEntity(utils::Entity::INVALID),
      m_yzPlaneEntity(utils::Entity::INVALID) {
    m_type = GizmoType::MOVE;
    createGizmoGeometry();
}

MoveGizmo::~MoveGizmo() {
    // 清理所有实体
    auto cleanupEntity = [this](utils::Entity& entity) {
        if (entity.isValid() && m_scene) {
            m_scene->removeEntity(entity);
        }
        if (entity.isValid() && m_engine) {
            utils::EntityManager::get().destroy(entity);
        }
    };
    
    cleanupEntity(m_xAxisEntity);
    cleanupEntity(m_yAxisEntity);
    cleanupEntity(m_zAxisEntity);
    cleanupEntity(m_xyPlaneEntity);
    cleanupEntity(m_xzPlaneEntity);
    cleanupEntity(m_yzPlaneEntity);
}

void MoveGizmo::render() {
    // 渲染逻辑在 Filament 中处理
}

void MoveGizmo::update() {
    // 更新 Gizmo 位置到选中节点的位置
    if (m_selectionManager && m_selectionManager->getSelectionCount() > 0) {
        Node* activeNode = m_selectionManager->getActiveNode();
        if (activeNode) {
            m_transformOrigin = activeNode->getPosition();
            
            // 更新所有轴线和平面的位置
            auto& transformManager = m_engine->getTransformManager();
            
            auto updateEntityPosition = [&](utils::Entity entity) {
                if (entity.isValid() && transformManager.hasComponent(entity)) {
                    auto instance = transformManager.getInstance(entity);
                    transformManager.setTransform(instance, filament::math::mat4f::translation(m_transformOrigin));
                }
            };
            
            updateEntityPosition(m_xAxisEntity);
            updateEntityPosition(m_yAxisEntity);
            updateEntityPosition(m_zAxisEntity);
            updateEntityPosition(m_xyPlaneEntity);
            updateEntityPosition(m_xzPlaneEntity);
            updateEntityPosition(m_yzPlaneEntity);
        }
    }
}

bool MoveGizmo::intersectRayWithAxis(const filament::math::float3& rayOrigin, const filament::math::float3& rayDirection, Axis axis, filament::math::float3& intersection) {
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
    default:
        return false;
    }
    
    // 射线与线段相交检测
    // 这里使用简化的实现，实际项目中可能需要更复杂的算法
    filament::math::float3 axisDir = axisEnd - axisStart;
    filament::math::float3 rayToAxis = axisStart - rayOrigin;
    
    float a = filament::math::dot(rayDirection, rayDirection);
    float b = 2.0f * filament::math::dot(rayDirection, rayToAxis);
    float c = filament::math::dot(rayToAxis, rayToAxis) - axisLength * axisLength;
    float discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0) {
        return false;
    }
    
    float t = (-b - sqrt(discriminant)) / (2.0f * a);
    if (t < 0) {
        return false;
    }
    
    intersection = rayOrigin + rayDirection * t;
    return true;
}

void MoveGizmo::applyTransform(const filament::math::float3& delta) {
    // 应用移动变换到所有选中的节点
    if (m_selectionManager) {
        const auto& selectedNodes = m_selectionManager->getSelectedNodes();
        for (Node* node : selectedNodes) {
            if (node) {
                filament::math::float3 newPosition = node->getPosition() + delta;
                node->setPosition(newPosition);
            }
        }
    }
}

void MoveGizmo::createGizmoGeometry() {
    // 创建轴线
    createAxis({1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, m_xAxisEntity); // X 轴 - 红色
    createAxis({0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, m_yAxisEntity); // Y 轴 - 绿色
    createAxis({0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, m_zAxisEntity); // Z 轴 - 蓝色
    
    // 创建平面
    createPlane({0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 0.3f}, m_xyPlaneEntity); // XY 平面 - 黄色半透明
    createPlane({0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f, 0.3f}, m_xzPlaneEntity); // XZ 平面 - 紫色半透明
    createPlane({1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 0.3f}, m_yzPlaneEntity); // YZ 平面 - 青色半透明
    
    // 将所有实体添加到场景
    auto addEntityToScene = [this](utils::Entity& entity) {
        if (entity.isValid() && m_scene) {
            m_scene->addEntity(entity);
        }
    };
    
    addEntityToScene(m_xAxisEntity);
    addEntityToScene(m_yAxisEntity);
    addEntityToScene(m_zAxisEntity);
    addEntityToScene(m_xyPlaneEntity);
    addEntityToScene(m_xzPlaneEntity);
    addEntityToScene(m_yzPlaneEntity);
}

void MoveGizmo::createAxis(const filament::math::float3& direction, const filament::math::float3& color, utils::Entity& entity) {
    // 创建轴线实体
    entity = utils::EntityManager::get().create();
    
    // 轴线长度和半径
    float length = 2.0f;
    float radius = 0.05f;
    
    // 创建圆柱体几何体
    // 这里使用简化的实现，实际项目中可能需要更复杂的几何体
    
    // 暂时留空，实际项目中需要创建真实的几何体
}

void MoveGizmo::createPlane(const filament::math::float3& normal, const filament::math::float3& color, utils::Entity& entity) {
    // 创建平面实体
    entity = utils::EntityManager::get().create();
    
    // 平面大小
    float size = 1.0f;
    
    // 创建平面几何体
    // 这里使用简化的实现，实际项目中可能需要更复杂的几何体
    
    // 暂时留空，实际项目中需要创建真实的几何体
}

} // namespace Kazia
