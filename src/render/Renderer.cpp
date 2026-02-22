#include "Renderer.h"
#include "app/AppConfig.h"
#include "core/asset/IBL/IBL.h"

#include <backend/DriverEnums.h>

#include <filament/TransformManager.h>
#include <filament/Options.h>
#include <filament/Skybox.h>
#include <filament/LightManager.h>
#include <filament/RenderableManager.h>
#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>
#include <filament/View.h>
#include <filament/Camera.h>
#include <filament/SwapChain.h>
#include <filament/Viewport.h>

#include <utils/Entity.h>
#include <utils/EntityManager.h>
#include <utils/NameComponentManager.h>

#include <gltfio/AssetLoader.h>
#include <gltfio/FilamentAsset.h>
#include <gltfio/ResourceLoader.h>
#include <gltfio/TextureProvider.h>

#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QDir>

#include <string>
#include <iostream>

Renderer::Renderer() = default;

Renderer::~Renderer() {
    shutdown();
}

// ───────────────────────────────────────────────────────────────
// Initialization
// ───────────────────────────────────────────────────────────────

void Renderer::initialize(void* nativeWindow, int width, int height) {
    m_nativeWindow = nativeWindow;
    m_width = width;
    m_height = height;

    m_engine = filament::Engine::create(filament::backend::Backend::OPENGL);
    if (!m_engine) {
        qFatal("Failed to create Filament Engine");
        return;
    }

    m_renderer = m_engine->createRenderer();
    m_scene = m_engine->createScene();
    m_uiScene = m_engine->createScene();

    // Cameras
    utils::Entity camEntity = utils::EntityManager::get().create();
    m_camera = m_engine->createCamera(camEntity);

    utils::Entity uiCamEntity = utils::EntityManager::get().create();
    m_uiCamera = m_engine->createCamera(uiCamEntity);

    // Views
    m_view = m_engine->createView();
    m_view->setScene(m_scene);
    m_view->setCamera(m_camera);

    m_uiView = m_engine->createView();
    m_uiView->setScene(m_uiScene);
    m_uiView->setCamera(m_uiCamera);

    // SwapChain
    m_swapChain = m_engine->createSwapChain(nativeWindow);

    // 设置 view 渲染选项
    filament::viewer::BloomOptions bloomOptions;
    filament::viewer::applySettings(m_engine, m_settings, m_view);

    // 创建太阳光
    filament::LightManager::Builder sunBuilder(filament::LightManager::Type::SUN);
    sunBuilder
        .color({ 1.0f, 1.0f, 1.0f })      // 白色光
        .intensity(30000.0f)            // 光强度
        .direction({ 0.5f, -1.0f, -0.5f });  // 光照方向（从上方斜照）
    utils::Entity sun = utils::EntityManager::get().create();
    sunBuilder.build(*m_engine, sun);
    m_scene->addEntity(sun);

    AppConfig cfg = loadAppConfig();
    if (!cfg.defaultModelDir.isEmpty() && QFileInfo::exists(cfg.defaultModel)) {
        m_gltfDirectory = cfg.defaultModelDir.toStdString();
    }

    // 添加默认方向光
    addDirectionalLight({ 0.5f, 1.0f, 0.5f }, { 1.0f, 0.0f, 1.0f }, 100000.0f);

    // 添加默认GLTF模型
    if (!cfg.defaultModel.isEmpty() && QFileInfo::exists(cfg.defaultModel)) {
        addGltfModel(cfg.defaultModel);
    }

    // 添加IBL
    if (!cfg.defaultIBL.isEmpty()) {
        loadAndApplyIBL(cfg.defaultIBL);
    }

    resize(width, height);
}

// ───────────────────────────────────────────────────────────────
// Shutdown
// ───────────────────────────────────────────────────────────────

