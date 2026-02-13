#ifndef PICKINGMANAGER_H
#define PICKINGMANAGER_H

#include <vector>
#include <memory>
#include "core/Math.h"

namespace Kazia {

class Node;
class Scene;
class SelectionManager;

// 简化的 Engine 类
class Engine {
public:
    Engine() {}
};

// 简化的 Camera 类
class Camera {
public:
    void getProjectionMatrix(float* matrix) {}
    void getModelMatrix(float* matrix) {}
};

class PickingManager {
private:
    Engine* m_engine;
    Camera* m_camera;
    Scene* m_scene;
    SelectionManager* m_selectionManager;
    
    // 屏幕信息
    int m_screenWidth;
    int m_screenHeight;
    
public:
    PickingManager(Engine* engine, Scene* scene, SelectionManager* selectionManager);
    ~PickingManager() = default;
    
    // 设置相机
    void setCamera(Camera* camera) { m_camera = camera; }
    
    // 设置屏幕大小
    void setScreenSize(int width, int height) {
        m_screenWidth = width;
        m_screenHeight = height;
    }
    
    // 从屏幕坐标选择对象
    Node* pickObject(int x, int y);
    
    // 射线与 AABB 相交检测
    bool intersectRayWithAABB(const math::float3& rayOrigin, 
                              const math::float3& rayDirection, 
                              const math::float3& aabbMin, 
                              const math::float3& aabbMax, 
                              float& tMin, 
                              float& tMax);
    
    // 射线与网格相交检测
    bool intersectRayWithMesh(const math::float3& rayOrigin, 
                              const math::float3& rayDirection, 
                              Node* node, 
                              math::float3& intersection);
    
    // 屏幕坐标转射线
    void screenToRay(int x, int y, math::float3& rayOrigin, math::float3& rayDirection);
    
    // 世界坐标转屏幕坐标
    void worldToScreen(const math::float3& worldPos, math::float2& screenPos);
    
private:
    // 递归检测射线与场景节点的相交
    Node* intersectRayWithScene(const math::float3& rayOrigin, 
                                const math::float3& rayDirection, 
                                Node* node, 
                                float& closestDistance);
};

} // namespace Kazia

#endif // PICKINGMANAGER_H
