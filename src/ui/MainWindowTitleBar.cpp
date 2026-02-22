#include "MainWindowTitleBar.h"
#include "MainWindow.h"
#include <QApplication>
#include <QStyle>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

TitleBar::TitleBar(MainWindow* mainWindow, QWidget* parent)
    : QWidget(parent)
    , m_mainWindow(mainWindow)
    , m_isDragging(false)
{
    setupUI();
    connectSignals();
}

TitleBar::~TitleBar()
{
}

void TitleBar::setupUI()
{
    // 设置标题栏的大小和背景
    setFixedHeight(32);
    setObjectName("TitleBar");
    
    // 创建水平布局
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(8, 0, 8, 0);
    m_layout->setSpacing(8);
    
    // 创建应用图标
    m_appIcon = new QLabel(this);
    m_appIcon->setObjectName("AppIcon");
    m_appIcon->setFixedSize(16, 16);
    QPixmap icon(QCoreApplication::applicationDirPath() + "/icons/bootstrap/app.svg");
    m_appIcon->setPixmap(icon.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_layout->addWidget(m_appIcon);
    
	// 创建菜单栏
    createMenus();

    // 创建应用标题
    m_appTitle = new QLabel("Kazia", this);
    m_appTitle->setObjectName("AppTitle");
    m_appTitle->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    m_layout->addWidget(m_appTitle);

	// 创建窗口控制按钮
    createButtons();
}

void TitleBar::createMenus()
{
	QMenuBar* menuBar = new QMenuBar(this);
    menuBar->setMinimumHeight(32);
    menuBar->setMaximumHeight(32);
    menuBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    menuBar->setObjectName("MenuBar");
    m_layout->addWidget(menuBar);

    // 文件菜单
    QMenu* fileMenu = menuBar->addMenu("文件");
    fileMenu->setObjectName("FileMenu");
    fileMenu->addAction("新建", m_mainWindow, &MainWindow::onNewScene, QKeySequence::New);
    fileMenu->addAction("打开", m_mainWindow, &MainWindow::onOpenScene, QKeySequence::Open);
    fileMenu->addAction("保存", m_mainWindow, &MainWindow::onSaveScene, QKeySequence::Save);
    fileMenu->addSeparator();
    fileMenu->addAction("导入模型", m_mainWindow, &MainWindow::onImportGltf);
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

void TitleBar::createButtons()
{
    // 创建最小化按钮
    m_minimizeButton = new QPushButton(this);
    m_minimizeButton->setObjectName("MinimizeButton");
    m_minimizeButton->setFixedSize(20, 20);
    m_minimizeButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMinButton));
    m_minimizeButton->setStyleSheet(
        "QPushButton { background-color: transparent; color: #CCCCCC; }" 
        "QPushButton:hover { background-color: #444444; }"
    );
    m_layout->addWidget(m_minimizeButton);
    
    // 创建最大化按钮
    m_maximizeButton = new QPushButton(this);
    m_maximizeButton->setObjectName("MaximizeButton");
    m_maximizeButton->setFixedSize(20, 20);
    updateMaximizeButton();
    m_maximizeButton->setStyleSheet(
        "QPushButton { background-color: transparent; color: #CCCCCC; }" 
        "QPushButton:hover { background-color: #444444; }"
    );
    m_layout->addWidget(m_maximizeButton);
    
    // 创建关闭按钮
    m_closeButton = new QPushButton(this);
    m_closeButton->setObjectName("CloseButton");
    m_closeButton->setFixedSize(20, 20);
    m_closeButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    m_closeButton->setStyleSheet(
        "QPushButton { background-color: transparent; color: #CCCCCC; }" 
        "QPushButton:hover { background-color: #E81123; }"
    );
    m_layout->addWidget(m_closeButton);
}

void TitleBar::connectSignals()
{
    // 连接按钮的信号槽
    connect(m_minimizeButton, &QPushButton::clicked, this, &TitleBar::onMinimizeClicked);
    connect(m_maximizeButton, &QPushButton::clicked, this, &TitleBar::onMaximizeClicked);
    connect(m_closeButton, &QPushButton::clicked, this, &TitleBar::onCloseClicked);
}

void TitleBar::mousePressEvent(QMouseEvent* event)
{
    // 只在标题栏区域（非按钮区域）处理拖拽
    if (event->button() == Qt::LeftButton && 
        !m_minimizeButton->geometry().contains(event->pos()) &&
        !m_maximizeButton->geometry().contains(event->pos()) &&
        !m_closeButton->geometry().contains(event->pos())) {
        
        m_isDragging = true;
        m_dragStartPosition = event->globalPos() - m_mainWindow->frameGeometry().topLeft();
        event->accept();
    }
}

void TitleBar::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDragging && (event->buttons() & Qt::LeftButton)) {
        QPoint newPosition = event->globalPos() - m_dragStartPosition;
        m_mainWindow->move(newPosition);
        event->accept();
    }
}

void TitleBar::mouseReleaseEvent(QMouseEvent* event)
{
    m_isDragging = false;
    event->accept();
}

void TitleBar::updateMaximizeButton()
{
    if (m_mainWindow->isMaximized()) {
        m_maximizeButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarNormalButton));
    } else {
        m_maximizeButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMaxButton));
    }
}

void TitleBar::setTitle(const QString& title)
{
    m_appTitle->setText(title);
}

void TitleBar::onMinimizeClicked()
{
    m_mainWindow->showMinimized();
}

void TitleBar::onMaximizeClicked()
{
    if (m_mainWindow->isMaximized()) {
        m_mainWindow->showNormal();
    } else {
        m_mainWindow->showMaximized();
    }
    updateMaximizeButton();
}

void TitleBar::onCloseClicked()
{
    QApplication::quit();
}
