#ifndef DELETENODECOMMAND_H
#define DELETENODECOMMAND_H

#include "ICommand.h"

namespace Kazia {

class Node;
class Scene;

class DeleteNodeCommand : public ICommand {
private:
    Scene* m_scene;
    Node* m_node;
    Node* m_parent;
    size_t m_childIndex;
    
public:
    DeleteNodeCommand(Scene* scene, Node* node);
    ~DeleteNodeCommand() override = default;
    
    // 执行命令
    bool execute() override;
    
    // 撤销命令
    bool undo() override;
    
    // 获取命令名称
    std::string getName() const override;
};

} // namespace Kazia

#endif // DELETENODECOMMAND_H
