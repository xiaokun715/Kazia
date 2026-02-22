#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QLabel>
#include <QStatusBar>
#include "DockManager.h"

class RenderWidget;
class SceneTreeWidget;
class PropertiesWidget;
class TitleBar;
class ViewportContainer;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

public:
    explicit MainWindow(QWidget * parent = nullptr);
    ~MainWindow() override;

    RenderWidget* getRenderWidget() const { return m_viewportWidget; }
    QWidget* getSceneTree() const { return m_sceneTreeWidget; }
    PropertiesWidget* getPropertiesPanel() const { return m_propertiesWidget; }
    TitleBar* getTitleBar() const { return m_titleBar; }

private:
    TitleBar* m_titleBar;

    ads::CDockManager* m_dockManager;
	ads::CDockWidget* m_viewportDock;
	ads::CDockWidget* m_sceneTreeDock;
	ads::CDockWidget* m_propertiesDock;
    QWidget* m_sceneTreeWidget;
    PropertiesWidget* m_propertiesWidget;
    RenderWidget* m_viewportWidget;
    ViewportContainer* m_viewportContainer;

    // 状态栏组件
    QLabel* m_editModeIndicator;
    QLabel* m_coordinateInfo;
    QLabel* m_fpsIndicator;
    QLabel* m_logInfo;

    void createToolbars();
    void createDockWidgets();
    void setupCustomTitleBar();
    void setupStatusBar();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void changeEvent(QEvent* event) override;

public slots:
    // 文件菜单槽函数
    void onNewScene();
    void onOpenScene();
    void onSaveScene();

	// 导入gltf模型
	void onImportGltf();
};

#endif // MAINWINDOW_H
