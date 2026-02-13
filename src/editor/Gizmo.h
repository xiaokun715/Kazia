#ifndef GIZMO_H
#define GIZMO_H

#include "core/Math.h"

#include <filament/Engine.h>
#include <filament/Scene.h>
#include <utils/EntityManager.h>

namespace Kazia {

class Node;
class SelectionManager;

class Gizmo {
public:
    enum class GizmoType {
        MOVE,
        ROTATE,
        SCALE
    };
    
    enum class Axis {
        NONE,
        X,
        Y,
        Z,
        PLANE_XY,
        PLANE_XZ,
        PLANE_YZ,
        ALL
    };
    
protected:
    filament::Engine* m_engine;
    filament::Scene* m_scene;
    SelectionManager* m_selectionManager;
    
    utils::Entity m_gizmoEntity;
    GizmoType m_type;
    Axis m_activeAxis;
    
    // 鼠标状态
    bool m_isDragging;
    int m_lastMouseX;
    int m_lastMouseY;
    
    // 变换原点
    math::float3 m_transformOrigin;
    
public:
    Gizmo(filament::Engine* engine, filament::Scene* scene, SelectionManager* selectionManager);
    virtual ~Gizmo();
    
    // 类型相关
    GizmoType getType() const { return m_type; }
    
    // 激活相关
    void setActive(bool active);
    
    // 鼠标事件处理
    virtual bool onMousePress(int x, int y, const filament::math::float3& rayOrigin, const filament::math::float3& rayDirection);
    virtual bool onMouseMove(int x, int y, const filament::math::float3& rayOrigin, const filament::math::float3& rayDirection);
    virtual bool onMouseRelease();
    
    // 渲染相关
    virtual void render() = 0;
    
    // 更新相关
    virtual void update() = 0;
    
protected:
    // 检测射线与轴线的交点
    virtual bool intersectRayWithAxis(const filament::math::float3& rayOrigin, const filament::math::float3& rayDirection, Axis axis, filament::math::float3& intersection) = 0;
    
    // 应用变换
    virtual void applyTransform(const filament::math::float3& delta) = 0;
    
    // 创建 Gizmo 几何体
    virtual void createGizmoGeometry() = 0;
};

} // namespace Kazia

#endif // GIZMO_H
