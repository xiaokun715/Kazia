#include "Node.h"

#include <sstream>

namespace Kazia {

Node::Node(const std::string& name) 
    : m_name(name), 
      m_position({0.0f, 0.0f, 0.0f}), 
      m_rotation({0.0f, 0.0f, 0.0f}), 
      m_scale({1.0f, 1.0f, 1.0f}), 
      m_parent(nullptr), 
      m_dirty(true)
{
    // 生成 UUID
    char uuid_str[37];
    #ifdef _WIN32
        RPC_STATUS status;
        UUID uuid;
        status = UuidCreate(&uuid);
        if (status == RPC_S_OK || status == RPC_S_UUID_LOCAL_ONLY)
        {
            unsigned char* uuid_str_ptr;
            status = UuidToStringA(&uuid, &uuid_str_ptr);
            if (status == RPC_S_OK)
            {
                strcpy_s(uuid_str, sizeof(uuid_str), (char*)uuid_str_ptr);
                RpcStringFreeA(&uuid_str_ptr);
            }
        }
    #else
        uuid_t uuid;
        uuid_generate(uuid);
        uuid_unparse(uuid, uuid_str);
    #endif
    m_uuid = uuid_str;
    
    // 初始化矩阵
    updateMatrix();
}

Node::~Node() {
    // 清理所有组件
    for (auto& component : m_components) {
        component->shutdown();
    }
    m_components.clear();
    
    // 移除所有子节点
    m_children.clear();
}

void Node::setPosition(const math::float3& position) {
    m_position = position;
    setDirty();
}

void Node::setRotation(const math::float3& rotation) {
    m_rotation = rotation;
    setDirty();
}

void Node::setScale(const math::float3& scale) {
    m_scale = scale;
    setDirty();
}

void Node::setParent(Node* parent) {
    if (m_parent == parent) {
        return;
    }
    
    // 从旧父节点移除
    if (m_parent) {
        m_parent->removeChild(this);
    }
    
    // 设置新父节点
    m_parent = parent;
    
    // 添加到新父节点
    if (m_parent) {
        auto it = std::find_if(m_parent->m_children.begin(), m_parent->m_children.end(), 
            [this](const std::unique_ptr<Node>& child) { return child.get() == this; });
        
        if (it == m_parent->m_children.end()) {
            // 如果还没有在子节点列表中，添加一个新的
            m_parent->m_children.push_back(std::unique_ptr<Node>(this));
        }
    }
    
    setDirty();
}

void Node::addChild(std::unique_ptr<Node> child) {
    if (child) {
        child->setParent(this);
        m_children.push_back(std::move(child));
    }
}

void Node::removeChild(Node* child) {
    auto it = std::find_if(m_children.begin(), m_children.end(), 
        [child](const std::unique_ptr<Node>& c) { return c.get() == child; });
    
    if (it != m_children.end()) {
        (*it)->m_parent = nullptr;
        m_children.erase(it);
    }
}

void Node::update() {
    if (m_dirty) {
        updateMatrix();
    }
    
    // 更新所有组件
    for (auto& component : m_components) {
        component->update();
    }
    
    // 更新所有子节点
    for (auto& child : m_children) {
        child->update();
    }
}

// 辅助函数：创建平移矩阵
math::mat4f translationMatrix(const math::float3& translation) {
    math::mat4f mat;
    mat.m[12] = translation.x;
    mat.m[13] = translation.y;
    mat.m[14] = translation.z;
    return mat;
}

// 辅助函数：创建缩放矩阵
math::mat4f scalingMatrix(const math::float3& scale) {
    math::mat4f mat;
    mat.m[0] = scale.x;
    mat.m[5] = scale.y;
    mat.m[10] = scale.z;
    return mat;
}

// 辅助函数：矩阵乘法
math::mat4f multiplyMatrices(const math::mat4f& a, const math::mat4f& b) {
    math::mat4f result;
    // 实现 4x4 矩阵乘法
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i * 4 + j] = 
                a.m[i * 4 + 0] * b.m[0 * 4 + j] +
                a.m[i * 4 + 1] * b.m[1 * 4 + j] +
                a.m[i * 4 + 2] * b.m[2 * 4 + j] +
                a.m[i * 4 + 3] * b.m[3 * 4 + j];
        }
    }
    return result;
}

void Node::updateMatrix() {
    // 计算本地矩阵
    math::mat4f translationMat = translationMatrix(m_position);
    math::mat4f scalingMat = scalingMatrix(m_scale);
    m_localMatrix = multiplyMatrices(translationMat, scalingMat);
    
    // 计算世界矩阵
    if (m_parent) {
        m_worldMatrix = multiplyMatrices(m_parent->getWorldMatrix(), m_localMatrix);
    } else {
        m_worldMatrix = m_localMatrix;
    }
    
    m_dirty = false;
    
    // 标记所有子节点为脏
    for (auto& child : m_children) {
        child->setDirty();
    }
}

void Node::setDirty() {
    m_dirty = true;
    
    // 标记所有子节点为脏
    for (auto& child : m_children) {
        child->setDirty();
    }
}

void Node::traverse(void (*callback)(Node*, void*), void* userData) {
    // 先调用当前节点的回调
    callback(this, userData);
    
    // 再遍历子节点
    for (auto& child : m_children) {
        child->traverse(callback, userData);
    }
}

void Node::removeComponent(Component* component) {
    auto it = std::find_if(m_components.begin(), m_components.end(), 
        [component](const std::unique_ptr<Component>& c) { return c.get() == component; });
    
    if (it != m_components.end()) {
        (*it)->shutdown();
        m_components.erase(it);
    }
}

} // namespace Kazia
