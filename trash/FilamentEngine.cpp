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

#define ICON_GRID   u8"\ue790"
#define ICON_WORLD  u8"\uece7"
#define ICON_CAMERA u8"\ue88d"
#define ICON_SELECT u8"\ue639"
#define ICON_MOVE u8"\ue67b"
#define ICON_NORMAL u8"\uec48"
#define ICON_ROTATE u8"\ue8b8"
#define ICON_SCALE u8"\ue70f"
#define ICON_ANGLE u8"\ue67d"
#define ICON_SHOW u8"\ue760"
#define ICON_SQUARE u8"\ue656"
#define ICON_PERSPECTIVE u8"\ue694"
#define ICON_MENU u8"\ue677"
#define ICON_METRIC u8"\ue624"
#define ICON_LIGHT u8"\ue66d"

#define ICON_GRID_UTF8         "\xEE\x9E\x90"  // U+E790
#define ICON_WORLD_UTF8        "\xEE\xB3\xA7"  // U+ECE7
#define ICON_CAMERA_UTF8       "\xEE\xA2\x8D"  // U+E88D
#define ICON_SELECT_UTF8       "\xEE\x98\xB9"  // U+E639
#define ICON_MOVE_UTF8         "\xEE\x99\xBB"  // U+E67B
#define ICON_NORMAL_UTF8       "\xEE\xB1\x88"  // U+EC48
#define ICON_ROTATE_UTF8       "\xEE\xA2\xB8"  // U+E8B8
#define ICON_SCALE_UTF8        "\xEE\x9C\x8F"  // U+E70F
#define ICON_ANGLE_UTF8        "\xEE\x99\xBD"  // U+E67D
#define ICON_SHOW_UTF8         "\xEE\x9D\xA0"  // U+E760
#define ICON_SQUARE_UTF8       "\xEE\x99\x96"  // U+E656
#define ICON_PERSPECTIVE_UTF8  "\xEE\x9A\x94"  // U+E694
#define ICON_MENU_UTF8         "\xEE\x99\xB7"  // U+E677
#define ICON_METRIC_UTF8       "\xEE\x98\xA4"  // U+E624
#define ICON_LIGHT_UTF8        "\xEE\x99\xAD"  // U+E66D

#define USE_ICON_FONT(expr) do {                \
    ImGuiIO& io = ImGui::GetIO();               \
    ImFont* iconFont = io.Fonts->Fonts[1];      \
    ImGui::PushFont(iconFont);                  \
    expr;                                       \
    ImGui::PopFont();                           \
} while (0)

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

    //// ImGui 初始化（移植 SDL 版本的核心逻辑）
    QString ttfPath = QCoreApplication::applicationDirPath() + "/fonts/Roboto-Medium.ttf";
    m_imguiHelper = std::make_unique<filagui::ImGuiHelper>(m_engine, m_viewUI, utils::Path(ttfPath.toUtf8().constData()));

    //// 设置初始 DisplaySize（防止初始化断言崩溃）
    updateImGuiDisplaySize(width, height);

    // 设置imgui样式
    SetupEditorStyle();
}

