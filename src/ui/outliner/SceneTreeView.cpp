#include "SceneTreeView.h"
#include <QItemSelectionModel>

SceneTreeView::SceneTreeView(QWidget* parent)
    : QTreeView(parent) {

    setHeaderHidden(false);
    setSelectionMode(SingleSelection);
    setSelectionBehavior(SelectRows);

    connect(selectionModel(),
        &QItemSelectionModel::selectionChanged,
        this,
        &SceneTreeView::onSelectionChanged);
}

void SceneTreeView::onSelectionChanged(const QItemSelection& selected,
    const QItemSelection&) {
    if (selected.indexes().isEmpty())
        return;

    auto index = selected.indexes().first();
    uint32_t id = index.data(Qt::UserRole).toUInt();

    emit entitySelected(id);
}