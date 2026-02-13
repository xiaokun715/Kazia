#include "TransformCommand.h"

#include "scene/Node.h"

namespace Kazia {

TransformCommand::TransformCommand(Node* node, 
                                 const math::float3& oldPosition, 
                                 const math::float3& newPosition, 
                                 const math::float3& oldRotation, 
                                 const math::float3& newRotation, 
                                 const math::float3& oldScale, 
                                 const math::float3& newScale) 
    : m_node(node), 
      m_oldPosition(oldPosition), 
      m_newPosition(newPosition), 
      m_oldRotation(oldRotation), 
      m_newRotation(newRotation), 
      m_oldScale(oldScale), 
      m_newScale(newScale) {
}

bool TransformCommand::execute() {
    if (!m_node) {
        return false;
    }
    
    // 应用新的变换
    m_node->setPosition(m_newPosition);
    m_node->setRotation(m_newRotation);
    m_node->setScale(m_newScale);
    
    return true;
}

bool TransformCommand::undo() {
    if (!m_node) {
        return false;
    }
    
    // 恢复旧的变换
    m_node->setPosition(m_oldPosition);
    m_node->setRotation(m_oldRotation);
    m_node->setScale(m_oldScale);
    
    return true;
}

std::string TransformCommand::getName() const {
    return "Transform Command";
}

} // namespace Kazia
