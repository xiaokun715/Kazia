#include "FilamentEngine.h"
#include <utils/EntityManager.h>
#include <utils/Entity.h>

FilamentEngine::FilamentEngine()
    : m_engine(nullptr)
    , m_renderer(nullptr)
    , m_scene(nullptr)
    , m_view(nullptr)
    , m_camera(nullptr)
    , m_swapChain(nullptr)
    , m_nativeWindow(nullptr)
{}

FilamentEngine::~FilamentEngine()
{
    shutdown();
}

void FilamentEngine::initialize(void* nativeWindow, int width, int height)
{
    // 存储原生窗口句柄
    m_nativeWindow = nativeWindow;
    
    // 创建 Filament 引擎
    m_engine = filament::Engine::create();
    if (!m_engine) {
        return;
    }

    // 创建渲染器
    m_renderer = m_engine->createRenderer();

    // 创建场景
    m_scene = m_engine->createScene();

    // 创建相机
    utils::Entity cameraEntity = utils::EntityManager::get().create();
    m_camera = m_engine->createCamera(cameraEntity);

    // 创建视图
    m_view = m_engine->createView();
    m_view->setScene(m_scene);
    m_view->setCamera(m_camera);

    // 创建交换链
    m_swapChain = m_engine->createSwapChain(m_nativeWindow);

    // 设置初始相机参数
    setCameraProjection(45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
    setCameraPosition({0.0f, 0.0f, 5.0f});
    setCameraTarget({0.0f, 0.0f, 0.0f});

    // 调整视图大小
    filament::Viewport viewport(0, 0, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    m_view->setViewport(viewport);
    
    // 设置清除选项
    filament::Renderer::ClearOptions clearOptions;
    clearOptions.clear = true;
    clearOptions.clearColor = {0.1f, 0.1f, 0.1f, 1.0f}; // 深灰色背景
    clearOptions.discard = true;
    m_renderer->setClearOptions(clearOptions);
}

void FilamentEngine::shutdown()
{
    if (m_engine) {
        // 按照正确的顺序销毁资源
        // 注意：必须先销毁依赖于引擎的资源，然后再销毁引擎本身
        
        // 1. 停止任何可能正在进行的渲染操作
        // 2. 销毁交换链
        if (m_swapChain) {
            try {
                m_engine->destroy(m_swapChain);
                m_swapChain = nullptr;
            } catch (...) {
                // 忽略销毁交换链时的错误
                m_swapChain = nullptr;
            }
        }
        
        // 3. 销毁视图
        if (m_view) {
            try {
                m_engine->destroy(m_view);
                m_view = nullptr;
            } catch (...) {
                // 忽略销毁视图时的错误
                m_view = nullptr;
            }
        }
        
        // 4. 销毁场景
        if (m_scene) {
            try {
                m_engine->destroy(m_scene);
                m_scene = nullptr;
            } catch (...) {
                // 忽略销毁场景时的错误
                m_scene = nullptr;
            }
        }
        
        // 5. 销毁渲染器
        if (m_renderer) {
            try {
                m_engine->destroy(m_renderer);
                m_renderer = nullptr;
            } catch (...) {
                // 忽略销毁渲染器时的错误
                m_renderer = nullptr;
            }
        }
        
        // 6. 销毁引擎
        try {
            // 注意：Engine::destroy 的正确调用方式是传递引擎指针，而不是指向引擎指针的指针
            // 根据 Filament 引擎的 API 文档，正确的调用方式是：
            // filament::Engine::destroy(m_engine);
            // 但是，为了避免可能的内存访问错误，我们先检查引擎指针是否有效
            if (m_engine) {
                filament::Engine::destroy(m_engine);
                m_engine = nullptr; // 确保将引擎指针设置为 nullptr
            }
        } catch (...) {
            // 忽略销毁引擎时的错误
            m_engine = nullptr; // 即使发生错误，也将引擎指针设置为 nullptr
        }
        
        // 确保所有指针都被设置为 nullptr
        m_camera = nullptr;
        m_nativeWindow = nullptr;
    }
}

void FilamentEngine::render()
{
    if (m_renderer && m_swapChain) {
        if (m_renderer->beginFrame(m_swapChain)) {
            m_renderer->render(m_view);
            m_renderer->endFrame();
        }
    }
}

void FilamentEngine::resize(int width, int height)
{
    if (m_view) {
        filament::Viewport viewport(0, 0, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
        m_view->setViewport(viewport);
    }

    if (m_camera) {
        setCameraProjection(45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
    }
    
    // 在窗口大小变化较大时（如最大化），重新创建交换链
    if (m_engine && m_nativeWindow && m_swapChain) {
        // 销毁旧的交换链
        m_engine->destroy(m_swapChain);
        m_swapChain = nullptr;
        
        // 创建新的交换链
        m_swapChain = m_engine->createSwapChain(m_nativeWindow);
    }
}

void FilamentEngine::setCameraPosition(const filament::math::float3& position)
{
    if (m_camera) {
        // 设置相机位置，看向原点
        m_camera->lookAt(position, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    }
}

void FilamentEngine::setCameraTarget(const filament::math::float3& target)
{
    if (m_camera) {
        // 设置相机看向目标，相机位置保持不变
        filament::math::float3 position = {0.0f, 0.0f, 5.0f};
        m_camera->lookAt(position, target, {0.0f, 1.0f, 0.0f});
    }
}

void FilamentEngine::setCameraProjection(float fov, float aspect, float near, float far)
{
    if (m_camera) {
        m_camera->setProjection(fov, aspect, near, far, filament::Camera::Fov::VERTICAL);
    }
}
