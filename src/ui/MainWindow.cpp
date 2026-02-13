#include "MainWindow.h"
#include "RenderWidget.h"
#include "SceneTree.h"
#include "PropertiesPanel.h"
#include "TitleBar.h"
#include "DockTitleBar.h"
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_renderWidget(nullptr)
    , m_sceneTree(nullptr)
    , m_propertiesPanel(nullptr)
    , m_titleBar(nullptr)
    , m_sceneTreeDock(nullptr)
    , m_propertiesPanelDock(nullptr)
    , m_editModeIndicator(nullptr)
    , m_coordinateInfo(nullptr)
    , m_fpsIndicator(nullptr)
    , m_logInfo(nullptr)
{
    // 设置无边框窗口
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, false);
    setObjectName("MainWindow");
    
    // 设置最小大小
    setMinimumSize(800, 600);

    // 创建自定义标题栏
    setupCustomTitleBar();

    // 创建渲染视口
    m_renderWidget = new RenderWidget(this);
    setCentralWidget(m_renderWidget);

    // 创建菜单
    createMenus();

    // 创建工具栏
    createToolbars();

    // 创建停靠窗口
    createDockWidgets();
    
    // 设置状态栏
    setupStatusBar();

    // 设置布局
    resize(1200, 800);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupCustomTitleBar()
{
    // 创建自定义标题栏
    m_titleBar = new TitleBar(this, this);
    
    // 将标题栏添加到窗口顶部
    // 注意：使用 setMenuWidget 将标题栏设置为菜单小部件
    // 这样标题栏会显示在菜单栏的上方
    setMenuWidget(m_titleBar);
}

void MainWindow::createMenus()
{
    QMenuBar* menuBar = this->menuBar();
    menuBar->setObjectName("MenuBar");

    // 文件菜单
    QMenu* fileMenu = menuBar->addMenu("文件");
    fileMenu->setObjectName("FileMenu");
    fileMenu->addAction("新建", this, nullptr, QKeySequence::New);
    fileMenu->addAction("打开", this, nullptr, QKeySequence::Open);
    fileMenu->addAction("保存", this, nullptr, QKeySequence::Save);
    fileMenu->addSeparator();
    fileMenu->addAction("退出", this, &QApplication::quit);

    // 编辑菜单
    QMenu* editMenu = menuBar->addMenu("编辑");
    editMenu->setObjectName("EditMenu");
    editMenu->addAction("撤销", this, nullptr, QKeySequence::Undo);
    editMenu->addAction("重做", this, nullptr, QKeySequence::Redo);
    editMenu->addSeparator();
    editMenu->addAction("复制", this, nullptr, QKeySequence::Copy);
    editMenu->addAction("粘贴", this, nullptr, QKeySequence::Paste);

    // 视图菜单
    QMenu* viewMenu = menuBar->addMenu("视图");
    viewMenu->setObjectName("ViewMenu");
    viewMenu->addAction("场景树", this, nullptr);
    viewMenu->addAction("属性面板", this, nullptr);

    // 帮助菜单
    QMenu* helpMenu = menuBar->addMenu("帮助");
    helpMenu->setObjectName("HelpMenu");
    helpMenu->addAction("关于", this, nullptr);
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
    // 场景树停靠窗口
    m_sceneTreeDock = new QDockWidget("场景树", this);
    m_sceneTreeDock->setObjectName("SceneTreeDock");
    
    // 创建自定义标题栏
    DockTitleBar* sceneTreeTitleBar = new DockTitleBar(m_sceneTreeDock, m_sceneTreeDock);
    m_sceneTreeDock->setTitleBarWidget(sceneTreeTitleBar);
    
    m_sceneTree = new SceneTree(m_sceneTreeDock);
    m_sceneTreeDock->setWidget(m_sceneTree);
    addDockWidget(Qt::LeftDockWidgetArea, m_sceneTreeDock);

    // 属性面板停靠窗口
    m_propertiesPanelDock = new QDockWidget("属性", this);
    m_propertiesPanelDock->setObjectName("PropertiesPanelDock");
    
    // 创建自定义标题栏
    DockTitleBar* propertiesTitleBar = new DockTitleBar(m_propertiesPanelDock, m_propertiesPanelDock);
    m_propertiesPanelDock->setTitleBarWidget(propertiesTitleBar);
    
    m_propertiesPanel = new PropertiesPanel(m_propertiesPanelDock);
    m_propertiesPanelDock->setWidget(m_propertiesPanel);
    addDockWidget(Qt::RightDockWidgetArea, m_propertiesPanelDock);

    // 连接场景树和属性面板的信号槽
    connect(m_sceneTree, &SceneTree::gameObjectSelected, this, [=](const QString& name) {
        // 这里可以从场景中获取游戏对象，并设置属性面板的属性
        m_propertiesPanel->setGameObjectProperties(
            name,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f
        );
    });

    // 连接属性面板的信号
    connect(m_propertiesPanel, &PropertiesPanel::propertyChanged, this, [=](const QString& property, float value) {
        // 这里可以更新游戏对象的属性
    });

    connect(m_propertiesPanel, &PropertiesPanel::nameChanged, this, [=](const QString& name) {
        // 这里可以更新游戏对象的名称
    });

    // 添加一些测试对象到场景树
    m_sceneTree->addGameObject("立方体1");
    m_sceneTree->addGameObject("球体1");
    m_sceneTree->addGameObject("圆柱体1");
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    // 当窗口大小改变时，渲染视口也需要调整大小
    if (m_renderWidget) {
        m_renderWidget->resize(m_renderWidget->size());
    }
}

void MainWindow::changeEvent(QEvent* event)
{
    QMainWindow::changeEvent(event);
    
    // 当窗口状态改变时（如最大化/还原），更新最大化按钮
    if (event->type() == QEvent::WindowStateChange) {
        if (m_titleBar) {
            m_titleBar->updateMaximizeButton();
        }
    }
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
