#include "IRenderer.h"
#include "RenderContext.h"

#include "scene/Node.h"

#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>
#include <filament/View.h>
#include <filament/Camera.h>
#include <filament/SwapChain.h>
#include <filament/LightManager.h>
#include <filament/RenderableManager.h>
#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <filament/TransformManager.h>

#include <utils/EntityManager.h>
#include <utils/Entity.h>

namespace Kazia {

class FilamentRenderer : public IRenderer {
private:
    std::shared_ptr<RenderContext> m_context;
    void* m_nativeWindow;
    
public:
    FilamentRenderer() {
        m_context = std::make_shared<RenderContext>();
    }
    
    ~FilamentRenderer() override {
        shutdown();
    }
    
    bool initialize(void* nativeWindow, int width, int height) override {
        m_nativeWindow = nativeWindow;
        
        // 创建 Filament 引擎
        m_context->engine = filament::Engine::create();
        if (!m_context->engine) {
            return false;
        }
        
        // 创建渲染器
        m_context->renderer = m_context->engine->createRenderer();
        
        // 创建场景
        m_context->scene = m_context->engine->createScene();
        
        // 创建相机
        utils::Entity cameraEntity = utils::EntityManager::get().create();
        m_context->camera = m_context->engine->createCamera(cameraEntity);
        
        // 创建视图
        m_context->view = m_context->engine->createView();
        m_context->view->setScene(m_context->scene);
        m_context->view->setCamera(m_context->camera);
        
        // 创建交换链
        m_context->swapChain = m_context->engine->createSwapChain(nativeWindow);
        
        // 设置窗口大小
        m_context->width = width;
        m_context->height = height;
        resize(width, height);
        
        // 设置初始相机参数
        setCameraProjection(45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
        setCameraPosition({0.0f, 0.0f, 5.0f});
        setCameraTarget({0.0f, 0.0f, 0.0f});
        
        // 设置清除选项
        m_context->clearOptions.clear = true;
        m_context->clearOptions.clearColor = {0.1f, 0.1f, 0.1f, 1.0f};
        m_context->clearOptions.discard = true;
        
        // 创建实体映射器
        m_context->entityMapper = std::make_unique<FilamentEntityMapper>(m_context->engine);
        
        return m_context->isValid();
    }
    
    void shutdown() override {
        if (m_context->isValid()) {
            // 销毁交换链
            if (m_context->swapChain) {
                m_context->engine->destroy(m_context->swapChain);
                m_context->swapChain = nullptr;
            }
            
            // 销毁视图
            if (m_context->view) {
                m_context->engine->destroy(m_context->view);
                m_context->view = nullptr;
            }
            
            // 销毁场景
            if (m_context->scene) {
                m_context->engine->destroy(m_context->scene);
                m_context->scene = nullptr;
            }
            
            // 销毁渲染器
            if (m_context->renderer) {
                m_context->engine->destroy(m_context->renderer);
                m_context->renderer = nullptr;
            }
            
            // 销毁引擎
            if (m_context->engine) {
                filament::Engine::destroy(m_context->engine);
                m_context->engine = nullptr;
            }
            
            // 重置相机
            m_context->camera = nullptr;
            
            // 清理实体映射器
            m_context->entityMapper.reset();
        }
    }
    
    void beginFrame() override {
        if (m_context->isValid() && m_context->renderer && m_context->swapChain) {
            m_context->renderer->beginFrame(m_context->swapChain);
        }
    }
    
    void render() override {
        if (m_context->isValid() && m_context->renderer && m_context->view) {
            m_context->renderer->render(m_context->view);
        }
    }
    
    void endFrame() override {
        if (m_context->isValid() && m_context->renderer) {
            m_context->renderer->endFrame();
        }
    }
    
    void resize(int width, int height) override {
        if (m_context->isValid()) {
            m_context->width = width;
            m_context->height = height;
            
            // 更新视口
            filament::Viewport viewport(0, 0, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
            m_context->view->setViewport(viewport);
            
            // 更新相机投影
            if (m_context->camera) {
                float aspect = static_cast<float>(width) / static_cast<float>(height);
                m_context->camera->setProjection(45.0f, aspect, 0.1f, 1000.0f, filament::Camera::Fov::VERTICAL);
            }
            
            // 重新创建交换链
            if (m_context->swapChain) {
                m_context->engine->destroy(m_context->swapChain);
                m_context->swapChain = m_context->engine->createSwapChain(m_nativeWindow);
            }
        }
    }
    
    void addMesh(const std::string& meshName, const std::string& meshPath) override {
        // 实现网格加载逻辑
        // 这里需要使用 glTF 加载器来加载网格
        // 暂时创建一个简单的立方体作为示例
        if (m_context->isValid()) {
            // 创建立方体实体
            utils::Entity cubeEntity = utils::EntityManager::get().create();
            
            // 立方体顶点数据
            float halfSize = 0.5f;
            static const float vertices[] = {
                // 前面
                -halfSize, -halfSize,  halfSize,
                 halfSize, -halfSize,  halfSize,
                 halfSize,  halfSize,  halfSize,
                -halfSize,  halfSize,  halfSize,
                // 后面
                -halfSize, -halfSize, -halfSize,
                -halfSize,  halfSize, -halfSize,
                 halfSize,  halfSize, -halfSize,
                 halfSize, -halfSize, -halfSize,
                // 上面
                -halfSize,  halfSize, -halfSize,
                -halfSize,  halfSize,  halfSize,
                 halfSize,  halfSize,  halfSize,
                 halfSize,  halfSize, -halfSize,
                // 下面
                -halfSize, -halfSize, -halfSize,
                 halfSize, -halfSize, -halfSize,
                 halfSize, -halfSize,  halfSize,
                -halfSize, -halfSize,  halfSize,
                // 右面
                 halfSize, -halfSize, -halfSize,
                 halfSize,  halfSize, -halfSize,
                 halfSize,  halfSize,  halfSize,
                 halfSize, -halfSize,  halfSize,
                // 左面
                -halfSize, -halfSize, -halfSize,
                -halfSize, -halfSize,  halfSize,
                -halfSize,  halfSize,  halfSize,
                -halfSize,  halfSize, -halfSize
            };
            
            // 立方体索引数据
            static const uint16_t indices[] = {
                // 前面
                0, 1, 2, 2, 3, 0,
                // 后面
                4, 5, 6, 6, 7, 4,
                // 上面
                8, 9, 10, 10, 11, 8,
                // 下面
                12, 13, 14, 14, 15, 12,
                // 右面
                16, 17, 18, 18, 19, 16,
                // 左面
                20, 21, 22, 22, 23, 20
            };
            
            // 创建顶点缓冲区
            filament::VertexBuffer* vertexBuffer = filament::VertexBuffer::Builder()
                .vertexCount(24)
                .bufferCount(1)
                .attribute(filament::VertexAttribute::POSITION, 0, filament::VertexBuffer::AttributeType::FLOAT3)
                .build(*m_context->engine);
            
            vertexBuffer->setBufferAt(*m_context->engine, 0, filament::VertexBuffer::BufferDescriptor(
                vertices, sizeof(vertices), nullptr));
            
            // 创建索引缓冲区
            filament::IndexBuffer* indexBuffer = filament::IndexBuffer::Builder()
                .indexCount(36)
                .bufferType(filament::IndexBuffer::IndexType::USHORT)
                .build(*m_context->engine);
            
            indexBuffer->setBuffer(*m_context->engine, filament::IndexBuffer::BufferDescriptor(
                indices, sizeof(indices), nullptr));
            
            // 创建可渲染对象
            filament::RenderableManager::Builder(1)
                .boundingBox({{-halfSize, -halfSize, -halfSize}, {halfSize, halfSize, halfSize}})
                .geometry(0, filament::RenderableManager::PrimitiveType::TRIANGLES, vertexBuffer, indexBuffer)
                .build(*m_context->engine, cubeEntity);
            
            // 设置立方体位置
            auto& transformManager = m_context->engine->getTransformManager();
            transformManager.setTransform(transformManager.getInstance(cubeEntity),
                filament::math::mat4f::translation({0.0f, 0.0f, 0.0f}));
            
            // 将立方体添加到场景
            m_context->scene->addEntity(cubeEntity);
        }
    }
    
    void removeMesh(const std::string& meshName) override {
        // 实现网格移除逻辑
    }
    
    void setCameraPosition(const math::float3& position) override {
        if (m_context->isValid() && m_context->camera) {
            filament::math::float3 filaPosition = {position.x, position.y, position.z};
            m_context->camera->lookAt(filaPosition, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
        }
    }
    
    void setCameraTarget(const math::float3& target) override {
        if (m_context->isValid() && m_context->camera) {
            filament::math::float3 position = {0.0f, 0.0f, 5.0f};
            filament::math::float3 filaTarget = {target.x, target.y, target.z};
            m_context->camera->lookAt(position, filaTarget, {0.0f, 1.0f, 0.0f});
        }
    }
    
    void setCameraProjection(float fov, float aspect, float near, float far) override {
        if (m_context->isValid() && m_context->camera) {
            m_context->camera->setProjection(fov, aspect, near, far, filament::Camera::Fov::VERTICAL);
        }
    }
    
    void addDirectionalLight(const math::float3& direction, const math::float3& color, float intensity) override {
        if (m_context->isValid()) {
            // 创建方向光
            filament::math::float3 filaDirection = {direction.x, direction.y, direction.z};
            filament::math::float3 filaColor = {color.x, color.y, color.z};
            
            filament::LightManager::Builder lightBuilder(filament::LightManager::Type::DIRECTIONAL);
            lightBuilder.color(filaColor);
            lightBuilder.intensity(intensity);
            lightBuilder.direction(filaDirection);
            
            utils::Entity lightEntity = utils::EntityManager::get().create();
            lightBuilder.build(*m_context->engine, lightEntity);
            m_context->scene->addEntity(lightEntity);
        }
    }
    
    void addPointLight(const math::float3& position, const math::float3& color, float intensity, float radius) override {
        if (m_context->isValid()) {
            // 创建点光源
            filament::math::float3 filaPosition = {position.x, position.y, position.z};
            filament::math::float3 filaColor = {color.x, color.y, color.z};
            
            filament::LightManager::Builder lightBuilder(filament::LightManager::Type::POINT);
            lightBuilder.color(filaColor);
            lightBuilder.intensity(intensity);
            lightBuilder.position(filaPosition);
            lightBuilder.falloff(radius);
            
            utils::Entity lightEntity = utils::EntityManager::get().create();
            lightBuilder.build(*m_context->engine, lightEntity);
            m_context->scene->addEntity(lightEntity);
        }
    }
    
    void setSkybox(const std::string& skyboxPath) override {
        // 实现天空盒设置逻辑
    }
    
    void setIBL(const std::string& iblPath) override {
        // 实现 IBL 设置逻辑
    }
    
    std::shared_ptr<RenderContext> getContext() const override {
        return m_context;
    }
    
    void syncSceneTransforms(const Node* rootNode) override {
        if (m_context->entityMapper) {
            m_context->entityMapper->syncAllTransforms(rootNode);
        }
    }
};

} // namespace Kazia
