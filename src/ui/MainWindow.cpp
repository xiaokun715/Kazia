#include "MainWindow.h"
#include "MainWindowTitleBar.h"
#include "render/renderer.h"
#include "inspector/PropertiesWidget.h"
#include "viewport/OpenGLViewPort.h"
#include "viewport/VulkanViewPort.h"
#include "viewport/ViewportWidget.h"
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QApplication>
#include <QVBoxLayout>
#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_viewportWidget(nullptr)
    , m_viewportContainer(nullptr)
    , m_titleBar(nullptr)
    , m_sceneTreeWidget(nullptr)
    , m_propertiesWidget(nullptr)
    , m_editModeIndicator(nullptr)
    , m_coordinateInfo(nullptr)
    , m_fpsIndicator(nullptr)
    , m_logInfo(nullptr)
{
    // 设置无边框窗口
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, false);
    setObjectName("MainWindow");
    setMinimumSize(800, 600);

    // Create the dock manager
    ads::CDockManager::setConfigFlags(ads::CDockManager::DefaultOpaqueConfig);
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHasCloseButton, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHasUndockButton, false);
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHasTabsMenuButton, false);
    m_dockManager = new ads::CDockManager(this);
    setCentralWidget(m_dockManager);

    // 创建自定义标题栏
    setupCustomTitleBar();

    // 创建工具栏
    createToolbars();

    // 创建停靠窗口
    createDockWidgets();
    
    // 设置状态栏
    setupStatusBar();

    // 设置布局
    showMaximized();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupCustomTitleBar()
{
    // 创建自定义标题栏
    m_titleBar = new TitleBar(this, this);
    setMenuWidget(m_titleBar);
}

void MainWindow::createToolbars()
{
    // 主工具栏
    QToolBar* mainToolbar = addToolBar("主工具栏");
    mainToolbar->setObjectName("MainToolbar");
    mainToolbar->addAction("新建");
    mainToolbar->addAction("打开");
    mainToolbar->addAction("保存");
    mainToolbar->addSeparator();
    mainToolbar->addAction("撤销");
    mainToolbar->addAction("重做");

    // 编辑工具栏
    QToolBar* editToolbar = addToolBar("编辑工具栏");
    editToolbar->setObjectName("EditToolbar");
    editToolbar->addAction("选择");
    editToolbar->addAction("移动");
    editToolbar->addAction("旋转");
    editToolbar->addAction("缩放");
    editToolbar->addSeparator();
    editToolbar->addAction("创建立方体");
    editToolbar->addAction("创建球体");
    editToolbar->addAction("创建圆柱体");
}

void MainWindow::createDockWidgets()
{
    // 创建渲染视口
    m_viewportDock = new ads::CDockWidget("Viewport");
    //m_viewportContainer = new ViewportContainer(this);
    //m_viewportDock->setWidget(m_viewportContainer);
    m_viewportWidget = new ViewportWidget(this);
    m_viewportDock->setWidget(m_viewportWidget);
    QString iconPath = QCoreApplication::applicationDirPath() + "/icons/bootstrap/app.svg";
    m_viewportDock->setIcon(QIcon(iconPath));
    m_dockManager->addDockWidget(ads::LeftDockWidgetArea, m_viewportDock);

    // 场景树停靠窗口
    m_sceneTreeDock = new ads::CDockWidget("Scene");
    m_sceneTreeWidget = new QWidget(this);
    m_sceneTreeDock->setWidget(m_sceneTreeWidget);
    m_dockManager->addDockWidget(ads::RightDockWidgetArea, m_sceneTreeDock);
    m_dockManager->setSplitterSizes(m_sceneTreeDock->dockAreaWidget(), { 700, 300 });

    // 属性面板停靠窗口
    m_propertiesDock = new ads::CDockWidget("Property");
    m_propertiesWidget = new PropertiesWidget(this);
    m_propertiesDock->setWidget(m_propertiesWidget);
    m_dockManager->addDockWidget(ads::BottomDockWidgetArea, m_propertiesDock, m_sceneTreeDock->dockAreaWidget());
    m_dockManager->setSplitterSizes(m_propertiesDock->dockAreaWidget(), { 1, 1 });

    // 连接场景树和属性面板的信号槽
    //connect(m_sceneTree, &SceneTree::gameObjectSelected, this, [=](const QString& name) {
    //    // 这里可以从场景中获取游戏对象，并设置属性面板的属性
    //    m_propertiesPanel->setGameObjectProperties(
    //        name,
    //        0.0f, 0.0f, 0.0f,
    //        0.0f, 0.0f, 0.0f,
    //        1.0f, 1.0f, 1.0f
    //    );
    //});

    // 连接属性面板的信号
    connect(m_propertiesWidget, &PropertiesWidget::propertyChanged, this, [=](const QString& property, float value) {
        // 这里可以更新游戏对象的属性
    });

    connect(m_propertiesWidget, &PropertiesWidget::nameChanged, this, [=](const QString& name) {
        // 这里可以更新游戏对象的名称
    });
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    // 当窗口大小改变时，渲染视口也需要调整大小
    //if (m_viewportContainer) {
		//m_viewportContainer->getRenderWidget()->resize(m_viewportContainer->size());
    //}
    if (m_viewportWidget) {
        m_viewportWidget->resize(m_viewportWidget->size());
    }
}

