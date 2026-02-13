#include "DockTitleBar.h"
#include <QDockWidget>
#include <QApplication>
#include <QStyle>

DockTitleBar::DockTitleBar(QDockWidget* dockWidget, QWidget* parent)
    : QWidget(parent)
    , m_dockWidget(dockWidget)
{
    setupUI();
    createButtons();
    connectSignals();
}

DockTitleBar::~DockTitleBar()
{
}

void DockTitleBar::setupUI()
{
    // 设置标题栏的大小和背景
    setFixedHeight(24);
    setObjectName("DockTitleBar");
    
    // 创建水平布局
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(8, 0, 4, 0);
    m_layout->setSpacing(4);
    
    // 创建标题标签
    m_titleLabel = new QLabel(m_dockWidget->windowTitle(), this);
    m_titleLabel->setObjectName("DockTitleLabel");
    m_titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_layout->addWidget(m_titleLabel);
}

void DockTitleBar::createButtons()
{
    // 创建浮动按钮
    m_floatButton = new QPushButton(this);
    m_floatButton->setObjectName("DockFloatButton");
    m_floatButton->setFixedSize(20, 20);
    m_floatButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarNormalButton));
    m_floatButton->setToolTip("浮动");
    m_layout->addWidget(m_floatButton);
    
    // 创建关闭按钮
    m_closeButton = new QPushButton(this);
    m_closeButton->setObjectName("DockCloseButton");
    m_closeButton->setFixedSize(20, 20);
    m_closeButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    m_closeButton->setToolTip("关闭");
    m_layout->addWidget(m_closeButton);
}

void DockTitleBar::connectSignals()
{
    // 连接按钮的信号槽
    connect(m_floatButton, &QPushButton::clicked, this, &DockTitleBar::onFloatClicked);
    connect(m_closeButton, &QPushButton::clicked, this, &DockTitleBar::onCloseClicked);
}

void DockTitleBar::setTitle(const QString& title)
{
    m_titleLabel->setText(title);
    m_dockWidget->setWindowTitle(title);
}

void DockTitleBar::onFloatClicked()
{
    if (m_dockWidget) {
        m_dockWidget->setFloating(!m_dockWidget->isFloating());
    }
}

void DockTitleBar::onCloseClicked()
{
    if (m_dockWidget) {
        m_dockWidget->hide();
    }
}
