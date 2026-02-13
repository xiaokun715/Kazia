#include "SceneTree.h"

SceneTree::SceneTree(QWidget *parent)
    : QWidget(parent)
    , m_treeWidget(nullptr)
{
    // 创建垂直布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // 创建树控件
    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setHeaderLabel("场景树");
    m_treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // 添加默认根节点
    QTreeWidgetItem* rootItem = new QTreeWidgetItem(m_treeWidget);
    rootItem->setText(0, "场景");
    m_treeWidget->addTopLevelItem(rootItem);

    // 连接信号槽
    connect(m_treeWidget, &QTreeWidget::itemClicked, this, &SceneTree::onItemClicked);

    // 添加到布局
    layout->addWidget(m_treeWidget);

    // 设置布局
    setLayout(layout);
}

SceneTree::~SceneTree()
{
}

void SceneTree::addGameObject(const QString& name)
{
    // 获取根节点
    QTreeWidgetItem* rootItem = m_treeWidget->topLevelItem(0);
    if (!rootItem) {
        rootItem = new QTreeWidgetItem(m_treeWidget);
        rootItem->setText(0, "场景");
        m_treeWidget->addTopLevelItem(rootItem);
    }

    // 创建新的游戏对象节点
    QTreeWidgetItem* gameObjectItem = new QTreeWidgetItem(rootItem);
    gameObjectItem->setText(0, name);

    // 展开根节点
    rootItem->setExpanded(true);
}

void SceneTree::removeGameObject(const QString& name)
{
    // 获取根节点
    QTreeWidgetItem* rootItem = m_treeWidget->topLevelItem(0);
    if (!rootItem) return;

    // 遍历查找并删除节点
    for (int i = 0; i < rootItem->childCount(); ++i) {
        QTreeWidgetItem* childItem = rootItem->child(i);
        if (childItem->text(0) == name) {
            delete childItem;
            break;
        }
    }
}

void SceneTree::clear()
{
    // 清除所有节点
    m_treeWidget->clear();

    // 重新添加根节点
    QTreeWidgetItem* rootItem = new QTreeWidgetItem(m_treeWidget);
    rootItem->setText(0, "场景");
    m_treeWidget->addTopLevelItem(rootItem);
}

void SceneTree::onItemClicked(QTreeWidgetItem* item, int column)
{
    // 发送游戏对象选择信号
    if (item && item != m_treeWidget->topLevelItem(0)) {
        emit gameObjectSelected(item->text(0));
    }
}