void MainWindow::changeEvent(QEvent* event)
{
    QMainWindow::changeEvent(event);
}

void MainWindow::setupStatusBar()
{
    // 获取主窗口的状态栏
    QStatusBar* statusBar = this->statusBar();
    statusBar->setObjectName("StatusBar");
    
    // 创建编辑模式指示器
    m_editModeIndicator = new QLabel("编辑模式: 选择", this);
    m_editModeIndicator->setObjectName("EditModeIndicator");
    m_editModeIndicator->setMinimumWidth(120);
    statusBar->addWidget(m_editModeIndicator);
    
    // 添加分隔线
    QFrame* separator1 = new QFrame(this);
    separator1->setObjectName("StatusBarSeparator");
    separator1->setFrameShape(QFrame::VLine);
    separator1->setFrameShadow(QFrame::Sunken);
    separator1->setFixedHeight(16);
    statusBar->addWidget(separator1);
    
    // 创建坐标信息显示
    m_coordinateInfo = new QLabel("坐标: (0.0, 0.0, 0.0)", this);
    m_coordinateInfo->setObjectName("CoordinateInfo");
    m_coordinateInfo->setMinimumWidth(150);
    statusBar->addWidget(m_coordinateInfo);
    
    // 添加分隔线
    QFrame* separator2 = new QFrame(this);
    separator2->setObjectName("StatusBarSeparator");
    separator2->setFrameShape(QFrame::VLine);
    separator2->setFrameShadow(QFrame::Sunken);
    separator2->setFixedHeight(16);
    statusBar->addWidget(separator2);
    
    // 创建FPS性能指标显示
    m_fpsIndicator = new QLabel("FPS: 60", this);
    m_fpsIndicator->setObjectName("FpsIndicator");
    m_fpsIndicator->setMinimumWidth(80);
    statusBar->addWidget(m_fpsIndicator);
    
    // 添加分隔线
    QFrame* separator3 = new QFrame(this);
    separator3->setObjectName("StatusBarSeparator");
    separator3->setFrameShape(QFrame::VLine);
    separator3->setFrameShadow(QFrame::Sunken);
    separator3->setFixedHeight(16);
    statusBar->addWidget(separator3);
    
    // 创建日志信息显示
    m_logInfo = new QLabel("就绪", this);
    m_logInfo->setObjectName("LogInfo");
    m_logInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_logInfo->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    statusBar->addWidget(m_logInfo);
}

// 文件菜单槽函数
void MainWindow::onNewScene() {

}

void MainWindow::onOpenScene() {
    QString filePath = QFileDialog::getOpenFileName(
        nullptr,
        "选择文件",
        ".",
        "gltf文件 (*.glb *gltf);;所有文件 (*.*)"
    );
    //m_viewportContainer->getRenderWidget()->getRenderer()->addGltfModel(filePath);
    m_viewportWidget->getRenderer()->addGltfModel(filePath);
	//m_sceneTree->addGltfModel(filePath);
}

void MainWindow::onSaveScene() {

}

// 导入gltf模型
void MainWindow::onImportGltf() {

}