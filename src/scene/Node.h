#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <memory>
#ifdef _WIN32
    #include <windows.h>
    #include <rpc.h>
#else
    #include <uuid/uuid.h>
#endif

#include "Component.h"
#include "core/Math.h"

namespace Kazia {

class Node {
private:
    std::string m_name;
    std::string m_uuid;
    
    // 变换属性
    Kazia::math::float3 m_position;
    Kazia::math::float3 m_rotation;
    Kazia::math::float3 m_scale;
    
    // 矩阵
    Kazia::math::mat4f m_localMatrix;
    Kazia::math::mat4f m_worldMatrix;
    
    // 层级结构
    Node* m_parent;
    std::vector<std::unique_ptr<Node>> m_children;
    
    // 组件
    std::vector<std::unique_ptr<Component>> m_components;
    
    // 标记是否需要更新矩阵
    bool m_dirty;
    
public:
    Node(const std::string& name = "Node");
    virtual ~Node();
    
    // UUID 相关
    const std::string& getUUID() const { return m_uuid; }
    
    // 名称相关
    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
    
    // 变换相关
    const Kazia::math::float3& getPosition() const { return m_position; }
    void setPosition(const Kazia::math::float3& position);
    
    const Kazia::math::float3& getRotation() const { return m_rotation; }
    void setRotation(const Kazia::math::float3& rotation);
    
    const Kazia::math::float3& getScale() const { return m_scale; }
    void setScale(const Kazia::math::float3& scale);
    
    // 矩阵相关
    const Kazia::math::mat4f& getLocalMatrix() const { return m_localMatrix; }
    const Kazia::math::mat4f& getWorldMatrix() const { return m_worldMatrix; }
    
    // 层级结构相关
    Node* getParent() const { return m_parent; }
    void setParent(Node* parent);
    
    void addChild(std::unique_ptr<Node> child);
    void removeChild(Node* child);
    size_t getChildCount() const { return m_children.size(); }
    Node* getChild(size_t index) const { return m_children[index].get(); }
    
    // 更新相关
    void update();
    void updateMatrix();
    void setDirty();
    
    // 遍历相关
    void traverse(void (*callback)(Node*, void*), void* userData);
    
    // 组件相关
    template <typename T>
    T* addComponent() {
        auto component = std::make_unique<T>();
        component->setOwner(this);
        component->initialize();
        m_components.push_back(std::move(component));
        return static_cast<T*>(m_components.back().get());
    }
    
    template <typename T>
    T* getComponent() const {
        for (const auto& component : m_components) {
            T* typedComponent = dynamic_cast<T*>(component.get());
            if (typedComponent) {
                return typedComponent;
            }
        }
        return nullptr;
    }
    
    template <typename T>
    std::vector<T*> getComponents() const {
        std::vector<T*> result;
        for (const auto& component : m_components) {
            T* typedComponent = dynamic_cast<T*>(component.get());
            if (typedComponent) {
                result.push_back(typedComponent);
            }
        }
        return result;
    }
    
    void removeComponent(Component* component);
    size_t getComponentCount() const { return m_components.size(); }
    Component* getComponent(size_t index) const { return m_components[index].get(); }
};


#endif // NODE_H
