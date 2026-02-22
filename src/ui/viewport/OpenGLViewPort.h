#ifndef OPENGL_VIEWPORT_WIDGET_H
#define OPENGL_VIEWPORT_WIDGET_H

#include "IViewPort.h"
#include "IViewPortSurface.h"
#include "render/IRenderBackend.h"
#include <QOpenGLWidget>

class Renderer;

class OpenGLViewport final
    : public QOpenGLWidget
    , public IViewportSurface
    , public IViewport
{
public:
    OpenGLViewport(QWidget* parent);
    ~OpenGLViewport() override;

    // IViewport
    void initialize() override;
    void resize(uint32_t w, uint32_t h) override;
    void render() override;

    // IViewportSurface
    void* nativeHandle() const override;
    uint32_t width() const override;
    uint32_t height() const override;
    bool isValid() const override;
    Renderer* getRenderer() const { return m_renderer; }

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    bool event(QEvent* e) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

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

private slots:
    void renderFrame();
};

#endif // !OPENGL_VIEWPORT_WIDGET_H
