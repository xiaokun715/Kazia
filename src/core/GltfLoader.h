#ifndef GLTFLOADER_H
#define GLTFLOADER_H

#include <string>
#include <vector>
#include <memory>

#include <filament/Engine.h>

namespace Kazia {

class Mesh;
class AssetManager;

class GltfLoader {
private:
    filament::Engine* m_engine;
    AssetManager* m_assetManager;
    
public:
    GltfLoader(filament::Engine* engine, AssetManager* assetManager);
    ~GltfLoader() = default;
    
    // 加载 glTF 文件
    bool loadFromFile(const std::string& path, std::vector<std::shared_ptr<Mesh>>& meshes);
    
private:
    // 解析 glTF 文件
    bool parseGltfFile(const std::string& path, std::vector<std::shared_ptr<Mesh>>& meshes);
    
    // 加载纹理
    bool loadTextures(const std::string& basePath, const void* gltfData, size_t gltfSize);
    
    // 加载材质
    bool loadMaterials(const void* gltfData, size_t gltfSize);
    
    // 加载网格
    bool loadMeshes(const void* gltfData, size_t gltfSize, std::vector<std::shared_ptr<Mesh>>& meshes);
};

} // namespace Kazia

#endif // GLTFLOADER_H
