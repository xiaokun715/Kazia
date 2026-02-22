#include "ViewportOverlayWidget.h"
#include <QPainter>
#include <QStyle>
#include <QPalette>

RenderOverlayWidget::RenderOverlayWidget(QWidget* parent) : QWidget(parent)
{
    //setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    //setAttribute(Qt::WA_NoSystemBackground);       // 保留
    //setAutoFillBackground(false);                  // 保留

    //// 改成半透明深色底（现代 UI 常用），或纯色测试
    //setStyleSheet(R"(
    //    background-color: rgba(30, 35, 45, 180);   /* 半透深灰 */
    //    border-radius: 6px;                        /* 可选圆角 */
    //)");

    // 关键设置：背景透明 + 不自动填充
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoFillBackground(false);

    // 背景透明（不要用 WA_TranslucentBackground，否则子控件常黑屏）
    setStyleSheet("background-color: transparent;");

    // 如果只想测试是否是背景问题，先用不透明色
    // setStyleSheet("background-color: #2a2a2a;");

    setupUI();
}

void RenderOverlayWidget::setupUI()
{
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(8, 4, 8, 4);
    m_layout->setSpacing(6);

    // ---- Left group ----
    m_viewModeButton = createTextButton("Perspective");
    m_litModeButton = createTextButton("Lit");
    m_showButton = createTextButton("Show");

    m_layout->addWidget(m_viewModeButton);
    m_layout->addWidget(m_litModeButton);
    m_layout->addWidget(m_showButton);

    m_layout->addStretch();

    // ---- Right group ----
    m_translateButton = createIconButton(QCoreApplication::applicationDirPath() + "/icons/bootstrap/translate.svg", "Translate");
    m_rotateButton = createIconButton(QCoreApplication::applicationDirPath() + "/icons/bootstrap/router.svg", "Rotate");
    m_scaleButton = createIconButton(QCoreApplication::applicationDirPath() + "/icons/bootstrap/tools.svg", "Scale");

    m_translateButton->setCheckable(true);
    m_rotateButton->setCheckable(true);
    m_scaleButton->setCheckable(true);

    m_translateButton->setChecked(true);

    m_snapAngleButton = createTextButton("10");
    m_snapScaleButton = createTextButton("0.25");

    m_layout->addWidget(m_translateButton);
    m_layout->addWidget(m_rotateButton);
    m_layout->addWidget(m_scaleButton);
    m_layout->addWidget(m_snapAngleButton);
    m_layout->addWidget(m_snapScaleButton);
}

QPushButton* RenderOverlayWidget::createTextButton(const QString& text)
{
    auto* btn = new QPushButton(text, this);
    btn->setFlat(true);
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setFixedHeight(24);
    return btn;
}

QToolButton* RenderOverlayWidget::createIconButton(const QString& iconPath, const QString& tooltip)
{
    auto* btn = new QToolButton(this);
    btn->setIcon(QIcon(iconPath));
    btn->setIconSize(QSize(16, 16));
    btn->setAutoRaise(true);
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setToolTip(tooltip);
    btn->setFixedSize(24, 24);
    return btn;
}