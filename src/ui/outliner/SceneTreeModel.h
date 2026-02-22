#ifndef SCENE_TREE_MODEL_H
#define SCENE_TREE_MODEL_H

#include <QAbstractItemModel>
#include "core/scene/SceneNode.h"

class SceneTreeModel : public QAbstractItemModel {
    Q_OBJECT
public:
    explicit SceneTreeModel(QObject* parent = nullptr);
    ~SceneTreeModel() override;

    QModelIndex index(int row, int column,
        const QModelIndex& parent) const override;

    QModelIndex parent(const QModelIndex& index) const override;

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex&) const override { return 1; }

    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation, int role) const override;

    // API
    SceneNode* root() const;
    SceneNode* nodeFromIndex(const QModelIndex& index) const;

    void addEntity(utils::Entity entity,
        const QString& name,
        SceneNode* parent = nullptr);

private:
    SceneNode* m_root = nullptr;
};

#endif // SCENE_TREE_MODEL_H
