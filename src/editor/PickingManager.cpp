#include "PickingManager.h"

#include "scene/Node.h"
#include "scene/Scene.h"
#include "scene/SelectionManager.h"

#include <limits>

namespace Kazia {

PickingManager::PickingManager(Engine* engine, Scene* scene, SelectionManager* selectionManager) 
    : m_engine(engine), 
      m_camera(nullptr), 
      m_scene(scene), 
      m_selectionManager(selectionManager), 
      m_screenWidth(0), 
      m_screenHeight(0) {
}

Node* PickingManager::pickObject(int x, int y) {
    if (!m_camera || !m_scene) {
        return nullptr;
    }
    
    // 屏幕坐标转射线
    math::float3 rayOrigin, rayDirection;
    screenToRay(x, y, rayOrigin, rayDirection);
    
    // 递归检测射线与场景节点的相交
    float closestDistance = std::numeric_limits<float>::max();
    Node* pickedNode = intersectRayWithScene(rayOrigin, rayDirection, m_scene->getRootNode(), closestDistance);
    
    return pickedNode;
}

bool PickingManager::intersectRayWithAABB(const math::float3& rayOrigin, 
                                          const math::float3& rayDirection, 
                                          const math::float3& aabbMin, 
                                          const math::float3& aabbMax, 
                                          float& tMin, 
                                          float& tMax) {
    // 使用 Slab 方法检测射线与 AABB 的相交
    math::float3 invRayDir = {1.0f / rayDirection.x, 1.0f / rayDirection.y, 1.0f / rayDirection.z};
    
    math::float3 t1 = {(aabbMin.x - rayOrigin.x) * invRayDir.x, 
                       (aabbMin.y - rayOrigin.y) * invRayDir.y, 
                       (aabbMin.z - rayOrigin.z) * invRayDir.z};
    math::float3 t2 = {(aabbMax.x - rayOrigin.x) * invRayDir.x, 
                       (aabbMax.y - rayOrigin.y) * invRayDir.y, 
                       (aabbMax.z - rayOrigin.z) * invRayDir.z};
    
    math::float3 tMinVec = {std::min(t1.x, t2.x), std::min(t1.y, t2.y), std::min(t1.z, t2.z)};
    math::float3 tMaxVec = {std::max(t1.x, t2.x), std::max(t1.y, t2.y), std::max(t1.z, t2.z)};
    
    tMin = std::max({tMinVec.x, tMinVec.y, tMinVec.z, 0.0f});
    tMax = std::min({tMaxVec.x, tMaxVec.y, tMaxVec.z});
    
    return tMin <= tMax;
}

bool PickingManager::intersectRayWithMesh(const math::float3& rayOrigin, 
                                          const math::float3& rayDirection, 
                                          Node* node, 
                                          math::float3& intersection) {
    // 简化实现：使用 AABB 代替网格
    // 实际项目中需要实现更复杂的射线与三角形相交检测
    
    // 假设节点的 AABB 为中心在原点，大小为 1 的立方体
    math::float3 aabbMin = {-0.5f, -0.5f, -0.5f};
    math::float3 aabbMax = {0.5f, 0.5f, 0.5f};
    
    // 考虑节点的变换
    math::mat4f worldMatrix = node->getWorldMatrix();
    
    // 计算 AABB 在世界空间中的范围
    // 这里使用简化的方法，实际项目中需要正确计算变换后的 AABB
    math::float3 nodePos = node->getPosition();
    aabbMin = {aabbMin.x + nodePos.x, aabbMin.y + nodePos.y, aabbMin.z + nodePos.z};
    aabbMax = {aabbMax.x + nodePos.x, aabbMax.y + nodePos.y, aabbMax.z + nodePos.z};
    
    // 检测射线与 AABB 的相交
    float tMin, tMax;
    if (intersectRayWithAABB(rayOrigin, rayDirection, aabbMin, aabbMax, tMin, tMax)) {
        // 计算交点
        intersection = {rayOrigin.x + rayDirection.x * tMin, 
                        rayOrigin.y + rayDirection.y * tMin, 
                        rayOrigin.z + rayDirection.z * tMin};
        return true;
    }
    
    return false;
}

void PickingManager::screenToRay(int x, int y, math::float3& rayOrigin, math::float3& rayDirection) {
    if (!m_camera) {
        return;
    }
    
    // 屏幕坐标归一化到 [-1, 1] 范围
    float nx = (2.0f * x) / m_screenWidth - 1.0f;
    float ny = 1.0f - (2.0f * y) / m_screenHeight;
    
    // 简化实现：使用默认的相机参数
    rayOrigin = {0.0f, 0.0f, 5.0f};
    rayDirection = {nx, ny, -1.0f};
}

void PickingManager::worldToScreen(const math::float3& worldPos, math::float2& screenPos) {
    if (!m_camera) {
        return;
    }
    
    // 简化实现：使用默认的相机参数
    screenPos = {(worldPos.x + 1.0f) * 0.5f * m_screenWidth, 
                 (1.0f - worldPos.y) * 0.5f * m_screenHeight};
}

Node* PickingManager::intersectRayWithScene(const math::float3& rayOrigin, 
                                           const math::float3& rayDirection, 
                                           Node* node, 
                                           float& closestDistance) {
    if (!node) {
        return nullptr;
    }
    
    // 检测射线与当前节点的相交
    math::float3 intersection;
    bool hit = intersectRayWithMesh(rayOrigin, rayDirection, node, intersection);
    
    Node* pickedNode = nullptr;
    float nodeDistance = 0.0f;
    
    if (hit) {
        // 计算交点到射线原点的距离
        nodeDistance = std::sqrt(std::pow(intersection.x - rayOrigin.x, 2) + 
                                std::pow(intersection.y - rayOrigin.y, 2) + 
                                std::pow(intersection.z - rayOrigin.z, 2));
        if (nodeDistance < closestDistance) {
            closestDistance = nodeDistance;
            pickedNode = node;
        }
    }
    
    // 递归检测子节点
    for (size_t i = 0; i < node->getChildCount(); ++i) {
        Node* child = node->getChild(i);
        float childClosestDistance = closestDistance;
        Node* childPickedNode = intersectRayWithScene(rayOrigin, rayDirection, child, childClosestDistance);
        
        if (childPickedNode && childClosestDistance < closestDistance) {
            closestDistance = childClosestDistance;
            pickedNode = childPickedNode;
        }
    }
    
    return pickedNode;
}

} // namespace Kazia
