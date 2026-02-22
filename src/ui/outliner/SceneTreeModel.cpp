#include "SceneTreeModel.h"

SceneTreeModel::SceneTreeModel(QObject* parent)
    : QAbstractItemModel(parent) {
    m_root = new SceneNode();
    m_root->name = "Scene";
}

SceneTreeModel::~SceneTreeModel() {
    delete m_root;
}

QModelIndex SceneTreeModel::index(int row, int column,
    const QModelIndex& parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    SceneNode* parentNode =
        parent.isValid() ? nodeFromIndex(parent) : m_root;

    SceneNode* child = parentNode->child(row);
    if (child)
        return createIndex(row, column, child);

    return QModelIndex();
}

QModelIndex SceneTreeModel::parent(const QModelIndex& index) const {
    if (!index.isValid())
        return QModelIndex();

    auto node = nodeFromIndex(index);
    auto parentNode = node->parent();

    if (!parentNode || parentNode == m_root)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}

int SceneTreeModel::rowCount(const QModelIndex& parent) const {
    SceneNode* parentNode =
        parent.isValid() ? nodeFromIndex(parent) : m_root;

    return parentNode->childCount();
}

QVariant SceneTreeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return {};

    auto node = nodeFromIndex(index);

    if (role == Qt::DisplayRole)
        return node->name;

    if (role == Qt::UserRole)
        return QVariant::fromValue<uint32_t>(node->entity.getId());

    return {};
}

QVariant SceneTreeModel::headerData(int section,
    Qt::Orientation orientation,
    int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0)
        return "Scene";

    return {};
}

SceneNode* SceneTreeModel::root() const {
    return m_root;
}

SceneNode* SceneTreeModel::nodeFromIndex(const QModelIndex& index) const {
    return index.isValid()
        ? static_cast<SceneNode*>(index.internalPointer())
        : m_root;
}

void SceneTreeModel::addEntity(utils::Entity entity,
    const QString& name,
    SceneNode* parent) {
    if (!parent) parent = m_root;

    int row = parent->childCount();
    beginInsertRows(createIndex(parent->row(), 0, parent), row, row);

    auto* node = new SceneNode(parent);
    node->entity = entity;
    node->name = name;

    parent->appendChild(node);
    endInsertRows();
}