#ifndef TRANSFORMCOMMAND_H
#define TRANSFORMCOMMAND_H

#include "ICommand.h"
#include "core/Math.h"

namespace Kazia {

class Node;

class TransformCommand : public ICommand {
private:
    Node* m_node;
    math::float3 m_oldPosition;
    math::float3 m_newPosition;
    math::float3 m_oldRotation;
    math::float3 m_newRotation;
    math::float3 m_oldScale;
    math::float3 m_newScale;
    
public:
    TransformCommand(Node* node, 
                    const math::float3& oldPosition, 
                    const math::float3& newPosition, 
                    const math::float3& oldRotation, 
                    const math::float3& newRotation, 
                    const math::float3& oldScale, 
                    const math::float3& newScale);
    ~TransformCommand() override = default;
    
    // 执行命令
    bool execute() override;
    
    // 撤销命令
    bool undo() override;
    
    // 获取命令名称
    std::string getName() const override;
};

} // namespace Kazia

#endif // TRANSFORMCOMMAND_H