void Renderer::shutdown() {
    if (!m_engine) return;

    if (m_swapChain)   m_engine->destroy(m_swapChain);
    if (m_view)        m_engine->destroy(m_view);
    if (m_uiView)      m_engine->destroy(m_uiView);
    if (m_scene)       m_engine->destroy(m_scene);
    if (m_uiScene)     m_engine->destroy(m_uiScene);
    if (m_renderer)    m_engine->destroy(m_renderer);

    filament::Engine::destroy(m_engine);

    m_engine = nullptr;
}

// ───────────────────────────────────────────────────────────────
// Frame
// ───────────────────────────────────────────────────────────────

void Renderer::renderFrame() {
    if (!m_renderer || !m_swapChain) return;

    if (m_renderer->beginFrame(m_swapChain)) {
        m_renderer->render(m_view);
        m_renderer->render(m_uiView);
        m_renderer->endFrame();
    }
}

void Renderer::clearPixel()
{
    // 设置清除选项
    filament::Renderer::ClearOptions clearOptions;
    clearOptions.clear = true;
    clearOptions.clearColor = { 0.1f, 0.1f, 0.1f, 1.0f }; // 深灰色背景
    clearOptions.discard = true;
    m_renderer->setClearOptions(clearOptions);
}

// ───────────────────────────────────────────────────────────────
// Resize
// ───────────────────────────────────────────────────────────────

void Renderer::resize(int width, int height) {
    m_width = width;
    m_height = height;

    filament::Viewport vp(0, 0, width, height);
    m_view->setViewport(vp);
    m_uiView->setViewport(vp);

    m_uiCamera->setProjection(
        filament::Camera::Projection::ORTHO,
        0.0, width,
        0.0, height,
        -1.0, 1.0
    );

    setCameraProjection(
        45.0f,
        float(width) / float(height),
        0.1f,
        1000.0f
    );

    if (m_swapChain) {
        m_engine->destroy(m_swapChain);
        m_swapChain = m_engine->createSwapChain(m_nativeWindow);
    }
}

// ───────────────────────────────────────────────────────────────
// Camera
// ───────────────────────────────────────────────────────────────

void Renderer::setCameraPosition(const filament::math::float3& pos) {
    if (m_camera) {
        m_camera->lookAt(pos, { 0, 0, 0 }, { 0, 1, 0 });
    }
}

void Renderer::setCameraTarget(const filament::math::float3& target) {
    if (m_camera) {
        filament::math::float3 eye{ 0, 0, 5 };
        m_camera->lookAt(eye, target, { 0, 1, 0 });
    }
}

void Renderer::setCameraProjection(float fov, float aspect, float nearPlane, float farPlane) {
    if (m_camera) {
        m_camera->setProjection(
            fov, aspect,
            nearPlane, farPlane,
            filament::Camera::Fov::VERTICAL
        );
    }
}

// ───────────────────────────────────────────────────────────────
// Add Light
// ───────────────────────────────────────────────────────────────

void Renderer::addDirectionalLight(const filament::math::float3& direction, const filament::math::float3& color, float intensity)
{
    if (!m_engine || !m_scene) return;

    // 创建方向光
    filament::LightManager::Builder lightBuilder(filament::LightManager::Type::DIRECTIONAL);
    lightBuilder.color(color);
    lightBuilder.intensity(intensity);
    lightBuilder.direction(direction);

    utils::Entity lightEntity = utils::EntityManager::get().create();
    lightBuilder.build(*m_engine, lightEntity);
    m_scene->addEntity(lightEntity);
}

void Renderer::addPointLight(const filament::math::float3& position, const filament::math::float3& color, float intensity, float radius)
{
    if (!m_engine || !m_scene) return;

    // 创建点光源
    filament::LightManager::Builder lightBuilder(filament::LightManager::Type::POINT);
    lightBuilder.color(color);
    lightBuilder.intensity(intensity);
    lightBuilder.position(position);
    lightBuilder.falloff(radius);

    utils::Entity lightEntity = utils::EntityManager::get().create();
    filament::LightManager::Builder::Result result = lightBuilder.build(*m_engine, lightEntity);
    if (result == filament::LightManager::Builder::Result::Success) {
        m_scene->addEntity(lightEntity);
    }
}

