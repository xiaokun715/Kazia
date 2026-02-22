#include "ViewportWidget.h"
#include "render/Renderer.h"
#include "OverlayUIWidget.h"
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <iostream>
#include <imgui.h>

ViewportWidget::ViewportWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_renderer(nullptr)
    , m_renderTimer(nullptr)
    , m_isDragging(false)
    , m_cameraDistance(5.0f)
    , m_cameraYaw(0.0f)
    , m_cameraPitch(0.0f)
    , m_initialWidth(1)
    , m_initialHeight(1)
{
    // 创建渲染定时器
    m_renderTimer = new QTimer(this);
    connect(m_renderTimer, &QTimer::timeout, this, &ViewportWidget::renderFrame);
}

ViewportWidget::~ViewportWidget()
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

void ViewportWidget::setupOverlayUI()
{
    auto engine = m_renderer->engine();
    auto uiView = m_renderer->uiView();
    
    m_overlayUI = std::make_unique<OverlayUIWidget>();
    m_overlayUI->initialize(engine, uiView, m_initialWidth, m_initialHeight);

    m_overlayUI->setToolbarCallback(
        [this](ToolbarAction action) {
            // TODO: 派发到 Editor / Gizmo / Camera
        }
    );
}

void ViewportWidget::initializeGL()
{
    initializeRenderer();

    // 启动渲染定时器
    m_renderTimer->start(16); // 约 60 FPS
}

void ViewportWidget::paintGL()
{
    renderFrame();
}

void ViewportWidget::resizeGL(int w, int h)
{
    if (m_renderer) {
        m_renderer->resize(w * devicePixelRatio(), h * devicePixelRatio());
    } else {
        // 存储初始大小，以便在渲染器初始化时使用
        m_initialWidth = w;
        m_initialHeight = h;
    }

    // 更新 ImGui DisplaySize
    m_overlayUI->resize(w, h, 1.0f);
}

bool ViewportWidget::event(QEvent* e)
{
    if (ImGui::GetCurrentContext()) {
        ImGuiIO& io = ImGui::GetIO();
        float dpr = devicePixelRatioF();

        switch (e->type()) {

        case QEvent::MouseMove: {
            auto* ev = static_cast<QMouseEvent*>(e);
            QPointF p = ev->position();
            io.AddMousePosEvent(p.x() * dpr, p.y() * dpr);
        }

        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease: {
            auto* ev = static_cast<QMouseEvent*>(e);
            bool down = (e->type() == QEvent::MouseButtonPress);

            QPointF p = ev->position();
            io.AddMousePosEvent(p.x() * dpr, p.y() * dpr);

            if (ev->button() == Qt::LeftButton)
                io.AddMouseButtonEvent(0, down);
            if (ev->button() == Qt::RightButton)
                io.AddMouseButtonEvent(1, down);
            if (ev->button() == Qt::MiddleButton)
                io.AddMouseButtonEvent(2, down);
            break;
        }

        case QEvent::Wheel: {
            auto* ev = static_cast<QWheelEvent*>(e);
            io.AddMouseWheelEvent(0.0f, ev->angleDelta().y() / 120.0f);
            break;
        }

        case QEvent::KeyPress:
        case QEvent::KeyRelease: {
            auto* ev = static_cast<QKeyEvent*>(e);
            bool down = (e->type() == QEvent::KeyPress);
            io.AddKeyEvent(ImGuiKey(ev->key()), down);
            break;
        }

        default:
            break;
        }
    }

    return QOpenGLWidget::event(e);
}

void ViewportWidget::mousePressEvent(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    if (event->button() != Qt::LeftButton) return;
    if (!m_renderer) return;

    m_isDragging = true;
    m_lastMousePos = event->pos();

    auto view = m_renderer->view();
    if (!view) return;

    float dpr = devicePixelRatio();

    int px = int(m_lastMousePos.x() * dpr);
    int py = int(m_lastMousePos.y() * dpr);

    view->pick(px, py,
        [](filament::View::PickingQueryResult const& result)
        {
            std::cout << "Picked depth: "
                << result.depth
                << "\n";
        });
}

void ViewportWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

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

void ViewportWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
    }
}

void ViewportWidget::wheelEvent(QWheelEvent* event)
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

void ViewportWidget::initializeRenderer()
{
    // 创建渲染器
    m_renderer = new Renderer();

    // 获取本地窗口句柄
    void* nativeWindow = reinterpret_cast<void*>(winId());

    // 使用存储的初始大小或当前窗口大小
    int initWidth = (m_initialWidth > 0) ? m_initialWidth : width() * devicePixelRatio();
    int initHeight = (m_initialHeight > 0) ? m_initialHeight : height() * devicePixelRatio();

    // 初始化渲染器
    m_renderer->initialize(nativeWindow, initWidth, initHeight);

    // 设置Overlay UI界面
    setupOverlayUI();
}

void ViewportWidget::shutdownRenderer()
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

void ViewportWidget::renderFrame()
{
    //if (m_renderer) {
    //    m_overlayUI->render(0.016f);
    //    m_renderer->renderFrame();
    //}

    auto renderer = m_renderer->renderer();
    auto swapChain = m_renderer->swapChain();
    auto view = m_renderer->view();
    auto uiView = m_renderer->uiView();

    if (m_overlayUI && width() > 0 && height() > 0) {
        m_overlayUI->resize(width(), height(), 1.0f);
    }

    if (renderer->beginFrame(swapChain)) {

        m_overlayUI->render(0.016f);

        auto* drawData = ImGui::GetDrawData();

        // 渲染 Filament 视图
        renderer->render(view);
        renderer->render(uiView);

        renderer->endFrame();
    }
}
