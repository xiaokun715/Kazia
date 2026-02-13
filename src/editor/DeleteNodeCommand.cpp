#include "DeleteNodeCommand.h"

#include "scene/Node.h"
#include "scene/Scene.h"

namespace Kazia {

DeleteNodeCommand::DeleteNodeCommand(Scene* scene, Node* node) 
    : m_scene(scene), 
      m_node(node), 
      m_parent(nullptr), 
      m_childIndex(0) {
    if (m_node) {
        // 保存父节点和子索引
        m_parent = m_node->getParent();
        if (m_parent) {
            // 查找节点在父节点中的索引
            for (size_t i = 0; i < m_parent->getChildCount(); ++i) {
                if (m_parent->getChild(i) == m_node) {
                    m_childIndex = i;
                    break;
                }
            }
        }
    }
}

bool DeleteNodeCommand::execute() {
    if (!m_node) {
        return false;
    }
    
    // 从父节点或场景中移除
    if (m_parent) {
        // 从父节点移除
        m_parent->removeChild(m_node);
    } else if (m_scene) {
        // 从场景移除
        m_scene->removeNode(m_node);
    }
    
    return true;
}

bool DeleteNodeCommand::undo() {
    if (!m_node) {
        return false;
    }
    
    // 将节点重新添加到原来的位置
    if (m_parent) {
        // 添加到父节点
        // 注意：这里需要重新创建 unique_ptr，因为原来的已经被销毁
        m_parent->addChild(std::unique_ptr<Node>(m_node));
    } else if (m_scene) {
        // 添加到场景
        m_scene->addNode(std::unique_ptr<Node>(m_node));
    }
    
    // 注意：节点的内存管理现在由父节点或场景负责
    m_node = nullptr;
    
    return true;
}

std::string DeleteNodeCommand::getName() const {
    return "Delete Node Command";
}

} // namespace Kazia
