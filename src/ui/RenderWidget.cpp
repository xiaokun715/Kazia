#include "RenderWidget.h"
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>

RenderWidget::RenderWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_renderer(nullptr)
    , m_renderTimer(nullptr)
    , m_isDragging(false)
    , m_cameraDistance(5.0f)
    , m_cameraYaw(0.0f)
    , m_cameraPitch(0.0f)
    , m_initialWidth(0)
    , m_initialHeight(0)
{
    // 设置 OpenGL 格式
    QSurfaceFormat format;
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    setFormat(format);

    // 创建渲染定时器
    m_renderTimer = new QTimer(this);
    connect(m_renderTimer, &QTimer::timeout, this, &RenderWidget::renderFrame);
}

RenderWidget::~RenderWidget()
{
    // 先停止渲染定时器，避免在销毁渲染器后仍然触发渲染事件
    if (m_renderTimer) {
        m_renderTimer->stop();
    }
    
    // 延迟销毁渲染器，确保所有事件处理完成
    QCoreApplication::processEvents();
    
    shutdownRenderer();
    
    // 再次处理事件，确保渲染器销毁完成
    QCoreApplication::processEvents();
    
    delete m_renderTimer;
}

void RenderWidget::initializeGL()
{
    initializeRenderer();

    // 启动渲染定时器
    m_renderTimer->start(16); // 约 60 FPS
}

void RenderWidget::paintGL()
{
    renderFrame();
}

void RenderWidget::resizeGL(int w, int h)
{
    if (m_renderer) {
        m_renderer->resize(w, h);
    } else {
        // 存储初始大小，以便在渲染器初始化时使用
        m_initialWidth = w;
        m_initialHeight = h;
    }
}

void RenderWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
    }
}

void RenderWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDragging) {
        QPoint delta = event->pos() - m_lastMousePos;
        
        // 更新相机角度
        m_cameraYaw += delta.x() * 0.01f;
        m_cameraPitch += delta.y() * 0.01f;
        
        // 限制俯仰角范围
        m_cameraPitch = qMax(-1.5f, qMin(1.5f, m_cameraPitch));
        
        // 计算相机位置
        float x = m_cameraDistance * cos(m_cameraYaw) * cos(m_cameraPitch);
        float y = m_cameraDistance * sin(m_cameraPitch);
        float z = m_cameraDistance * sin(m_cameraYaw) * cos(m_cameraPitch);
        
        if (m_renderer) {
            m_renderer->setCameraPosition(filament::math::float3{x, y, z});
        }
        
        m_lastMousePos = event->pos();
    }
}

void RenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
    }
}

void RenderWidget::wheelEvent(QWheelEvent* event)
{
    // 调整相机距离
    m_cameraDistance -= event->angleDelta().y() * 0.001f;
    m_cameraDistance = qMax(1.0f, qMin(20.0f, m_cameraDistance));
    
    // 计算相机位置
    float x = m_cameraDistance * cos(m_cameraYaw) * cos(m_cameraPitch);
    float y = m_cameraDistance * sin(m_cameraPitch);
    float z = m_cameraDistance * sin(m_cameraYaw) * cos(m_cameraPitch);
    
    if (m_renderer) {
        m_renderer->setCameraPosition(filament::math::float3{x, y, z});
    }
}

void RenderWidget::initializeRenderer()
{
    // 创建渲染器
    m_renderer = new Renderer();

    // 获取本地窗口句柄
    void* nativeWindow = reinterpret_cast<void*>(winId());

    // 使用存储的初始大小或当前窗口大小
    int initWidth = (m_initialWidth > 0) ? m_initialWidth : width();
    int initHeight = (m_initialHeight > 0) ? m_initialHeight : height();

    // 初始化渲染器
    m_renderer->initialize(nativeWindow, initWidth, initHeight);
}

void RenderWidget::shutdownRenderer()
{
    if (m_renderer) {
        try {
            m_renderer->shutdown();
            delete m_renderer;
            m_renderer = nullptr;
        } catch (...) {
            // 忽略销毁渲染器时的错误
            m_renderer = nullptr;
        }
    }
}

void RenderWidget::renderFrame()
{
    if (m_renderer) {
        m_renderer->renderFrame();
    }
}
