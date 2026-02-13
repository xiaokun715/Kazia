#ifndef ROTATEGIZMO_H
#define ROTATEGIZMO_H

#include "Gizmo.h"

namespace Kazia {

class RotateGizmo : public Gizmo {
private:
    // 旋转环几何体
    utils::Entity m_xRingEntity;
    utils::Entity m_yRingEntity;
    utils::Entity m_zRingEntity;
    
public:
    RotateGizmo(filament::Engine* engine, filament::Scene* scene, SelectionManager* selectionManager);
    ~RotateGizmo() override;
    
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
    // 创建旋转环
    void createRing(const filament::math::float3& axis, const filament::math::float3& color, utils::Entity& entity);
};

} // namespace Kazia

#endif // ROTATEGIZMO_H
