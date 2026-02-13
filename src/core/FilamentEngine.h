#ifndef FILAMENTENGINE_H
#define FILAMENTENGINE_H

#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>
#include <filament/Camera.h>
#include <filament/SwapChain.h>
#include <filament/View.h>
#include <filament/Viewport.h>

class FilamentEngine
{
private:
    filament::Engine* m_engine;
    filament::Renderer* m_renderer;
    filament::Scene* m_scene;
    filament::View* m_view;
    filament::Camera* m_camera;
    filament::SwapChain* m_swapChain;
    void* m_nativeWindow; // 存储原生窗口句柄，用于重新创建交换链

public:
    FilamentEngine();
    ~FilamentEngine();

    void initialize(void* nativeWindow, int width, int height);
    void shutdown();

    void render();
    void resize(int width, int height);

    filament::Engine* getEngine() const { return m_engine; }
    filament::Renderer* getRenderer() const { return m_renderer; }
    filament::Scene* getScene() const { return m_scene; }
    filament::View* getView() const { return m_view; }
    filament::Camera* getCamera() const { return m_camera; }

    void setCameraPosition(const filament::math::float3& position);
    void setCameraTarget(const filament::math::float3& target);
    void setCameraProjection(float fov, float aspect, float near, float far);
};

#endif // FILAMENTENGINE_H
