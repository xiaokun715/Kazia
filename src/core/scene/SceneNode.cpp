#include "SceneNode.h"

SceneNode::SceneNode(SceneNode* parent)
    : m_parent(parent) {
}

SceneNode::~SceneNode() {
    for (auto c : m_children)
        delete c;
}

SceneNode* SceneNode::parent() const {
    return m_parent;
}

SceneNode* SceneNode::child(int row) const {
    if (row < 0 || row >= (int)m_children.size())
        return nullptr;
    return m_children[row];
}

int SceneNode::childCount() const {
    return (int)m_children.size();
}

int SceneNode::row() const {
    if (!m_parent) return 0;
    auto& siblings = m_parent->m_children;
    for (int i = 0; i < (int)siblings.size(); ++i)
        if (siblings[i] == this)
            return i;
    return 0;
}

void SceneNode::appendChild(SceneNode* child) {
    child->m_parent = this;
    m_children.push_back(child);
}

void SceneNode::removeChild(SceneNode* child) {
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end())
        m_children.erase(it);
}