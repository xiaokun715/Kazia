#include "Scene.h"

namespace Kazia {

Scene::Scene(const std::string& name) : m_name(name) {
    // 创建根节点
    m_rootNode = std::make_unique<Node>("Root");
}

Scene::~Scene() {
    // 根节点会自动清理所有子节点
}

void Scene::addNode(std::unique_ptr<Node> node) {
    if (node) {
        m_rootNode->addChild(std::move(node));
    }
}

void Scene::removeNode(Node* node) {
    if (node && node != m_rootNode.get()) {
        // 从父节点移除
        if (node->getParent()) {
            node->getParent()->removeChild(node);
        }
    }
}

Node* Scene::findNodeByName(const std::string& name) const {
    Node* result = nullptr;
    
    auto findCallback = [](Node* node, void* userData) {
        auto data = reinterpret_cast<std::pair<const std::string*, Node**>>(userData);
        const std::string& name = *(data->first);
        Node** result = data->second;
        
        if (node->getName() == name) {
            *result = node;
        }
    };
    
    std::pair<const std::string*, Node**> userData = {&name, &result};
    m_rootNode->traverse(findCallback, &userData);
    
    return result;
}

Node* Scene::findNodeByUUID(const std::string& uuid) const {
    Node* result = nullptr;
    
    auto findCallback = [](Node* node, void* userData) {
        auto data = reinterpret_cast<std::pair<const std::string*, Node**>>(userData);
        const std::string& uuid = *(data->first);
        Node** result = data->second;
        
        if (node->getUUID() == uuid) {
            *result = node;
        }
    };
    
    std::pair<const std::string*, Node**> userData = {&uuid, &result};
    m_rootNode->traverse(findCallback, &userData);
    
    return result;
}

void Scene::update() {
    // 更新根节点，会递归更新所有子节点
    m_rootNode->update();
}

void Scene::render() {
    // 渲染逻辑会在 Renderer 中处理
}

} // namespace Kazia
