#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "Component.h"

namespace Kazia {

// 简化的 Entity 类型
typedef uint64_t Entity;

class MeshComponent : public Component {
private:
    Entity m_meshEntity;
    std::string m_meshPath;
    
public:
    MeshComponent();
    ~MeshComponent() override = default;
    
    // 网格实体相关
    Entity getMeshEntity() const { return m_meshEntity; }
    void setMeshEntity(Entity entity) { m_meshEntity = entity; }
    
    // 网格路径相关
    const std::string& getMeshPath() const { return m_meshPath; }
    void setMeshPath(const std::string& path) { m_meshPath = path; }
    
    // 生命周期方法
    void initialize() override;
    void update() override;
    void shutdown() override;
};

} // namespace Kazia

#endif // MESHCOMPONENT_H
