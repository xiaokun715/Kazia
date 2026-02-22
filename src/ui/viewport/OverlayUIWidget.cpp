#include "OverlayUIWidget.h"

#include <QCoreApplication>
#include <QString>

#include <imgui.h>
#include <imgui_internal.h>

OverlayUIWidget::OverlayUIWidget() = default;
OverlayUIWidget::~OverlayUIWidget() = default;

void OverlayUIWidget::initialize(
    filament::Engine* engine,
    filament::View* uiView,
    int width,
    int height
) {
    m_engine = engine;
    m_uiView = uiView;
    m_width = width;
    m_height = height;

    QString fontPath =
        QCoreApplication::applicationDirPath() + "/fonts/Roboto-Medium.ttf";

    m_imguiHelper = std::make_unique<filagui::ImGuiHelper>(
        engine,
        uiView,
        utils::Path(fontPath.toUtf8().constData())
    );

    updateDisplaySize(width, height, 1.0f);
    setupEditorStyle();
}

void OverlayUIWidget::resize(int width, int height, float scale)
{
    m_width = width;
    m_height = height;
    updateDisplaySize(width, height, scale);
}

void OverlayUIWidget::updateDisplaySize(int width, int height, float scale)
{
    if (!m_imguiHelper) return;
    m_imguiHelper->setDisplaySize(width, height, scale, scale);
}

void OverlayUIWidget::render(float deltaTime)
{
    if (!m_imguiHelper) return;

    updateDisplaySize(m_width, m_height, 1.0f);

    m_imguiHelper->render(deltaTime,
        [&](filament::Engine*, filament::View*) {
            drawViewportToolbar();
        });
}

// ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤
// Style
// ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤
void OverlayUIWidget::setupEditorStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();

    QString fontPath =
        QCoreApplication::applicationDirPath() + "/fonts/Roboto-Medium.ttf";
    QString iconFontPath =
        QCoreApplication::applicationDirPath() + "/fonts/iconfont.ttf";

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

    ImFontConfig cfg;
    cfg.MergeMode = true;
    cfg.PixelSnapH = true;

    static const ImWchar ranges[] = { 0xE600, 0xF000, 0 };

    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF(fontPath.toUtf8().constData(), 28.0f);
    io.Fonts->AddFontFromFileTTF(iconFontPath.toUtf8().constData(), 28.0f, &cfg, ranges);
    io.Fonts->Build();

    style.ScaleAllSizes(1.8f);
}

// ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤
// Toolbar
// ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤
void OverlayUIWidget::drawViewportToolbar()
{
    ImGuiIO& io = ImGui::GetIO();
    const float toolbarHeight = 40.0f;

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, toolbarHeight), ImGuiCond_Always);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.13f, 0.13f, 0.13f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.2f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.30f, 1.0f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.85f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.88f, 0.88f, 0.88f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.40f, 0.40f, 0.40f, 0.8f));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 3.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);

    ImGui::Begin("ViewportToolbar", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav
    );

    const float winH = ImGui::GetWindowHeight();
    const float frameH = ImGui::GetFrameHeight();
    const float cy = (winH - frameH) * 0.5f;
    const float fp2 = ImGui::GetStyle().FramePadding.x * 2.0f;

    // Emit action and update active tool if it is a tool button.
    auto emitImguiSignal = [&](ToolbarAction action) {
        if (action == ToolbarAction::SelectTool || action == ToolbarAction::MoveTool ||
            action == ToolbarAction::RotateTool || action == ToolbarAction::ScaleTool) {
            m_activeTool = action;
        }
        if (m_onToolbarAction) m_onToolbarAction(action);
        };

    // Plain button ¡ª returns true on click.
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

    // ©¤©¤ LEFT GROUP ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤
    // Hamburger button opens a popup dropdown menu
    ImGui::SetCursorPosY(cy);
    if (ImGui::Button(ICON_MENU_UTF8))
        ImGui::OpenPopup("##ViewportMenu");

    // Popup styles MUST be pushed before BeginPopup to affect the window itself.
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.10f, 0.10f, 0.10f, 0.97f));
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.20f, 0.38f, 0.75f, 0.50f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.22f, 0.42f, 0.80f, 0.80f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.28f, 0.50f, 0.90f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.90f, 0.90f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(0.45f, 0.45f, 0.45f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.28f, 0.28f, 0.28f, 1.00f));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6.0f, 8.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(24.0f, 8.0f));

    if (ImGui::BeginPopup("##ViewportMenu")) {

        // ©¤©¤ VIEW ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤
        ImGui::TextDisabled("VIEW");
        ImGui::Spacing();

        if (ImGui::BeginMenu("View Mode")) {
            if (ImGui::MenuItem("Perspective", "P")) emitImguiSignal(ToolbarAction::Perspective);
            if (ImGui::MenuItem("Lit", "L")) emitImguiSignal(ToolbarAction::Lit);
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Show / Hide...")) emitImguiSignal(ToolbarAction::Show);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // ©¤©¤ TRANSFORM ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤
        ImGui::TextDisabled("TRANSFORM");
        ImGui::Spacing();

        if (ImGui::BeginMenu("Transform Tool")) {
            if (ImGui::MenuItem("Select", "Q", m_activeTool == ToolbarAction::SelectTool))
                emitImguiSignal(ToolbarAction::SelectTool);
            if (ImGui::MenuItem("Move", "W", m_activeTool == ToolbarAction::MoveTool))
                emitImguiSignal(ToolbarAction::MoveTool);
            if (ImGui::MenuItem("Rotate", "E", m_activeTool == ToolbarAction::RotateTool))
                emitImguiSignal(ToolbarAction::RotateTool);
            if (ImGui::MenuItem("Scale", "R", m_activeTool == ToolbarAction::ScaleTool))
                emitImguiSignal(ToolbarAction::ScaleTool);
            ImGui::EndMenu();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // ©¤©¤ SPACE ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤
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

    // ©¤©¤ RIGHT GROUP (right-aligned) ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤
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

    // Transform tools ¡ª highlighted when active
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