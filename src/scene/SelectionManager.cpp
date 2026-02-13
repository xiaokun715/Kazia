#include "SelectionManager.h"

#include "Node.h"
#include <algorithm>

namespace Kazia {

SelectionManager::SelectionManager() : m_activeNode(nullptr) {
}

void SelectionManager::selectNode(Node* node) {
    if (!node) {
        return;
    }
    
    // 检查节点是否已经被选中
    if (isNodeSelected(node)) {
        return;
    }
    
    // 添加到选中列表
    m_selectedNodes.push_back(node);
    
    // 设置为活动节点
    m_activeNode = node;
    
    // 触发选择变化信号
    // onSelectionChanged();
}

void SelectionManager::selectNodes(const std::vector<Node*>& nodes) {
    // 清空当前选择
    deselectAll();
    
    // 添加新的选择
    for (Node* node : nodes) {
        if (node) {
            m_selectedNodes.push_back(node);
        }
    }
    
    // 设置活动节点为第一个选中的节点
    if (!m_selectedNodes.empty()) {
        m_activeNode = m_selectedNodes[0];
    } else {
        m_activeNode = nullptr;
    }
    
    // 触发选择变化信号
    // onSelectionChanged();
}

void SelectionManager::deselectNode(Node* node) {
    if (!node) {
        return;
    }
    
    // 从选中列表中移除
    auto it = std::find(m_selectedNodes.begin(), m_selectedNodes.end(), node);
    if (it != m_selectedNodes.end()) {
        m_selectedNodes.erase(it);
    }
    
    // 如果移除的是活动节点，重新设置活动节点
    if (node == m_activeNode) {
        if (!m_selectedNodes.empty()) {
            m_activeNode = m_selectedNodes[0];
        } else {
            m_activeNode = nullptr;
        }
    }
    
    // 触发选择变化信号
    // onSelectionChanged();
}

void SelectionManager::deselectAll() {
    m_selectedNodes.clear();
    m_activeNode = nullptr;
    
    // 触发选择变化信号
    // onSelectionChanged();
}

bool SelectionManager::isNodeSelected(Node* node) const {
    if (!node) {
        return false;
    }
    
    auto it = std::find(m_selectedNodes.begin(), m_selectedNodes.end(), node);
    return it != m_selectedNodes.end();
}

} // namespace Kazia
