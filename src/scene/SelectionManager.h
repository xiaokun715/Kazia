#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H

#include <vector>
#include <string>

namespace Kazia {

class Node;

class SelectionManager {
private:
    std::vector<Node*> m_selectedNodes;
    Node* m_activeNode;
    
public:
    SelectionManager();
    ~SelectionManager() = default;
    
    // 选择相关
    void selectNode(Node* node);
    void selectNodes(const std::vector<Node*>& nodes);
    void deselectNode(Node* node);
    void deselectAll();
    
    // 获取选中的节点
    const std::vector<Node*>& getSelectedNodes() const { return m_selectedNodes; }
    Node* getActiveNode() const { return m_activeNode; }
    
    // 检查节点是否被选中
    bool isNodeSelected(Node* node) const;
    
    // 选择数量
    size_t getSelectionCount() const { return m_selectedNodes.size(); }
    
    // 信号（暂时留空，后续与 UI 系统集成）
    // void onSelectionChanged();
};

} // namespace Kazia

#endif // SELECTIONMANAGER_H
