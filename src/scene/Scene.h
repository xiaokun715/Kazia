#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include <memory>

#include "Node.h"

namespace Kazia {

class Scene {
private:
    std::string m_name;
    std::unique_ptr<Node> m_rootNode;
    
public:
    Scene(const std::string& name = "Scene");
    ~Scene();
    
    // 根节点相关
    Node* getRootNode() const { return m_rootNode.get(); }
    
    // 添加/移除节点
    void addNode(std::unique_ptr<Node> node);
    void removeNode(Node* node);
    
    // 查找节点
    Node* findNodeByName(const std::string& name) const;
    Node* findNodeByUUID(const std::string& uuid) const;
    
    // 更新和渲染
    void update();
    void render();
    
    // 名称相关
    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
};

} // namespace Kazia

#endif // SCENE_H