void FilamentEngine::SetupEditorStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();
    QString ttfPath = QCoreApplication::applicationDirPath() + "/fonts/Roboto-Medium.ttf";
    QString iconTtfPath = QCoreApplication::applicationDirPath() + "/fonts/iconfont.ttf";

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
    ImFontConfig cfg;
    cfg.MergeMode = true;
    cfg.PixelSnapH = true;
    static const ImWchar glyphRanges[] = {
        0xE604, 0xECE7, // Basic Latin + Latin Supplement
        0
    };
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF(ttfPath.toUtf8().constData(), 28.0f);
    io.Fonts->AddFontFromFileTTF(iconTtfPath.toUtf8().constData(), 28.0f, &cfg, glyphRanges);
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
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.30f, 1.0f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.0f,  0.0f,  0.85f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text,          ImVec4(0.88f, 0.88f, 0.88f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Separator,     ImVec4(0.40f, 0.40f, 0.40f, 0.8f));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,   ImVec2(2.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,  ImVec2(6.0f, 3.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);

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
    // Hamburger button opens a popup dropdown menu
    ImGui::SetCursorPosY(cy);
    if (ImGui::Button(ICON_MENU_UTF8))
        ImGui::OpenPopup("##ViewportMenu");

    // Popup styles MUST be pushed before BeginPopup to affect the window itself.
    ImGui::PushStyleColor(ImGuiCol_PopupBg,       ImVec4(0.10f, 0.10f, 0.10f, 0.97f));
    ImGui::PushStyleColor(ImGuiCol_Header,        ImVec4(0.20f, 0.38f, 0.75f, 0.50f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.22f, 0.42f, 0.80f, 0.80f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive,  ImVec4(0.28f, 0.50f, 0.90f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Text,          ImVec4(0.90f, 0.90f, 0.90f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_TextDisabled,  ImVec4(0.45f, 0.45f, 0.45f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Separator,     ImVec4(0.28f, 0.28f, 0.28f, 1.00f));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6.0f, 8.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,   ImVec2(24.0f, 8.0f));

    if (ImGui::BeginPopup("##ViewportMenu")) {

        // ── VIEW ────────────────────────────────────────────────────────
        ImGui::TextDisabled("VIEW");
        ImGui::Spacing();

        if (ImGui::BeginMenu("View Mode")) {
            if (ImGui::MenuItem("Perspective", "P")) emitImguiSignal(ToolbarAction::Perspective);
            if (ImGui::MenuItem("Lit",         "L")) emitImguiSignal(ToolbarAction::Lit);
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Show / Hide...")) emitImguiSignal(ToolbarAction::Show);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // ── TRANSFORM ───────────────────────────────────────────────────
        ImGui::TextDisabled("TRANSFORM");
        ImGui::Spacing();

        if (ImGui::BeginMenu("Transform Tool")) {
            if (ImGui::MenuItem("Select", "Q", m_activeTool == ToolbarAction::SelectTool))
                emitImguiSignal(ToolbarAction::SelectTool);
            if (ImGui::MenuItem("Move",   "W", m_activeTool == ToolbarAction::MoveTool))
                emitImguiSignal(ToolbarAction::MoveTool);
            if (ImGui::MenuItem("Rotate", "E", m_activeTool == ToolbarAction::RotateTool))
                emitImguiSignal(ToolbarAction::RotateTool);
            if (ImGui::MenuItem("Scale",  "R", m_activeTool == ToolbarAction::ScaleTool))
                emitImguiSignal(ToolbarAction::ScaleTool);
            ImGui::EndMenu();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // ── SPACE ───────────────────────────────────────────────────────
        ImGui::TextDisabled("COORDINATE SPACE");
        ImGui::Spacing();

        if (ImGui::MenuItem("World Space", nullptr, m_activeTool == ToolbarAction::WorldSpace))
            emitImguiSignal(ToolbarAction::WorldSpace);
        if (ImGui::MenuItem("Local Space", nullptr, m_activeTool == ToolbarAction::LocalSpace))
            emitImguiSignal(ToolbarAction::LocalSpace);

        ImGui::EndPopup();
    }

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(7);

    VSep();
    if (Btn(ICON_PERSPECTIVE" Perspective")) emitImguiSignal(ToolbarAction::Perspective);
    VSep();
    if (Btn(ICON_LIGHT" Lig"))         emitImguiSignal(ToolbarAction::Lit);
    VSep();
    if (Btn(ICON_SHOW" SHOW"))        emitImguiSignal(ToolbarAction::Show);
    VSep();

    // ── RIGHT GROUP (right-aligned) ──────────────────────────────────────
    auto calcW = [&](const char8_t* lbl) -> float {
        return ImGui::CalcTextSize(reinterpret_cast<const char*>(lbl), nullptr, true).x + fp2 + 2.0f;
    };
    const float sepW = 6.0f + 1.0f + 6.0f;

    float rightW =
        calcW(ICON_SELECT) + calcW(ICON_MOVE) + calcW(ICON_ROTATE) + calcW(ICON_SCALE) + 2.0f * 3 +
        sepW +
        calcW(ICON_WORLD) + calcW(ICON_NORMAL) + 2.0f +
        sepW +
        calcW(ICON_GRID) + calcW(ICON_ANGLE) + calcW(ICON_METRIC) + calcW(ICON_CAMERA) + calcW(ICON_SQUARE) + 2.0f * 4 +
        10.0f;

    ImGui::SameLine();
    ImGui::SetCursorPosX(io.DisplaySize.x - rightW);

    // Transform tools — highlighted when active
    ToolBtn(ICON_SELECT, ToolbarAction::SelectTool);
    ImGui::SameLine(0, 2); ToolBtn(ICON_MOVE, ToolbarAction::MoveTool);
    ImGui::SameLine(0, 2); ToolBtn(ICON_ROTATE, ToolbarAction::RotateTool);
    ImGui::SameLine(0, 2); ToolBtn(ICON_SCALE, ToolbarAction::ScaleTool);

    VSep();

    if (Btn(ICON_WORLD)) emitImguiSignal(ToolbarAction::WorldSpace);
    ImGui::SameLine(0, 2);
    if (Btn(ICON_NORMAL)) emitImguiSignal(ToolbarAction::LocalSpace);

    VSep();
    if (Btn(ICON_GRID))  emitImguiSignal(ToolbarAction::GridSize);
    ImGui::SameLine(0, 2);
    if (Btn(ICON_ANGLE))  emitImguiSignal(ToolbarAction::AngleSnap);
    ImGui::SameLine(0, 2);
    if (Btn(ICON_METRIC)) emitImguiSignal(ToolbarAction::LocationSnap);
    ImGui::SameLine(0, 2);
    if (Btn(ICON_CAMERA)) emitImguiSignal(ToolbarAction::CameraSpeed);
    ImGui::SameLine(0, 2);
    if (Btn(ICON_SQUARE))     emitImguiSignal(ToolbarAction::More);

    ImGui::PopStyleVar(4);
    ImGui::PopStyleColor(6);
    ImGui::End();
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
