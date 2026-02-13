#ifndef MOVEGIZMO_H
#define MOVEGIZMO_H

#include "Gizmo.h"

namespace Kazia {

class MoveGizmo : public Gizmo {
private:
    // 轴线几何体
    utils::Entity m_xAxisEntity;
    utils::Entity m_yAxisEntity;
    utils::Entity m_zAxisEntity;
    
    // 平面几何体
    utils::Entity m_xyPlaneEntity;
    utils::Entity m_xzPlaneEntity;
    utils::Entity m_yzPlaneEntity;
    
public:
    MoveGizmo(filament::Engine* engine, filament::Scene* scene, SelectionManager* selectionManager);
    ~MoveGizmo() override;
    
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
    // 创建轴线
    void createAxis(const filament::math::float3& direction, const filament::math::float3& color, utils::Entity& entity);
    
    // 创建平面
    void createPlane(const filament::math::float3& normal, const filament::math::float3& color, utils::Entity& entity);
};

} // namespace Kazia

#endif // MOVEGIZMO_H
