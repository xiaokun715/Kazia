#include "CreateNodeCommand.h"

#include "scene/Node.h"
#include "scene/Scene.h"

namespace Kazia {

CreateNodeCommand::CreateNodeCommand(Scene* scene, Node* parent, const std::string& nodeName) 
    : m_scene(scene), 
      m_parent(parent), 
      m_node(nullptr), 
      m_nodeName(nodeName) {
}

bool CreateNodeCommand::execute() {
    if (!m_scene) {
        return false;
    }
    
    // 创建新节点
    m_node = new Node(m_nodeName);
    
    // 添加到场景或父节点
    if (m_parent) {
        // 添加到父节点
        m_parent->addChild(std::unique_ptr<Node>(m_node));
    } else {
        // 添加到场景根节点
        m_scene->addNode(std::unique_ptr<Node>(m_node));
    }
    
    return true;
}

bool CreateNodeCommand::undo() {
    if (!m_node) {
        return false;
    }
    
    // 从场景或父节点中移除
    if (m_parent) {
        // 从父节点移除
        m_parent->removeChild(m_node);
    } else if (m_scene) {
        // 从场景移除
        m_scene->removeNode(m_node);
    }
    
    // 注意：节点的内存管理由父节点或场景负责，这里不需要手动删除
    m_node = nullptr;
    
    return true;
}

std::string CreateNodeCommand::getName() const {
    return "Create Node Command";
}

} // namespace Kazia
