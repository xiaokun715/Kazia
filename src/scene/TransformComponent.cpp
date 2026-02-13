#include "TransformComponent.h"

namespace Kazia {

TransformComponent::TransformComponent() 
    : m_position({0.0f, 0.0f, 0.0f}), 
      m_rotation({0.0f, 0.0f, 0.0f}), 
      m_scale({1.0f, 1.0f, 1.0f}) {
}

void TransformComponent::setPosition(const math::float3& position) {
    m_position = position;
    
    // 同步到节点
    if (getOwner()) {
        getOwner()->setPosition(position);
    }
}

void TransformComponent::setRotation(const math::float3& rotation) {
    m_rotation = rotation;
    
    // 同步到节点
    if (getOwner()) {
        getOwner()->setRotation(rotation);
    }
}

void TransformComponent::setScale(const math::float3& scale) {
    m_scale = scale;
    
    // 同步到节点
    if (getOwner()) {
        getOwner()->setScale(scale);
    }
}

void TransformComponent::update() {
    // 从节点同步变换
    if (getOwner()) {
        m_position = getOwner()->getPosition();
        m_rotation = getOwner()->getRotation();
        m_scale = getOwner()->getScale();
    }
}

} // namespace Kazia
