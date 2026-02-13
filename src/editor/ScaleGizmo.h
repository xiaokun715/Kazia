#ifndef SCALEGIZMO_H
#define SCALEGIZMO_H

#include "Gizmo.h"

namespace Kazia {

class ScaleGizmo : public Gizmo {
private:
    // 缩放手柄几何体
    utils::Entity m_xHandleEntity;
    utils::Entity m_yHandleEntity;
    utils::Entity m_zHandleEntity;
    utils::Entity m_uniformHandleEntity;
    
public:
    ScaleGizmo(filament::Engine* engine, filament::Scene* scene, SelectionManager* selectionManager);
    ~ScaleGizmo() override;
    
    // 渲染相关
    void render() override;
    
    // 更新相关
    void update() override;
    
protected:
    // 检测射线与轴线的交点
    bool intersectRayWithAxis(const filament::math::float3& rayOrigin, const filament::math::float3& rayDirection, Axis axis, filament::math::float3& intersection) override;
    
    // 应用变换
    void applyTransform(const filament::math::float3& delta) override;
    
    // 创建 Gizmo 几何体
    void createGizmoGeometry() override;
    
private:
    // 创建缩放手柄
    void createHandle(const filament::math::float3& direction, const filament::math::float3& color, utils::Entity& entity);
};

} // namespace Kazia

#endif // SCALEGIZMO_H
