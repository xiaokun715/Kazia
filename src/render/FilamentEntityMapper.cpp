#include "FilamentEntityMapper.h"

#include "scene/Node.h"

namespace Kazia {

FilamentEntityMapper::FilamentEntityMapper(filament::Engine* engine) : m_engine(engine) {
}

void FilamentEntityMapper::addMapping(const std::string& nodeUUID, utils::Entity entity) {
    m_nodeToEntityMap[nodeUUID] = entity;
    m_entityToNodeMap[entity] = nodeUUID;
}

void FilamentEntityMapper::removeMapping(const std::string& nodeUUID) {
    auto it = m_nodeToEntityMap.find(nodeUUID);
    if (it != m_nodeToEntityMap.end()) {
        utils::Entity entity = it->second;
        m_entityToNodeMap.erase(entity);
        m_nodeToEntityMap.erase(it);
    }
}

void FilamentEntityMapper::removeMapping(utils::Entity entity) {
    auto it = m_entityToNodeMap.find(entity);
    if (it != m_entityToNodeMap.end()) {
        std::string nodeUUID = it->second;
        m_nodeToEntityMap.erase(nodeUUID);
        m_entityToNodeMap.erase(it);
    }
}

utils::Entity FilamentEntityMapper::getEntity(const std::string& nodeUUID) const {
    auto it = m_nodeToEntityMap.find(nodeUUID);
    if (it != m_nodeToEntityMap.end()) {
        return it->second;
    }
    return utils::Entity::INVALID;
}

std::string FilamentEntityMapper::getNodeUUID(utils::Entity entity) const {
    auto it = m_entityToNodeMap.find(entity);
    if (it != m_entityToNodeMap.end()) {
        return it->second;
    }
    return "";
}

void FilamentEntityMapper::syncTransform(const Node* node) {
    if (!node || !m_engine) {
        return;
    }
    
    utils::Entity entity = getEntity(node->getUUID());
    if (!entity.isValid()) {
        return;
    }
    
    // 获取 TransformManager 并同步变换
    auto& transformManager = m_engine->getTransformManager();
    if (transformManager.hasComponent(entity)) {
        auto instance = transformManager.getInstance(entity);
        
        // 转换 math::mat4f 到 filament::math::mat4f
        const auto& worldMatrix = node->getWorldMatrix();
        filament::math::mat4f filaMatrix;
        for (int i = 0; i < 16; i++) {
            filaMatrix[i] = worldMatrix.m[i];
        }
        
        transformManager.setTransform(instance, filaMatrix);
    }
}

void FilamentEntityMapper::syncAllTransforms(const Node* rootNode) {
    if (!rootNode) {
        return;
    }
    
    // 同步根节点
    syncTransform(rootNode);
    
    // 递归同步所有子节点
    for (size_t i = 0; i < rootNode->getChildCount(); ++i) {
        syncAllTransforms(rootNode->getChild(i));
    }
}

void FilamentEntityMapper::syncMeshComponent(const Node* node) {
    if (!node || !m_engine) {
        return;
    }
    
    // 获取 MeshComponent
    auto* meshComponent = node->getComponent<MeshComponent>();
    if (!meshComponent) {
        return;
    }
    
    // 获取对应的 Filament 实体
    utils::Entity entity = getEntity(node->getUUID());
    if (!entity.isValid()) {
        return;
    }
    
    // 这里需要与 Filament 的 RenderableManager 进行交互
    // 暂时留空，后续集成 Filament 后实现
}

void FilamentEntityMapper::syncCameraComponent(const Node* node) {
    if (!node || !m_engine) {
        return;
    }
    
    // 获取 CameraComponent
    auto* cameraComponent = node->getComponent<CameraComponent>();
    if (!cameraComponent) {
        return;
    }
    
    // 获取对应的 Filament 实体
    utils::Entity entity = getEntity(node->getUUID());
    if (!entity.isValid()) {
        return;
    }
    
    // 这里需要与 Filament 的 CameraManager 进行交互
    // 暂时留空，后续集成 Filament 后实现
}

void FilamentEntityMapper::syncLightComponent(const Node* node) {
    if (!node || !m_engine) {
        return;
    }
    
    // 获取 LightComponent
    auto* lightComponent = node->getComponent<LightComponent>();
    if (!lightComponent) {
        return;
    }
    
    // 获取对应的 Filament 实体
    utils::Entity entity = getEntity(node->getUUID());
    if (!entity.isValid()) {
        return;
    }
    
    // 这里需要与 Filament 的 LightManager 进行交互
    // 暂时留空，后续集成 Filament 后实现
}

void FilamentEntityMapper::syncAllComponents(const Node* rootNode) {
    if (!rootNode) {
        return;
    }
    
    // 同步根节点的组件
    syncMeshComponent(rootNode);
    syncCameraComponent(rootNode);
    syncLightComponent(rootNode);
    
    // 递归同步所有子节点的组件
    for (size_t i = 0; i < rootNode->getChildCount(); ++i) {
        syncAllComponents(rootNode->getChild(i));
    }
}

void FilamentEntityMapper::clear() {
    m_nodeToEntityMap.clear();
    m_entityToNodeMap.clear();
}

} // namespace Kazia
