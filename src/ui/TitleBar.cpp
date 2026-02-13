#include "TitleBar.h"
#include "MainWindow.h"
#include <QApplication>
#include <QStyle>

TitleBar::TitleBar(MainWindow* mainWindow, QWidget* parent)
    : QWidget(parent)
    , m_mainWindow(mainWindow)
    , m_isDragging(false)
{
    setupUI();
    createButtons();
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
    m_appIcon->setFixedSize(20, 20);
    m_layout->addWidget(m_appIcon);
    
    // 创建应用标题
    m_appTitle = new QLabel("Kazia - 3D Renderer", this);
    m_appTitle->setObjectName("AppTitle");
    m_appTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_layout->addWidget(m_appTitle);
    
    // 添加按钮的占位符，后续在 createButtons 中添加
}

void TitleBar::createButtons()
{
    // 创建最小化按钮
    m_minimizeButton = new QPushButton(this);
    m_minimizeButton->setObjectName("MinimizeButton");
    m_minimizeButton->setFixedSize(24, 24);
    m_minimizeButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMinButton));
    m_minimizeButton->setStyleSheet(
        "QPushButton { background-color: transparent; color: #CCCCCC; }" 
        "QPushButton:hover { background-color: #444444; }"
    );
    m_layout->addWidget(m_minimizeButton);
    
    // 创建最大化按钮
    m_maximizeButton = new QPushButton(this);
    m_maximizeButton->setObjectName("MaximizeButton");
    m_maximizeButton->setFixedSize(24, 24);
    updateMaximizeButton();
    m_maximizeButton->setStyleSheet(
        "QPushButton { background-color: transparent; color: #CCCCCC; }" 
        "QPushButton:hover { background-color: #444444; }"
    );
    m_layout->addWidget(m_maximizeButton);
    
    // 创建关闭按钮
    m_closeButton = new QPushButton(this);
    m_closeButton->setObjectName("CloseButton");
    m_closeButton->setFixedSize(24, 24);
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
