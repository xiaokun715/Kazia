//#include "SceneTreeModel.h"
//
//QModelIndex SceneTreeModel::index(
//    int row, int col, const QModelIndex& parent) const
//{
//    SceneNode* parentNode =
//        parent.isValid()
//        ? static_cast<SceneNode*>(parent.internalPointer())
//        : scene_->root();
//
//    if (row < 0 || row >= parentNode->children.size())
//        return QModelIndex();
//
//    SceneNode* child = parentNode->children[row].get();
//    return createIndex(row, col, child);
//}
//
//QModelIndex SceneTreeModel::parent(const QModelIndex& child) const
//{
//    if (!child.isValid())
//        return QModelIndex();
//
//    SceneNode* node =
//        static_cast<SceneNode*>(child.internalPointer());
//
//    SceneNode* parentNode = node->parent;
//    if (!parentNode || parentNode == scene_->root())
//        return QModelIndex();
//
//    SceneNode* grand = parentNode->parent;
//    int row = indexOf(parentNode, grand);
//
//    return createIndex(row, 0, parentNode);
//}
//
//QVariant SceneTreeModel::data(
//    const QModelIndex& index, int role) const
//{
//    if (!index.isValid())
//        return {};
//
//    SceneNode* node =
//        static_cast<SceneNode*>(index.internalPointer());
//
//    if (role == Qt::DisplayRole)
//        return QString::fromStdString(node->name);
//
//    return {};
//}
