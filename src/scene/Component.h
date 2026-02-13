#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#ifdef _WIN32
    #include <windows.h>
    #include <rpc.h>
#else
    #include <uuid/uuid.h>
#endif

namespace Kazia {

class Node;

class Component {
private:
    std::string m_uuid;
    Node* m_owner;
    
public:
    Component();
    virtual ~Component() = default;
    
    // UUID 相关
    const std::string& getUUID() const { return m_uuid; }
    
    // 所有者相关
    Node* getOwner() const { return m_owner; }
    void setOwner(Node* owner) { m_owner = owner; }
    
    // 生命周期方法
    virtual void initialize() {}
    virtual void update() {}
    virtual void render() {}
    virtual void shutdown() {}
};

} // namespace Kazia

#endif // COMPONENT_H
