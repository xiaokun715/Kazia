#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QLabel>
#include <QStatusBar>

class RenderWidget;
class SceneTree;
class PropertiesPanel;
class TitleBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    RenderWidget* m_renderWidget;
    SceneTree* m_sceneTree;
    PropertiesPanel* m_propertiesPanel;
    TitleBar* m_titleBar;

    QDockWidget* m_sceneTreeDock;
    QDockWidget* m_propertiesPanelDock;
    
    // 状态栏组件
    QLabel* m_editModeIndicator;
    QLabel* m_coordinateInfo;
    QLabel* m_fpsIndicator;
    QLabel* m_logInfo;

    void createMenus();
    void createToolbars();
    void createDockWidgets();
    void setupCustomTitleBar();
    void setupStatusBar();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    RenderWidget* getRenderWidget() const { return m_renderWidget; }
    SceneTree* getSceneTree() const { return m_sceneTree; }
    PropertiesPanel* getPropertiesPanel() const { return m_propertiesPanel; }
    TitleBar* getTitleBar() const { return m_titleBar; }

protected:
    void resizeEvent(QResizeEvent* event) override;
    void changeEvent(QEvent* event) override;
};

#endif // MAINWINDOW_H
