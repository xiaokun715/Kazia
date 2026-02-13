#ifndef CREATENODECOMMAND_H
#define CREATENODECOMMAND_H

#include "ICommand.h"

#include <string>

namespace Kazia {

class Node;
class Scene;

class CreateNodeCommand : public ICommand {
private:
    Scene* m_scene;
    Node* m_parent;
    Node* m_node;
    std::string m_nodeName;
    
public:
    CreateNodeCommand(Scene* scene, Node* parent, const std::string& nodeName);
    ~CreateNodeCommand() override = default;
    
    // 执行命令
    bool execute() override;
    
    // 撤销命令
    bool undo() override;
    
    // 获取命令名称
    std::string getName() const override;
    
    // 获取创建的节点
    Node* getNode() const { return m_node; }
};

} // namespace Kazia

#endif // CREATENODECOMMAND_H
