#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#pragma once
#include <vector>
#include <QString>
#include <utils/Entity.h>

class SceneNode {
public:
    explicit SceneNode(SceneNode* parent = nullptr);
    ~SceneNode();

    // hierarchy
    SceneNode* parent() const;
    SceneNode* child(int row) const;
    int childCount() const;
    int row() const;

    void appendChild(SceneNode* child);
    void removeChild(SceneNode* child);

    // data
    QString name;
    utils::Entity entity;

private:
    SceneNode* m_parent = nullptr;
    std::vector<SceneNode*> m_children;
};

#endif // SCENE_NODE_H
