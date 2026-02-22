#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <functional>

#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>
#include <filament/View.h>
#include <filament/Camera.h>
#include <filament/SwapChain.h>
#include <filament/Viewport.h>

#include <filagui/ImGuiHelper.h>

#include <utils/Entity.h>
#include <utils/EntityManager.h>

#include <viewer/Settings.h>

#include <QString>

class Renderer {
public:
    Renderer();
    ~Renderer();

    // ── Lifecycle ───────────────────────────────────────────────
    void initialize(void* nativeWindow, int width, int height);
    void shutdown();

    void renderFrame();
    void resize(int width, int height);
    void clearPixel();

    // ── Camera ──────────────────────────────────────────────────
    void setCameraPosition(const filament::math::float3& position);
    void setCameraTarget(const filament::math::float3& target);
    void setCameraProjection(float fov, float aspect, float nearPlane, float farPlane);

    // ── Scene And View access ────────────────────────────────────────────
    filament::Engine* engine() const { return m_engine; }
    filament::Renderer* renderer() const { return m_renderer; }
    filament::SwapChain* swapChain() const { return m_swapChain; }
    filament::Scene* scene()  const { return m_scene; }
    filament::Scene* uiScene()  const { return m_uiScene; }
    filament::View* view()  const { return m_view; }
    filament::View* uiView()  const { return m_uiView; }

    // ── Add Ligth access ────────────────────────────────────────────
    void addDirectionalLight(const filament::math::float3& direction, const filament::math::float3& color, float intensity);
    void addPointLight(const filament::math::float3& position, const filament::math::float3& color, float intensity, float radius);

    // ── Add Renderable access ────────────────────────────────────────────
    void addCube(const filament::math::float3& position, const filament::math::float3& size, const filament::math::float3& color);
    void addGltfModel(const QString& file);
    
    // ── Load IBL access ────────────────────────────────────────────
    bool loadAndApplyIBL(const QString& iblPath);

private:
    // ── Filament core ───────────────────────────────────────────
    filament::Engine* m_engine = nullptr;
    filament::Renderer* m_renderer = nullptr;
    filament::Scene* m_scene = nullptr;
    filament::Scene* m_uiScene = nullptr;
    filament::View* m_view = nullptr;
    filament::View* m_uiView = nullptr;
    filament::Camera* m_camera = nullptr;
    filament::Camera* m_uiCamera = nullptr;
    filament::SwapChain* m_swapChain = nullptr;

    // ── State ───────────────────────────────────────────────────
    void* m_nativeWindow = nullptr;
    int   m_width = 0;
    int   m_height = 0;

    // ── Setting ───────────────────────────────────────────────────
    filament::viewer::ViewSettings m_settings;

    // ── MISC ───────────────────────────────────────────────────
    std::string m_gltfDirectory;
};

#endif // RENDERER_H