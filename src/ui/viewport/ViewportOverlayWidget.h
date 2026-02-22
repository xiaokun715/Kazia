#ifndef VIEWPORT_OVERLAY_WIDGET_H
#define VIEWPORT_OVERLAY_WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>

class MainWindow;

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QMenu>

class RenderOverlayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RenderOverlayWidget(QWidget* parent = nullptr);

private:
    void setupUI();
    QPushButton* createTextButton(const QString& text);
    QToolButton* createIconButton(const QString& iconPath, const QString& tooltip = QString());

private:
    QHBoxLayout* m_layout = nullptr;

    // Left
    QPushButton* m_viewModeButton = nullptr;
    QPushButton* m_litModeButton = nullptr;
    QPushButton* m_showButton = nullptr;

    // Right
    QToolButton* m_translateButton = nullptr;
    QToolButton* m_rotateButton = nullptr;
    QToolButton* m_scaleButton = nullptr;

    QPushButton* m_snapAngleButton = nullptr;
    QPushButton* m_snapScaleButton = nullptr;
};

#endif // VIEWPORT_OVERLAY_WIDGET_H