// ───────────────────────────────────────────────────────────────
// Add Renderable
// ───────────────────────────────────────────────────────────────

void Renderer::addCube(const filament::math::float3& position, const filament::math::float3& size, const filament::math::float3& color)
{
    if (!m_engine || !m_scene) return;

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
        .build(*m_engine);

    vertexBuffer->setBufferAt(*m_engine, 0, filament::VertexBuffer::BufferDescriptor(
        vertices, sizeof(vertices), nullptr));

    // 创建索引缓冲区
    filament::IndexBuffer* indexBuffer = filament::IndexBuffer::Builder()
        .indexCount(36)
        .bufferType(filament::IndexBuffer::IndexType::USHORT)
        .build(*m_engine);

    indexBuffer->setBuffer(*m_engine, filament::IndexBuffer::BufferDescriptor(
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
        .boundingBox({ {-halfX, -halfY, -halfZ}, {halfX, halfY, halfZ} })
        .geometry(0, filament::RenderableManager::PrimitiveType::TRIANGLES, vertexBuffer, indexBuffer)
        .build(*m_engine, cubeEntity);

    // 设置立方体位置
    auto& transformManager = m_engine->getTransformManager();
    transformManager.setTransform(transformManager.getInstance(cubeEntity),
        filament::math::mat4f::translation(position));

    // 将立方体添加到场景
    m_scene->addEntity(cubeEntity);
}

void Renderer::addGltfModel(const QString& file) {
    // 1. 创建 AssetLoader
    filament::gltfio::MaterialProvider* materials = filament::gltfio::createJitShaderProvider(m_engine, true);
    if (!materials) {
        std::cerr << "Failed to create MaterialProvider" << std::endl;
        return;
    }

    utils::NameComponentManager* names = new utils::NameComponentManager(utils::EntityManager::get());
    filament::gltfio::AssetLoader* assetLoader = filament::gltfio::AssetLoader::create({ m_engine, materials, names });
    if (!assetLoader) {
        std::cerr << "Failed to create AssetLoader" << std::endl;
        delete names;
        return;
    }

    // 2. 读取 glTF 文件（使用 Qt 方法）
    QFile qFile(file);
    if (!qFile.open(QIODevice::ReadOnly)) {
        std::cerr << "Failed to open file: " << qFile.errorString().toStdString() << std::endl;
        std::cerr << "File path: " << file.toStdString() << std::endl;
        // 清理资源
        delete names;
        return;
    }

    QByteArray fileData = qFile.readAll();
    qFile.close();

    std::vector<uint8_t> buffer(fileData.begin(), fileData.end());
    std::cout << "File size: " << buffer.size() << " bytes" << std::endl;

    // 3. 检查文件大小
    if (buffer.empty()) {
        std::cerr << "Failed to read glTF file: buffer is empty" << std::endl;
        return;
    }

    // 4. 解析 glTF
    filament::gltfio::FilamentAsset* asset = assetLoader->createAsset(buffer.data(), buffer.size());
    if (!asset) {
        std::cerr << "Failed to parse glTF: asset is null" << std::endl;
        return;
    }

    // 检查实例是否有效
    filament::gltfio::FilamentInstance* instance = asset->getInstance();
    if (!instance) {
        std::cerr << "Failed to get instance from asset" << std::endl;
        assetLoader->destroyAsset(asset);
        return;
    }

    // 5. 获取文件目录路径
    QDir fileDir = QFileInfo(file).dir();
    QString binFileName = fileDir.absoluteFilePath(QFileInfo(file).baseName() + ".bin");
    QFile binFile(binFileName);
    if (binFile.exists()) {
        std::cout << "bin file exists: " << binFileName.toStdString() << std::endl;
    }
    else {
        std::cerr << "bin file does not exist: " << binFileName.toStdString() << std::endl;
        // 尝试查找所有 .bin 文件
        QStringList binFiles = fileDir.entryList(QStringList() << "*.bin");
        std::cout << "Found bin files: " << binFiles.size() << std::endl;
        for (const QString& binFile : binFiles) {
            std::cout << "  - " << binFile.toStdString() << std::endl;
        }
    }

    // 6. 创建 ResourceLoader
    filament::gltfio::ResourceConfiguration config = {};
    config.engine = m_engine;
    config.gltfPath = m_gltfDirectory.c_str();
    config.normalizeSkinningWeights = true;

    filament::gltfio::ResourceLoader* resourceLoader = new filament::gltfio::ResourceLoader(config);
    if (!resourceLoader) {
        std::cerr << "Failed to create ResourceLoader" << std::endl;
        return;
    }

    // 7. 创建纹理提供者
    filament::gltfio::TextureProvider* stbDecoder = filament::gltfio::createStbProvider(m_engine);
    filament::gltfio::TextureProvider* ktxDecoder = filament::gltfio::createKtx2Provider(m_engine);
    if (!stbDecoder) {
        std::cerr << "Failed to create StbProvider" << std::endl;
        return;
    }

    resourceLoader->addTextureProvider("image/png", stbDecoder);
    resourceLoader->addTextureProvider("image/jpeg", stbDecoder);
    resourceLoader->addTextureProvider("image/ktx2", ktxDecoder);
    //if (true) {
    //    filament::gltfio::TextureProvider *webpDecoder = filament::gltfio::createWebpProvider(engine);
    //    resourceLoader->addTextureProvider("image/webp", webpDecoder);
    //}

    // 8. 同步加载资源
    if (!resourceLoader->loadResources(asset)) {
        std::cerr << "Unable to start loading resources for " << file.toStdString() << std::endl;
        exit(1);
    }

    // 10. 释放源数据
    asset->releaseSourceData();
    std::cout << "Source data released" << std::endl;

    // 11. 添加到场景
    auto& tcm = m_engine->getTransformManager();
    auto root = tcm.getInstance(asset->getRoot());
    filament::math::mat4f transform;
    tcm.setTransform(root, transform);
    m_scene->addEntities(asset->getLightEntities(), asset->getLightEntityCount());
    auto& rcm = m_engine->getRenderableManager();
    for (size_t i = 0, n = asset->getRenderableEntityCount(); i < n; i++) {
        auto ri = rcm.getInstance(asset->getRenderableEntities()[i]);
        rcm.setScreenSpaceContactShadows(ri, true);
    }
    const utils::Entity* renderables = asset->getRenderableEntities();
    size_t count = asset->getRenderableEntityCount();
    if (renderables && count > 0) {
        for (size_t i = 0; i < count; i++) {
            m_scene->addEntity(renderables[i]);
        }
        std::cout << "Successfully loaded glTF model: " << file.toStdString() << std::endl;
    }
    else {
        std::cerr << "No renderable entities found in glTF" << std::endl;
    }
}

// ───────────────────────────────────────────────────────────────
// Load IBL
// ───────────────────────────────────────────────────────────────
bool Renderer::loadAndApplyIBL(const QString& iblPath) {
    IBL* ibl = new IBL(*m_engine);

    QByteArray pathUtf8 = iblPath.toUtf8();
    if (!ibl->loadFromKtx(pathUtf8.constData())) {
        std::cerr << "Could not load the specified IBL: " << iblPath.toStdString() << std::endl;
        ibl = nullptr;
        return false;
    }

    if (ibl != nullptr) {
        ibl->getSkybox()->setLayerMask(~0u, ~0u);
        //ibl->getSkybox()->setLayerMask(0x7, 0x4);
        m_scene->setSkybox(ibl->getSkybox());
        m_scene->setIndirectLight(ibl->getIndirectLight());
    }

    // 可选：应用雾效纹理（如果需要）
    filament::Texture* fogTexture = ibl->getFogTexture();
    if (fogTexture) {
        // 这里可以设置雾效纹理到场景或视图
        //view->getSettings().view.fogSettings.fogColorTexture = fogTexture;
    }

    std::cout << "Successfully loaded and applied IBL: " << iblPath.toStdString() << std::endl;
    return true;
}