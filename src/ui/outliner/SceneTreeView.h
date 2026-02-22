#ifndef SCENE_TREE_VIEW_H
#define SCENE_TREE_VIEW_H

#include <QTreeView>

class SceneTreeView : public QTreeView {
    Q_OBJECT
public:
    explicit SceneTreeView(QWidget* parent = nullptr);

signals:
    void entitySelected(uint32_t entityId);

private slots:
    void onSelectionChanged(const QItemSelection& selected,
        const QItemSelection&);
};

#endif // SCENE_TREE_H