#include "FilamentEngine.h"
#include "app/AppConfig.h"
#include <utils/EntityManager.h>
#include <utils/Entity.h>
#include <backend/DriverEnums.h>
#include <filagui/ImGuiHelper.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_win32.h>
#include <QString>
#include <QCoreApplication>
#include <QClipboard>
#include <QApplication>
#include <qpaintdevice.h>
#include <iostream>


FilamentEngine::FilamentEngine()
    : m_engine(nullptr)
    , m_renderer(nullptr)
    , m_scene(nullptr)
    , m_viewScene(nullptr)
    , m_uiScene(nullptr)
    , m_viewUI(nullptr)
    , m_camera(nullptr)
    , m_uiCamera(nullptr)
    , m_swapChain(nullptr)
    , m_nativeWindow(nullptr)
    , m_width(0)
    , m_height(0)
{}

FilamentEngine::~FilamentEngine()
{
    shutdown();
}

void FilamentEngine::initialize(void* nativeWindow, int width, int height)
{
    m_nativeWindow = nativeWindow;
    m_width = width;
    m_height = height;

    m_engine = filament::Engine::create(filament::backend::Backend::OPENGL);
    if (!m_engine) {
        return;
    }

    m_renderer = m_engine->createRenderer();
    m_scene = m_engine->createScene();

    utils::Entity cameraEntity = utils::EntityManager::get().create();
    m_camera = m_engine->createCamera(cameraEntity);

    m_viewScene = m_engine->createView();
    m_viewScene->setScene(m_scene);
    m_viewScene->setCamera(m_camera);
    m_viewScene->setFroxelVizEnabled(true);

    m_viewUI = m_engine->createView();
    m_uiScene = m_engine->createScene();
    m_viewUI->setScene(m_uiScene);
    utils::Entity uiCameraEntity = utils::EntityManager::get().create();
    m_uiCamera = m_engine->createCamera(uiCameraEntity);
    m_uiCamera->setProjection(
        filament::Camera::Projection::ORTHO,
        0.0, (double)width,      // left, right
        0.0, (double)height,     // bottom, top (必须 bottom < top，不能反)
        -1.0, 1.0                // near, far
    );
    m_viewUI->setCamera(m_uiCamera);

    std::cout << m_engine->getViewCount() << std::endl;
    std::cout << m_viewScene->getVisibleLayers() << std::endl;

    m_swapChain = m_engine->createSwapChain(m_nativeWindow);

    filament::Viewport viewport(0, 0, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    m_viewScene->setViewport(viewport);
    m_viewUI->setViewport(viewport);

    AppConfig cfg = loadAppConfig();

    //// ImGui 初始化（移植 SDL 版本的核心逻辑）
    QString ttfPath = QCoreApplication::applicationDirPath() + cfg.defaultImguiTtf;
    m_imguiHelper = std::make_unique<filagui::ImGuiHelper>(
        m_engine, m_viewUI, utils::Path(ttfPath.toUtf8().constData()));

    //// 设置初始 DisplaySize（防止初始化断言崩溃）
    updateImGuiDisplaySize(width, height);

    // 设置imgui样式
    SetupEditorStyle();
}

void FilamentEngine::SetupEditorStyle()
{
    AppConfig cfg = loadAppConfig();

    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();
    QString ttfPath = QCoreApplication::applicationDirPath() + cfg.defaultImguiTtf;

    style.WindowBorderSize = 0.0f;
    style.ChildBorderSize = 0.0f;
    style.PopupBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.TabBorderSize = 0.0f;

    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.PopupRounding = 0.0f;

    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(8, 6);
    style.ItemSpacing = ImVec2(8, 6);
    style.ItemInnerSpacing = ImVec2(6, 6);

    style.ScrollbarSize = 16.0f;
    style.GrabMinSize = 12.0f;

    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 5.0f;

    // Glyph ranges: Basic Latin + Latin Supplement + Arrows + Mathematical Operators
    // This lets ImGui atlas include the Unicode symbols used in the toolbar.
    static const ImWchar glyphRanges[] = {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x2190, 0x22FF, // Arrows (↖ ↻ ⇔ …) + Mathematical Operators (≡ ∠ ⊕ ⊞ …)
        0, 0xE790
    };
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF(ttfPath.toUtf8().constData(), 28.0f, nullptr, glyphRanges);
    io.Fonts->Build();

    // 🔴 最后统一 scale
    style.ScaleAllSizes(2.0f);
}

void FilamentEngine::updateImGuiDisplaySize(int width, int height, float scale)
{
    if (!m_imguiHelper) return;

    float dpr = 1.f;
    // 使用 ImGuiHelper 的 setDisplaySize 方法，而不是直接修改 ImGuiIO
    // 这确保 ImGuiHelper 内部状态（包括投影矩阵）被正确更新
    m_imguiHelper->setDisplaySize(width, height, scale * dpr, scale * dpr);
}

void FilamentEngine::render()
{
    if (!m_renderer || !m_swapChain) return;

    // 每帧更新 DisplaySize（关键！工作代码每帧都调用）
    // 必须在 beginFrame 之前调用，确保 ImGuiHelper 内部状态同步
    if (m_imguiHelper && m_width > 0 && m_height > 0) {
        updateImGuiDisplaySize(m_width, m_height, 1.0f);
    }

    if (m_renderer->beginFrame(m_swapChain)) {

        m_imguiHelper->render(0.016f, [&](filament::Engine*, filament::View*) {
            this->drawViewportToolbar();
        });

        auto* drawData = ImGui::GetDrawData();

        // 渲染 Filament 视图
        m_renderer->render(m_viewScene);
        m_renderer->render(m_viewUI);

        m_renderer->endFrame();
    }
}

void FilamentEngine::resize(int width, int height)
{
    // 存储窗口尺寸（用于每帧更新 ImGui DisplaySize）
    m_width = width;
    m_height = height;

    if (m_viewScene) {
        filament::Viewport viewport(0, 0, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
        m_viewScene->setViewport(viewport);
    }

    if (m_viewUI) {
        filament::Viewport viewport(0, 0, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
        m_viewUI->setViewport(viewport);
    }

    // 更新 UI Camera 投影（关键！）
    if (m_uiCamera) {
        m_uiCamera->setProjection(
            filament::Camera::Projection::ORTHO,
            0.0, (double)width,      // left, right
            0.0, (double)height,     // bottom, top
            -1.0, 1.0
        );
    }

    if (m_camera) {
        setCameraProjection(45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
    }

    // 更新 ImGui DisplaySize
    updateImGuiDisplaySize(width, height);

    // 在窗口大小变化较大时（如最大化），重新创建交换链
    if (m_engine && m_nativeWindow && m_swapChain) {
        // 销毁旧的交换链
        m_engine->destroy(m_swapChain);
        m_swapChain = nullptr;

        // 创建新的交换链
        m_swapChain = m_engine->createSwapChain(m_nativeWindow);
    }
}

void FilamentEngine::drawViewportToolbar()
{
    ImGuiIO& io = ImGui::GetIO();
    const float toolbarHeight = 40.0f;

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, toolbarHeight), ImGuiCond_Always);

    ImGui::PushStyleColor(ImGuiCol_WindowBg,      ImVec4(0.13f, 0.13f, 0.13f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.0f,  0.0f,  0.0f,  0.2f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.30f, 0.30f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.0f,  0.0f,  0.85f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text,          ImVec4(0.88f, 0.88f, 0.88f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Separator,     ImVec4(0.40f, 0.40f, 0.40f, 0.8f));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,   ImVec2(2.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,  ImVec2(6.0f, 3.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);

    ImGui::Begin("ViewportToolbar", nullptr,
        ImGuiWindowFlags_NoDecoration      |
        ImGuiWindowFlags_NoMove            |
        ImGuiWindowFlags_NoResize          |
        ImGuiWindowFlags_NoSavedSettings   |
        ImGuiWindowFlags_NoFocusOnAppearing|
        ImGuiWindowFlags_NoNav
    );

    const float winH   = ImGui::GetWindowHeight();
    const float frameH = ImGui::GetFrameHeight();
    const float cy     = (winH - frameH) * 0.5f;
    const float fp2    = ImGui::GetStyle().FramePadding.x * 2.0f;

    // Emit action and update active tool if it is a tool button.
    auto emitImguiSignal = [&](ToolbarAction action) {
        if (action == ToolbarAction::SelectTool || action == ToolbarAction::MoveTool ||
            action == ToolbarAction::RotateTool || action == ToolbarAction::ScaleTool) {
            m_activeTool = action;
        }
        if (onToolbarAction) onToolbarAction(action);
    };

    // Plain button — returns true on click.
    auto Btn = [&](const char8_t* label) -> bool {
        ImGui::SetCursorPosY(cy);
        return ImGui::Button(reinterpret_cast<const char*>(label));
    };

    // Tool button: highlighted when it is the active tool.
    auto ToolBtn = [&](const char8_t* label, ToolbarAction action) {
        bool active = (m_activeTool == action);
        if (active)
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.40f, 0.80f, 1.0f));
        if (Btn(label)) emitImguiSignal(action);
        if (active)
            ImGui::PopStyleColor();
    };

    auto VSep = [&]() {
        ImGui::SameLine(0.0f, 6.0f);
        ImGui::SetCursorPosY(cy);
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
        ImGui::SameLine(0.0f, 6.0f);
    };

    // ── LEFT GROUP ──────────────────────────────────────────────────────
    if (Btn(u8"="))           emitImguiSignal(ToolbarAction::Menu);
    VSep();
    if (Btn(u8"Perspective")) emitImguiSignal(ToolbarAction::Perspective);
    VSep();
    if (Btn(u8"Lit"))         emitImguiSignal(ToolbarAction::Lit);
    VSep();
    if (Btn(u8"Show"))        emitImguiSignal(ToolbarAction::Show);
    VSep();

    // Red badge (no action needed, just visual)
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.72f, 0.10f, 0.10f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.82f, 0.20f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.60f, 0.05f, 0.05f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
    Btn(u8" 1 ");
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);

    // ── RIGHT GROUP (right-aligned) ──────────────────────────────────────
    auto calcW = [&](const char8_t* lbl) -> float {
        return ImGui::CalcTextSize(reinterpret_cast<const char*>(lbl), nullptr, true).x + fp2 + 2.0f;
    };
    const float sepW = 6.0f + 1.0f + 6.0f;

    float rightW =
        calcW(u8"Sel") + calcW(u8"Mov") + calcW(u8"Rot") + calcW(u8"Scl") + 2.0f * 3 +
        sepW +
        calcW(u8"Wld") + calcW(u8"Lcl") + 2.0f +
        sepW +
        calcW(u8"# 10") + calcW(u8"^ 10") + calcW(u8"~ .25") + calcW(u8"Cam 1") + calcW(u8"+") + 2.0f * 4 +
        10.0f;

    ImGui::SameLine();
    ImGui::SetCursorPosX(io.DisplaySize.x - rightW);

    // Transform tools — highlighted when active
    ToolBtn(u8"Sel", ToolbarAction::SelectTool);
    ImGui::SameLine(0, 2); ToolBtn(u8"Mov", ToolbarAction::MoveTool);
    ImGui::SameLine(0, 2); ToolBtn(u8"Rot", ToolbarAction::RotateTool);
    ImGui::SameLine(0, 2); ToolBtn(u8"Scl", ToolbarAction::ScaleTool);

    VSep();

    if (Btn(u8"Wld")) emitImguiSignal(ToolbarAction::WorldSpace);
    ImGui::SameLine(0, 2);
    if (Btn(u8"Lcl")) emitImguiSignal(ToolbarAction::LocalSpace);

    VSep();

    if (Btn(u8"# 10"))  emitImguiSignal(ToolbarAction::GridSize);
    ImGui::SameLine(0, 2);
    if (Btn(u8"^ 10"))  emitImguiSignal(ToolbarAction::AngleSnap);
    ImGui::SameLine(0, 2);
    if (Btn(u8"~ .25")) emitImguiSignal(ToolbarAction::LocationSnap);
    ImGui::SameLine(0, 2);
    if (Btn(u8"Cam 1")) emitImguiSignal(ToolbarAction::CameraSpeed);
    ImGui::SameLine(0, 2);
    if (Btn(u8"+"))     emitImguiSignal(ToolbarAction::More);

    ImGui::PopStyleVar(4);
    ImGui::PopStyleColor(6);
    ImGui::End();
}

void FilamentEngine::drawImGui()
{
    if (!m_imguiHelper) return;

    updateImGuiDisplaySize(500, 500, 1.0f);

    // 渲染 ImGui
    m_imguiHelper->render(0.016f, [this](filament::Engine* engine, filament::View* view) {
        // 自定义 UI：Viewport Overlay 工具栏
        ImGui::SetNextWindowPos(ImVec2(0, 0));  // 固定在左上角
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 32));  // 固定高度 32px

        ImGui::Begin("Viewport Overlay", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoBackground);  // 透明背景

        // 工具栏布局（横向排列）
        ImGui::BeginGroup();  // 左侧按钮组

        if (ImGui::Button("Perspective")) {
            // 处理点击事件
        }
        ImGui::SameLine();

        if (ImGui::Button("Lit")) {
            // 处理点击
        }
        ImGui::SameLine();

        if (ImGui::Button("Show")) {
            // 处理点击
        }

        ImGui::EndGroup();

        // 中间填充
        ImGui::SameLine();
        ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0));

        // 右侧图标/状态（示例）
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "FPS: %.1f", ImGui::GetIO().Framerate);

        ImGui::End();
     });
}

void FilamentEngine::clearPixel()
{
    // 设置清除选项
    filament::Renderer::ClearOptions clearOptions;
    clearOptions.clear = true;
    clearOptions.clearColor = { 0.1f, 0.1f, 0.1f, 1.0f }; // 深灰色背景
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
        if (m_viewScene) {
            try {
                m_engine->destroy(m_viewScene);
                m_viewScene = nullptr;
            } catch (...) {
                // 忽略销毁视图时的错误
                m_viewScene = nullptr;
            }
        }

        if (m_viewUI) {
            try {
                m_engine->destroy(m_viewUI);
                m_viewUI = nullptr;
            } catch (...) {
                m_viewUI = nullptr;
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

        if (m_uiScene) {
            try {
                m_engine->destroy(m_uiScene);
                m_uiScene = nullptr;
            } catch (...) {
                m_uiScene = nullptr;
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
        m_uiCamera = nullptr;
        m_nativeWindow = nullptr;
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
