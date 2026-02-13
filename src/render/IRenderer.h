#ifndef IRENDERER_H
#define IRENDERER_H

#include <memory>
#include <string>
#include <vector>

#include "RenderContext.h"

namespace Kazia {

class Node;

// 简化的数学库
namespace math {
    struct float3 {
        float x, y, z;
        float3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
    };
}

class IRenderer {
public:
    virtual ~IRenderer() = default;
    
    // 初始化和关闭
    virtual bool initialize(void* nativeWindow, int width, int height) = 0;
    virtual void shutdown() = 0;
    
    // 渲染相关
    virtual void beginFrame() = 0;
    virtual void render() = 0;
    virtual void endFrame() = 0;
    
    // 视口和大小调整
    virtual void resize(int width, int height) = 0;
    
    // 场景操作
    virtual void addMesh(const std::string& meshName, const std::string& meshPath) = 0;
    virtual void removeMesh(const std::string& meshName) = 0;
    
    // 相机操作
    virtual void setCameraPosition(const math::float3& position) = 0;
    virtual void setCameraTarget(const math::float3& target) = 0;
    virtual void setCameraProjection(float fov, float aspect, float near, float far) = 0;
    
    // 光照操作
    virtual void addDirectionalLight(const math::float3& direction, const math::float3& color, float intensity) = 0;
    virtual void addPointLight(const math::float3& position, const math::float3& color, float intensity, float radius) = 0;
    
    // 环境设置
    virtual void setSkybox(const std::string& skyboxPath) = 0;
    virtual void setIBL(const std::string& iblPath) = 0;
    
    // 获取上下文
    virtual std::shared_ptr<RenderContext> getContext() const = 0;
    
    // 场景同步
    virtual void syncSceneTransforms(const Node* rootNode) = 0;
};

} // namespace Kazia

#endif // IRENDERER_H
