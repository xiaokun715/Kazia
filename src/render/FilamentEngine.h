#ifndef FILAMENTENGINE_H
#define FILAMENTENGINE_H

#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>
#include <filament/Camera.h>
#include <filament/SwapChain.h>
#include <filament/View.h>
#include <filament/Viewport.h>
#include <filagui/ImGuiHelper.h>
#include <QEvent>
#include <functional>

// Toolbar actions emitted by ImGui widgets each frame they are clicked.
enum class ToolbarAction {
    None,
    // View
    Perspective, Lit, Show,
    // Transform tools
    SelectTool, MoveTool, RotateTool, ScaleTool,
    // Coordinate space
    WorldSpace, LocalSpace,
    // Snap / camera
    GridSize, AngleSnap, LocationSnap, CameraSpeed,
    // Misc
    Menu, More,
};

class FilamentEngine
{
private:
    filament::Engine* m_engine;
    filament::Renderer* m_renderer;
    filament::Scene* m_scene;
    filament::Scene* m_uiScene;
    filament::View* m_viewScene;
    filament::View* m_viewUI;
    filament::Camera* m_camera;
    filament::Camera* m_uiCamera;
    filament::SwapChain* m_swapChain;
    std::unique_ptr<filagui::ImGuiHelper> m_imguiHelper;
    void* m_nativeWindow;

    int m_width;
    int m_height;

    // Active tool tracked for button highlight
    ToolbarAction m_activeTool = ToolbarAction::SelectTool;

public:
    FilamentEngine();
    ~FilamentEngine();

    // Register a handler that is called (on the render thread) whenever a
    // toolbar button is clicked. Keep the handler lightweight — post to the
    // main thread if you need to mutate editor state.
    std::function<void(ToolbarAction)> onToolbarAction;

    void initialize(void* nativeWindow, int width, int height);
    void shutdown();
    void clearPixel();
    void drawImGui();
    void SetupEditorStyle();
    void drawViewportToolbar();

    bool handleQtEvent(QEvent* event);
    void updateImGuiDisplaySize(int width, int height, float scale = 1.0f);

    void render();
    void resize(int width, int height);

    filament::Engine* getEngine() const { return m_engine; }
    filament::Renderer* getRenderer() const { return m_renderer; }
    filament::Scene* getScene() const { return m_scene; }
    filament::View* getView() const { return m_viewScene; }
    filament::Camera* getCamera() const { return m_camera; }

    void setCameraPosition(const filament::math::float3& position);
    void setCameraTarget(const filament::math::float3& target);
    void setCameraProjection(float fov, float aspect, float near, float far);
};

#endif // FILAMENTENGINE_H
