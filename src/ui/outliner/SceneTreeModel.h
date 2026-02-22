#ifndef SCENE_TREE_MODEL_H
#define SCENE_TREE_MODEL_H

#include <QAbstractItemModel>

class Scene;

class SceneTreeModel : public QAbstractItemModel {
    Q_OBJECT
public:
    explicit SceneTreeModel(Scene* scene, QObject* parent = nullptr);

    QModelIndex index(int row, int col,
        const QModelIndex& parent) const override;

    QModelIndex parent(const QModelIndex& child) const override;

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex&) const override { return 1; }

    QVariant data(const QModelIndex& index, int role) const override;

private:
    Scene* scene_;
};


#endif // SCENE_TREE_MODEL_H
