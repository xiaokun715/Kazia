//#ifndef SCENETREE_H
//#define SCENETREE_H
//
//#include <QWidget>
//#include <QTreeWidget>
//#include <QVBoxLayout>
//#include "core/Renderer.h"
//
//class SceneTree : public QWidget
//{
//    Q_OBJECT
//
//private:
//    QTreeWidget* m_treeWidget;
//	Renderer* m_renderer;
//
//public:
//    SceneTree(Renderer* renderer, QWidget *parent = nullptr);
//    ~SceneTree();
//
//    QTreeWidget* getTreeWidget() const { return m_treeWidget; }
//
//    void addGameObject(const QString& name);
//    void removeGameObject(const QString& name);
//    void clear();
//
//    void addGltfModel(const QString& fileName);
//
//signals:
//    void gameObjectSelected(const QString& name);
//
//private slots:
//    void onItemClicked(QTreeWidgetItem* item, int column);
//};
//
//#endif // SCENETREE_H
