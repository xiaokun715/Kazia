#ifndef OPENGL_VIEWPORT_WIDGET_H
#define OPENGL_VIEWPORT_WIDGET_H

#include "IViewPort.h"
#include "IViewPortSurface.h"
#include "render/IRenderBackend.h"
#include <QOpenGLWidget>

class OpenGLViewport final
    : public QOpenGLWidget
    , public IViewportSurface
    , public IViewport
{
public:
    OpenGLViewport(IRenderBackend* backend);

    // IViewport
    void initialize() override;
    void resize(uint32_t w, uint32_t h) override;
    void render() override;

    // IViewportSurface
    void* nativeHandle() const override;
    uint32_t width() const override;
    uint32_t height() const override;
    bool isValid() const override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    IRenderBackend* backend_;
};

#endif // !OPENGL_VIEWPORT_WIDGET_H
