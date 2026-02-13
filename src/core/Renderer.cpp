#include "Renderer.h"
#include <filament/LightManager.h>
#include <filament/RenderableManager.h>
#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <filament/TransformManager.h>
#include <utils/Entity.h>
#include <utils/EntityManager.h>

Renderer::Renderer()
    : m_filamentEngine(nullptr)
    , m_isInitialized(false)
{
    m_filamentEngine = new FilamentEngine();
}

Renderer::~Renderer()
{
    shutdown();
    delete m_filamentEngine;
}

void Renderer::initialize(void* nativeWindow, int width, int height)
{
    if (m_filamentEngine) {
        m_filamentEngine->initialize(nativeWindow, width, height);
        
        // 先设置为已初始化，这样后续的添加操作才能执行
        m_isInitialized = true;

        // 添加默认方向光
        addDirectionalLight({0.5f, -1.0f, 0.5f}, {1.0f, 1.0f, 1.0f}, 100000.0f);

        // 添加一个测试立方体
        addCube({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f});
    }
}

void Renderer::shutdown()
{
    if (m_filamentEngine) {
        m_filamentEngine->shutdown();
        m_isInitialized = false;
    }
}

void Renderer::renderFrame()
{
    if (m_filamentEngine && m_isInitialized) {
        m_filamentEngine->render();
    }
}

void Renderer::resize(int width, int height)
{
    if (m_filamentEngine && m_isInitialized) {
        m_filamentEngine->resize(width, height);
    }
}

void Renderer::addDirectionalLight(const filament::math::float3& direction, const filament::math::float3& color, float intensity)
{
    if (!m_filamentEngine || !m_isInitialized) return;

    auto engine = m_filamentEngine->getEngine();
    auto scene = m_filamentEngine->getScene();

    if (!engine || !scene) return;

    // 创建方向光
    filament::LightManager::Builder lightBuilder(filament::LightManager::Type::DIRECTIONAL);
    lightBuilder.color(color);
    lightBuilder.intensity(intensity);
    lightBuilder.direction(direction);

    utils::Entity lightEntity = utils::EntityManager::get().create();
    lightBuilder.build(*engine, lightEntity);
    scene->addEntity(lightEntity);
}

void Renderer::addPointLight(const filament::math::float3& position, const filament::math::float3& color, float intensity, float radius)
{
    if (!m_filamentEngine || !m_isInitialized) return;

    auto engine = m_filamentEngine->getEngine();
    auto scene = m_filamentEngine->getScene();

    if (!engine || !scene) return;

    // 创建点光源
    filament::LightManager::Builder lightBuilder(filament::LightManager::Type::POINT);
    lightBuilder.color(color);
    lightBuilder.intensity(intensity);
    lightBuilder.position(position);
    lightBuilder.falloff(radius);

    utils::Entity lightEntity = utils::EntityManager::get().create();
    filament::LightManager::Builder::Result result = lightBuilder.build(*engine, lightEntity);
    if (result == filament::LightManager::Builder::Result::Success) {
        scene->addEntity(lightEntity);
    }
}

void Renderer::setCameraPosition(const filament::math::float3& position)
{
    if (m_filamentEngine && m_isInitialized) {
        m_filamentEngine->setCameraPosition(position);
    }
}

void Renderer::setCameraTarget(const filament::math::float3& target)
{
    if (m_filamentEngine && m_isInitialized) {
        m_filamentEngine->setCameraTarget(target);
    }
}

void Renderer::setCameraProjection(float fov, float aspect, float near, float far)
{
    if (m_filamentEngine && m_isInitialized) {
        m_filamentEngine->setCameraProjection(fov, aspect, near, far);
    }
}

void Renderer::addCube(const filament::math::float3& position, const filament::math::float3& size, const filament::math::float3& color)
{
    if (!m_filamentEngine || !m_isInitialized) return;

    auto engine = m_filamentEngine->getEngine();
    auto scene = m_filamentEngine->getScene();

    if (!engine || !scene) return;

    // 创建立方体实体
    utils::Entity cubeEntity = utils::EntityManager::get().create();

    // 立方体顶点数据（8个顶点）
    float halfX = size.x * 0.5f;
    float halfY = size.y * 0.5f;
    float halfZ = size.z * 0.5f;

    static const float vertices[] = {
        // 前面
        -halfX, -halfY,  halfZ,
         halfX, -halfY,  halfZ,
         halfX,  halfY,  halfZ,
        -halfX,  halfY,  halfZ,
        // 后面
        -halfX, -halfY, -halfZ,
        -halfX,  halfY, -halfZ,
         halfX,  halfY, -halfZ,
         halfX, -halfY, -halfZ,
        // 上面
        -halfX,  halfY, -halfZ,
        -halfX,  halfY,  halfZ,
         halfX,  halfY,  halfZ,
         halfX,  halfY, -halfZ,
        // 下面
        -halfX, -halfY, -halfZ,
         halfX, -halfY, -halfZ,
         halfX, -halfY,  halfZ,
        -halfX, -halfY,  halfZ,
        // 右面
         halfX, -halfY, -halfZ,
         halfX,  halfY, -halfZ,
         halfX,  halfY,  halfZ,
         halfX, -halfY,  halfZ,
        // 左面
        -halfX, -halfY, -halfZ,
        -halfX, -halfY,  halfZ,
        -halfX,  halfY,  halfZ,
        -halfX,  halfY, -halfZ
    };

    // 立方体索引数据（36个索引，12个三角形）
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
        .build(*engine);

    vertexBuffer->setBufferAt(*engine, 0, filament::VertexBuffer::BufferDescriptor(
        vertices, sizeof(vertices), nullptr));

    // 创建索引缓冲区
    filament::IndexBuffer* indexBuffer = filament::IndexBuffer::Builder()
        .indexCount(36)
        .bufferType(filament::IndexBuffer::IndexType::USHORT)
        .build(*engine);

    indexBuffer->setBuffer(*engine, filament::IndexBuffer::BufferDescriptor(
        indices, sizeof(indices), nullptr));

    // 创建材质（使用简单的默认材质）
    // 注意：这里简化处理，实际项目中应该使用正确的材质文件
    filament::Material* material = nullptr;
    filament::MaterialInstance* materialInstance = nullptr;
    
    // 尝试创建一个简单的颜色材质
    // 由于没有材质文件，我们暂时使用 nullptr，Filament 会使用默认材质
    // 实际项目中应该创建或加载正确的材质

    // 创建可渲染对象
    filament::RenderableManager::Builder(1)
        .boundingBox({{-halfX, -halfY, -halfZ}, {halfX, halfY, halfZ}})
        .geometry(0, filament::RenderableManager::PrimitiveType::TRIANGLES, vertexBuffer, indexBuffer)
        .build(*engine, cubeEntity);

    // 设置立方体位置
    auto& transformManager = engine->getTransformManager();
    transformManager.setTransform(transformManager.getInstance(cubeEntity),
        filament::math::mat4f::translation(position));

    // 将立方体添加到场景
    scene->addEntity(cubeEntity);
}
