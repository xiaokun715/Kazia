#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H

#include <QWidget>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLContext>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>

#include "../core/Renderer.h"

class RenderWidget : public QOpenGLWidget
{
    Q_OBJECT

private:
    Renderer* m_renderer;
    QTimer* m_renderTimer;

    // 相机控制变量
    bool m_isDragging;
    QPoint m_lastMousePos;
    float m_cameraDistance;
    float m_cameraYaw;
    float m_cameraPitch;
    
    // 初始窗口大小
    int m_initialWidth;
    int m_initialHeight;

    void initializeRenderer();
    void shutdownRenderer();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

public:
    RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget();

    Renderer* getRenderer() const { return m_renderer; }

private slots:
    void renderFrame();
};

#endif // RENDERINGWIDGET_H
