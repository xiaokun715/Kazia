#ifndef RENDERER_H
#define RENDERER_H

#include <QString>
#include <viewer/Settings.h>
#include "FilamentEngine.h"

class Renderer
{
private:
    FilamentEngine* m_filamentEngine;
    bool m_isInitialized; // 跟踪渲染器是否已初始化
    std::string m_gltfDirectory;
    filament::viewer::ViewSettings m_settings;

public:
    Renderer();
    ~Renderer();

    void initialize(void* nativeWindow, int width, int height);
    void shutdown();

    void renderFrame();
    void resize(int width, int height);

    FilamentEngine* getFilamentEngine() const { return m_filamentEngine; }

    // 光照控制
    void addDirectionalLight(const filament::math::float3& direction, const filament::math::float3& color, float intensity);
    void addPointLight(const filament::math::float3& position, const filament::math::float3& color, float intensity, float radius);

    // 几何体创建
    void addCube(const filament::math::float3& position, const filament::math::float3& size, const filament::math::float3& color);
	void addGltfModel(const QString& file);
    bool loadAndApplyIBL(const QString& iblPath);

    // 相机控制
    void setCameraPosition(const filament::math::float3& position);
    void setCameraTarget(const filament::math::float3& target);
    void setCameraProjection(float fov, float aspect, float near, float far);
};

#endif // RENDERER_H
