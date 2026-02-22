#ifndef OVERLAY_UI_WIDGET_H
#define OVERLAY_UI_WIDGET_H

#include <memory>
#include <functional>

#include <filagui/ImGuiHelper.h>
#include <filament/Engine.h>
#include <filament/View.h>
#include <filament/Camera.h>

// ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤
// Icon UTF8
// ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤
#define ICON_GRID              u8"\ue790"
#define ICON_WORLD             u8"\uece7"
#define ICON_CAMERA            u8"\ue88d"
#define ICON_SELECT            u8"\ue639"
#define ICON_MOVE              u8"\ue67b"
#define ICON_NORMAL            u8"\uec48"
#define ICON_ROTATE            u8"\ue8b8"
#define ICON_SCALE             u8"\ue70f"
#define ICON_ANGLE             u8"\ue67d"
#define ICON_SHOW              u8"\ue760"
#define ICON_SQUARE            u8"\ue656"
#define ICON_PERSPECTIVE       u8"\ue694"
#define ICON_MENU              u8"\ue677"
#define ICON_METRIC            u8"\ue624"
#define ICON_LIGHT             u8"\ue66d"
#define ICON_GRID_UTF8         "\xEE\x9E\x90"
#define ICON_WORLD_UTF8        "\xEE\xB3\xA7"
#define ICON_CAMERA_UTF8       "\xEE\xA2\x8D"
#define ICON_SELECT_UTF8       "\xEE\x98\xB9"
#define ICON_MOVE_UTF8         "\xEE\x99\xBB"
#define ICON_NORMAL_UTF8       "\xEE\xB1\x88"
#define ICON_ROTATE_UTF8       "\xEE\xA2\xB8"
#define ICON_SCALE_UTF8        "\xEE\x9C\x8F"
#define ICON_ANGLE_UTF8        "\xEE\x99\xBD"
#define ICON_SHOW_UTF8         "\xEE\x9D\xA0"
#define ICON_SQUARE_UTF8       "\xEE\x99\x96"
#define ICON_PERSPECTIVE_UTF8  "\xEE\x9A\x94"
#define ICON_MENU_UTF8         "\xEE\x99\xB7"
#define ICON_METRIC_UTF8       "\xEE\x98\xA4"
#define ICON_LIGHT_UTF8        "\xEE\x99\xAD"

enum class ToolbarAction {
    None,
    Perspective,
    Lit,
    Show,

    SelectTool,
    MoveTool,
    RotateTool,
    ScaleTool,

    WorldSpace,
    LocalSpace,

    GridSize,
    AngleSnap,
    LocationSnap,
    CameraSpeed,
    More
};

class OverlayUIWidget {
public:
    using ToolbarCallback = std::function<void(ToolbarAction)>;

    OverlayUIWidget();
    ~OverlayUIWidget();

    void initialize(
        filament::Engine* engine,
        filament::View* uiView,
        int width,
        int height
    );

    void resize(int width, int height, float scale = 1.0f);

    void render(float deltaTime);

    void setToolbarCallback(ToolbarCallback cb) {
        m_onToolbarAction = std::move(cb);
    }

    void updateDisplaySize(int width, int height, float scale);
    void setupEditorStyle();

    void drawViewportToolbar();

private:
    filament::Engine* m_engine = nullptr;
    filament::View* m_uiView = nullptr;

    std::unique_ptr<filagui::ImGuiHelper> m_imguiHelper;

    int m_width = 0;
    int m_height = 0;

    ToolbarAction  m_activeTool = ToolbarAction::SelectTool;
    ToolbarCallback m_onToolbarAction;
};

#endif // OVERLAY_UI_WIDGET_H