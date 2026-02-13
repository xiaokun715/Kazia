#ifndef FILAMENTENTITYMAPPER_H
#define FILAMENTENTITYMAPPER_H

#include <string>
#include <unordered_map>

#include <filament/Engine.h>
#include <filament/TransformManager.h>

namespace Kazia {

class Node;

class FilamentEntityMapper {
private:
    filament::Engine* m_engine;
    
    // 映射表
    std::unordered_map<std::string, utils::Entity> m_nodeToEntityMap;
    std::unordered_map<utils::Entity, std::string> m_entityToNodeMap;
    
public:
    FilamentEntityMapper(filament::Engine* engine);
    ~FilamentEntityMapper() = default;
    
    // 映射管理
    void addMapping(const std::string& nodeUUID, utils::Entity entity);
    void removeMapping(const std::string& nodeUUID);
    void removeMapping(utils::Entity entity);
    
    // 查询映射
    utils::Entity getEntity(const std::string& nodeUUID) const;
    std::string getNodeUUID(utils::Entity entity) const;
    
    // 同步方法
    void syncTransform(const Node* node);
    void syncAllTransforms(const Node* rootNode);
    void syncMeshComponent(const Node* node);
    void syncCameraComponent(const Node* node);
    void syncLightComponent(const Node* node);
    void syncAllComponents(const Node* rootNode);
    
    // 清理方法
    void clear();
};

} // namespace Kazia

#endif // FILAMENTENTITYMAPPER_H
